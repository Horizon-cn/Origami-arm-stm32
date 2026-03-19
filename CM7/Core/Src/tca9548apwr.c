//
// Created by Yuchen You on 2024/9/15.
//

#include "tca9548apwr.h"

/*********************************************************************
 * INCLUDES
 */
#include "i2c.h"

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/**
 @brief 选择通道打开
 @param channel -[in] 通道号
 @return 无
*/
void TCA9548A_SetChannel(uint8_t channel) {
    uint8_t data;
    switch (channel) {
        case 0:
            data = TCA9548A_CHANNEL_0;
            break;
        case 1:
            data = TCA9548A_CHANNEL_1;
            break;
        case 2:
            data = TCA9548A_CHANNEL_2;
            break;
        case 3:
            data = TCA9548A_CHANNEL_3;
            break;
        case 4:
            data = TCA9548A_CHANNEL_4;
            break;
        case 5:
            data = TCA9548A_CHANNEL_5;
            break;
        case 6:
            data = TCA9548A_CHANNEL_6;
            break;
        case 7:
            data = TCA9548A_CHANNEL_7;
            break;
        default:
            break;
    }
    HAL_I2C_Master_Transmit(&hi2c1, (TCA9548A_SLAVE_ADDR << 1) | TCA9548A_WRITE_BIT, &data, 1, 10);
}

uint8_t TCA9548A_Init() {
    uint8_t readBuffer;
    HAL_I2C_Master_Receive(&hi2c1, (TCA9548A_SLAVE_ADDR << 1) | TCA9548A_READ_BIT, &readBuffer, 1, 20);
    return readBuffer;
}
uint8_t Mux_Scan(){

    uint8_t address;
    for (address = 0x00; address < 0xFF;address++ ) {
        if (HAL_I2C_IsDeviceReady(&hi2c1, (address << 1) | TCA9548A_WRITE_BIT, 1, 10) == HAL_OK) {
            return address;
        }
    }
    return 0;
}
/****************************************************END OF FILE****************************************************/
