#ifndef AOA_H_
#define AOA_H_

#define AOA_INPUTS_NUM 12 //number of phototransistors on aoa sensor
#define AOA_SWITCH_DELAY 5 // delay until next phototransistor is selected in ms
#define AOA_NOISE_THRESHOLD 2
#define AOA_I_MARGIN 8 // margin of section I when calculating aoa
#define AOA_III_MARGIN 8 // margin of section I when calculating aoa

// TODO: FLASH layout for calibration data

#define AOA_DEG_FAIL -1
#define AOA_FLOAT_FAIL -1
#define AOA_INT_FAIL 63000

#define M_PI 3.14159265358979323846264338327

#endif
