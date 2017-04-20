#include "aoa_plug.h"

#include <stdlib.h>
#include <math.h>

// Utility function for initializing calibration parameters
void assign(float* output, float* input, int size) {
	int i = 0;
	for (i = 0; i < size; ++i) {
		output[i] = input[i];
	}
}

void AoAPlugInit(AoAPlug* sensor, uint8_t num) {
	uint8_t i;
	// TODO: Init Port
	// sensor->led = (num==1) ? 4 : 3;
	// TODO: Led mode
	sensor->threshold = AOA_NOISE_THRESHOLD;

	// Calibration data for node 6
	sensor->P1I = 0.0714173;
	sensor->P1III = -0.046489;
	float p2[5] = { -0.0000477345334, -0.000450211632, 0.0177757912, 0.299785083, 1.52901584 };
	float p01[AOA_INPUTS_NUM] = { 2.1091110368414916, 2.2355774983709034, 2.1609641590766451, 2.0966530458394756, 2.1919046170290821,
								2.1814950749328847, 2.1297408369438244, 2.0104331504209814, 2.0858827952948205, 2.5062560549434765,
								2.2724596495853122, 1.8477844290539249 };
	float p03[AOA_INPUTS_NUM] = { 1.4291038129149067, 1.6394898887997251, 1.4992743308804475, 1.5152024080966249, 1.5011817478765821,
								1.5492095865283322, 1.589421262851936, 1.4769705670599229, 1.5215779618069172, 1.4054248824421982,
								1.8098479832435357, 1.5176944908435044 };
	assign(sensor->PII, p2, 5);
	assign(sensor->P0I, p01, AOA_INPUTS_NUM);
	assign(sensor->P0III, p03, AOA_INPUTS_NUM);
	sensor->MAX_VAL_THRESHOLD = 40; // TODO: verify

	// TODO: Load calibration data from flash

	for (i = -15; i < 16; ++i) { // dont change int to uint8_t
		sensor->CW_CCW[i + 15] = sensor->PII[0] * pow(i, 4) + sensor->PII[1] * pow(i, 3) + sensor->PII[2] * pow(i, 2) + sensor->PII[3] * i + sensor->PII[4];
	}
}

// TODO: Implementation
// void select(uint8_t channel);

