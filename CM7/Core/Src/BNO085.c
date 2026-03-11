#include "BNO085.h"
#include "BNO_085_I2C.h"

void send_feature_command(I2C_HandleTypeDef *hi2c , uint32_t frequency){
	// Construct the Set Feature command to enable rotation vector (quaternion) output
	  uint32_t interval_us = 1000000UL / frequency;        // reporting interval in microseconds
	  uint8_t cmd[21];
	  uint8_t seq = 0;  // sequence number for channel 2
	  cmd[0] = 21;
	  cmd[1] = 0x0;
	  cmd[2] = 0x02;                        // channel 2 (SH-2 control)
	  cmd[3] = seq;                         // sequence number
	  cmd[4] = 0xFD;                        // Report ID: Set Feature Command&#8203;:contentReference[oaicite:7]{index=7}
	  //cmd[5] = ROTATION_VECTOR_ID;          // Feature Report ID: Rotation Vector
	  cmd[5] = ARVR_ROTATION_VECTOR_ID;
	  cmd[6] = 0x00;                        // Feature flags: 0 = normal (non-wake)
	  cmd[7] = 0x00; cmd[8] = 0x00;         // Change sensitivity (LSB, MSB) = 0
	  // Fill 4-byte report interval (little-endian)
	  cmd[9]  = (uint8_t)(interval_us & 0xFF);
	  cmd[10] = (uint8_t)((interval_us >> 8) & 0xFF);
	  cmd[11] = (uint8_t)((interval_us >> 16) & 0xFF);
	  cmd[12] = (uint8_t)((interval_us >> 24) & 0xFF);
	  // 4-byte batch interval (set to 0 for no batching)
	  cmd[13] = cmd[14] = cmd[15] = cmd[16] = 0x00;
	  // 4-byte sensor-specific config (not used for rotation vector)
	  cmd[17] = cmd[18] = cmd[19] = cmd[20] = 0x00;

	  // Transmit the command over I2C
	  if (HAL_I2C_Master_Transmit(hi2c, BNO085_I2C_ADDR, cmd, sizeof(cmd), HAL_MAX_DELAY) != HAL_OK) {
	    Error_Handler();  // transmission error
	  }
	  printf("Feature command sent OK\r\n");
}

int read_quaternion(I2C_HandleTypeDef *hi2c, Quaternion* quat) {
    uint8_t data[21] = {0};

    // 从I2C读取数据（失败则触发系统级错误）
    if (HAL_I2C_Master_Receive(hi2c, BNO085_I2C_ADDR, data, sizeof(data), 10) != HAL_OK) {
        printf("I2C读取失败，触发系统错误\r\n");
        Error_Handler(); // 此函数应包含系统级错误处理（不会返回）
    }

    if (data[9] != ARVR_ROTATION_VECTOR_ID) {
        return 0; // 非目标数据类型，通知调用方
    }

    // 解析原始数据（注意字节序处理）
    int16_t qi= (int16_t)((data[14] << 8) | data[13]);
    int16_t qj = (int16_t)((data[16] << 8) | data[15]);
    int16_t qk = (int16_t)((data[18] << 8) | data[17]);
    int16_t qw = (int16_t)((data[20] << 8) | data[19]);

    // 转换为浮点数并填充结构体
    const float scale = 1.0f / 16384.0f; // 统一缩放因子
    quat->fx = qi * scale;
    quat->fy = qj * scale;
    quat->fz = qk * scale;
    quat->fw = qw * scale;

    return 1; // 成功获取有效数据
}


// Compute the conjugate of a quaternion.
Quaternion conjugate(Quaternion q) {
    Quaternion qc;
    qc.fw = q.fw;
    qc.fx = -q.fx;
    qc.fy = -q.fy;
    qc.fz = -q.fz;
    return qc;
}

// Multiply two quaternions: result = q * p.
Quaternion multiply(Quaternion q, Quaternion p) {
    Quaternion result;
    result.fw = q.fw * p.fw - q.fx * p.fx - q.fy * p.fy - q.fz * p.fz;
    result.fx = q.fw * p.fx + q.fx * p.fw + q.fy * p.fz - q.fz * p.fy;
    result.fy = q.fw * p.fy - q.fx * p.fz + q.fy * p.fw + q.fz * p.fx;
    result.fz = q.fw * p.fz + q.fx * p.fy - q.fy * p.fx + q.fz * p.fw;
    return result;
}

Quaternion quaternionDifference(Quaternion q1, Quaternion q2) {
    // q_diff = q2 * q1^{-1} = q2 * conj(q1)
    Quaternion q1_conj = conjugate(q1);
    return multiply(q2, q1_conj);
}
