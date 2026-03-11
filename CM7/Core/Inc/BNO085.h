#ifndef BNO085_H
#define BNO085_H

#include "main.h"

#define BNO085_I2C_ADDR    (0x4A << 1)  // 7-bit addr 0x4A, left-shifted for HAL (or not)
#define ROTATION_VECTOR_ID 0x05        // BNO085 Rotation Vector feature report ID
#define ARVR_ROTATION_VECTOR_ID 0x28   // BNO085 AR/VR_Rotation Vector feature report ID
#define REPORT_FREQ_HZ     50          // Rotation vector report frequency (50 Hz)

typedef struct {
    float fw;
    float fx;
    float fy;
    float fz;
} Quaternion;

void send_feature_command(I2C_HandleTypeDef *hi2c, uint32_t frequency);
int read_quaternion(I2C_HandleTypeDef *hi2c, Quaternion* quat);
Quaternion multiply(Quaternion, Quaternion);
Quaternion conjugate(Quaternion);
Quaternion quaternionDifference(Quaternion, Quaternion);
#endif