// Reimplemented method for testing purposes
void read_inputs(uint16_t* output_arr, int mode) {
	uint8_t i;
	if (mode == 1) {
		int interf[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
		for (i = 0; i < AOA_INPUTS_NUM; ++i) output_arr[i] = interf[i];
	} else if (mode == 2) {
		int interf2[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
		for (i = 0; i < AOA_INPUTS_NUM; ++i) output_arr[i] = interf2[i];
	} else {
		int values[12] = { 7,4,4,0,4,4,0,17,35,96,48,16 };
		for (i = 0; i < AOA_INPUTS_NUM; ++i) output_arr[i] = values[i];
	}
}

void set_threshold(AoAPlug* sensor) {
	uint8_t i;
	int noise = 0;
	sensor->threshold = AOA_NOISE_THRESHOLD;

	read_inputs(sensor->interf, 1);
	read_inputs(sensor->interf2, 2);

	for (i = 0; i < AOA_INPUTS_NUM; ++i) {
		noise = sensor->interf2[i] - sensor->interf[i];
		if (abs(noise) > sensor->threshold) {
			sensor->threshold = abs(noise) + AOA_NOISE_THRESHOLD;
		}
	}
}

void set_values(AoAPlug* sensor) {
	uint8_t i;
	int diff = 0;

	read_inputs(sensor->values, 3);
	for (i = 0; i < AOA_INPUTS_NUM; ++i) {
		diff = sensor->values[i] - sensor->interf[i];
		// set to 0 all values that are above interf by less than threshold
		sensor->values[i] = (diff > sensor->threshold) ? (uint16_t)diff : 0;
	}
}

float get_aoa_deg(AoAPlug* sensor) {
	set_values(sensor);
	// if double_peak() is implemented change force version
	if (max_value(sensor) < sensor->MAX_VAL_THRESHOLD) {
		return AOA_DEG_FAIL;
	}
	return calculate_aoa(sensor);
}

float get_aoa(AoAPlug* sensor) {
	float aoa = get_aoa_deg(sensor);

	if (aoa == AOA_DEG_FAIL) {
		return AOA_FLOAT_FAIL;
	}
	return aoa * M_PI / 180;
}

uint16_t get_aoa_int(AoAPlug* sensor) {
	float aoa = get_aoa(sensor);

	if (aoa == AOA_FLOAT_FAIL) {
		return AOA_INT_FAIL;
	}
	return (uint16_t)(aoa*10000);
}

// Function not used
/*uint16_t get_aoa_int_force(AoAPlug* sensor) {
	uint16_t tmp = sensor->MAX_VAL_THRESHOLD;

	sensor->MAX_VAL_THRESHOLD = 0; // temporarily remove threshold
	float aoa=get_aoa(sensor);

	sensor->MAX_VAL_THRESHOLD = tmp; // restore original threshold
	return (uint16_t)(aoa*10000);
}*/

float calculate_aoa(AoAPlug* sensor) {
	uint8_t m = 0;    // index of values array element with maximum value
	uint8_t ccw = 0;  // index of values array element ccw from m
	uint8_t cw = 0;   // index of values array element cw from m
	//uint8_t section;  // section used to calculate aoa: should be 1, 2 or 3
	float v_m, v_ccw, v_cw, v_cw_ccw, aoa;
	uint8_t i;

	for (i = 1; i < AOA_INPUTS_NUM; ++i) {
		if (sensor->values[i] > sensor->values[m]) m = i;
	}
	ccw = (m + AOA_INPUTS_NUM - 1) % AOA_INPUTS_NUM;
	cw = (m + 1) % AOA_INPUTS_NUM;
	v_m = sensor->values[m];
	v_cw = sensor->values[cw];
	v_ccw = sensor->values[ccw];
	v_cw_ccw = v_cw / v_ccw;

	// interpolate
	for (i = 1; i < 31; ++i) {
		if (v_cw_ccw<sensor->CW_CCW[i]) break;
	}

	// calculate aoa relative to m, based on chosen section
	if (i < AOA_I_MARGIN) {
		//section = 1; //I
		aoa = (v_m / v_ccw-sensor->P0I[ccw]) / sensor->P1I;
	} else if (i > (30 - AOA_III_MARGIN)) {
		//section = 3; //III
		aoa = (v_m / v_cw-sensor->P0III[ccw]) / sensor->P1III;
	} else {
		//section = 2; //II
		aoa = i - 16 + (v_cw_ccw-sensor->CW_CCW[i-1]) / (sensor->CW_CCW[i] - sensor->CW_CCW[i-1]);
	}

	// get absolute aoa and put it in 0 to 360 range
	aoa += m * 30.0;
	aoa = aoa < 0 ? aoa + 360 : aoa;
	return aoa;
}

uint16_t max_value(AoAPlug* sensor) {
	uint8_t i;
	uint16_t max = sensor->values[0];

	for (i = 1; i < AOA_INPUTS_NUM; ++i) {
		if (sensor->values[i] > max) {
			max = sensor->values[i];
		}
	}
	return max;
}

// Function not used
/*void store_maxval_threshold(AoAPlug* sensor, uint16_t maxval_threshold) {
	if (sensor->MAX_VAL_THRESHOLD!=maxval_threshold) {
		sensor->MAX_VAL_THRESHOLD = maxval_threshold;
		// TODO: flash write
		//eeprom_write_word(MAXVALTHR_EEPROM_ADDR, MAX_VAL_THRESHOLD);
	}
}*/

//uint8_t double_peak();

// TODO: Implementation
// void led_on();

// TODO: Implementation
// void led_off();
