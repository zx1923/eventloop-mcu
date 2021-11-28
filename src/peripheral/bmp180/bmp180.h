#ifndef __USER_EL_BMP180_H__
#define __USER_EL_BMP180_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "eos.h"
#ifdef ENABLE_SENSOR_BMP180_I2C

typedef struct {
	short cd_ac1;
	short cd_ac2;
	short cd_ac3;
	unsigned short cd_ac4;
	unsigned short cd_ac5;
	unsigned short cd_ac6;
	short cd_b1;
	short cd_b2;
	short cd_mb;
	short cd_mc;
	short cd_md;
	long cd_b3;
	unsigned long cd_b4;
	long cd_b5;
	long cd_b6;
	unsigned long cd_b7;
	long ut;
	long up;
} el_bmp180_calibration_t;

typedef struct
{
	el_i2c_def *hi2c;
	const char* name;
	uint16_t addressR;
	uint16_t addressW;
	float temperature;
	long pressure;
	el_bmp180_calibration_t *calibration;
} el_bmp180_t;

#define BMP180_CHIP_ID				0x55
#define BMP180_TEMP_STEP			0.1
#define BMP180_REG_R_CHIP_ID	0xD0

// power mode
#define BMP180_POWER_MODE			0x01

// 1 byte write
#define BMP180_ADDRESS_MEM		0xF4

// 1 byte cmd to rite
#define BMP180_REG_CMD_TEMP		0x2E
#define BMP180_REG_R_MEM			0xF6
#define BMP180_REG_CMD_PRESS	0x34

// 不同 power mode 下会有所不同
#define BMP180_OSS						0

// 2 bytes words
#define BMP180_REG_R_AC1			0xAA
#define BMP180_REG_R_AC2			0xAC
#define BMP180_REG_R_AC3			0xAE
#define BMP180_REG_R_AC4			0xB0
#define BMP180_REG_R_AC5			0xB2
#define BMP180_REG_R_AC6			0xB4
#define BMP180_REG_R_B1				0xB6
#define BMP180_REG_R_B2				0xB8
#define BMP180_REG_R_MB				0xBA
#define BMP180_REG_R_MC				0xBC
#define BMP180_REG_R_MD				0xBE

extern el_io_status_t __user_el_i2c_read_bytes(el_i2c_def *hi2c, uint16_t devAddress, uint16_t memAddress, uint8_t *pData, uint16_t pLen);
extern el_io_status_t __user_el_i2c_write_bytes(el_i2c_def *hi2c, uint16_t devAddress, uint16_t memAddress, uint8_t *pData, uint16_t pLen);

el_bmp180_t *el_bmp180_regist(el_i2c_def *hi2c, const char* deviceName, uint16_t address7Bit);
void el_bmp180_readStart(el_bmp180_t *device);
float el_bmp180_readTemperature(el_bmp180_t *device);
long el_bmp180_readPressure(el_bmp180_t *device);

#endif // ENABLE_SENSOR_BMP180_I2C

#ifdef __cplusplus
}
#endif

#endif // __USER_EL_BMP180_H__

