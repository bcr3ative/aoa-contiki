#ifndef AOA_PLUG_H_
#define AOA_PLUG_H_

#include <stdint.h>

#include "aoa.h"

typedef struct {
	uint8_t led;

	// calibration params in polys2 format
    float P1I;
    float P1III;
    float PII[5];  // for convenient calculation of cw_ccw
    float CW_CCW[31];

    // bias for section I and III, these values can be reduced to int
    float P0I[AOA_INPUTS_NUM];
    float P0III[AOA_INPUTS_NUM];

    // minimum value of maximum incident light accepted as reliable mesurement
    uint16_t MAX_VAL_THRESHOLD;

    //values of each photodetector reading
    uint16_t values[AOA_INPUTS_NUM];

    //interference caused by exterior light on each photodetector
    uint16_t interf[AOA_INPUTS_NUM];
    uint16_t interf2[AOA_INPUTS_NUM];
    int threshold;
} AoAPlug;

void AoAPlugInit(AoAPlug*, uint8_t);
// void select(uint8_t channel);
void read_inputs(uint16_t* output_arr, int mode);
void set_threshold(AoAPlug*);
void set_values(AoAPlug*);
float get_aoa_deg(AoAPlug*);
float get_aoa(AoAPlug*);
uint16_t get_aoa_int(AoAPlug*);
// uint16_t get_aoa_int_force(AoAPlug*);
float calculate_aoa(AoAPlug*);
uint16_t max_value(AoAPlug*);
// void store_maxval_threshold(AoAPlug*, uint16_t);
// uint8_t double_peak();
// void led_on();
// void led_off();

#endif