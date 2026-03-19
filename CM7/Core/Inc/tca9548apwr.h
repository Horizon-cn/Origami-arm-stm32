//
// Created by Yuchen You on 2024/9/15.
//

#ifndef ROBOTIC_ARM_TCA9548APWR_H
#define ROBOTIC_ARM_TCA9548APWR_H

#include "i2c.h"
#include "gpio.h"

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>

/*********************************************************************
 * DEFINITIONS
 */
#define TCA9548A_SLAVE_ADDR         0x73

#define TCA9548A_WRITE_BIT          0x00
#define TCA9548A_READ_BIT           0x01

#define TCA9548A_CHANNEL_0          0x01
#define TCA9548A_CHANNEL_1          0x02
#define TCA9548A_CHANNEL_2          0x04
#define TCA9548A_CHANNEL_3          0x08
#define TCA9548A_CHANNEL_4          0x10
#define TCA9548A_CHANNEL_5          0x20
#define TCA9548A_CHANNEL_6          0x40
#define TCA9548A_CHANNEL_7          0x80

typedef enum {
    MAG1 = 0,
    MAG2 = 1,
    MAG3 = 2,
    BNO = 4
} I2CPort_e;

/*********************************************************************
 * API FUNCTIONS
 */
void TCA9548A_SetChannel(uint8_t channel);

uint8_t TCA9548A_Init();
uint8_t Mux_Scan();


#endif //ROBOTIC_ARM_TCA9548APWR_H