#include "bmp180.h"
#ifdef ENABLE_SENSOR_BMP180_I2C

uint8_t _i2c_readOneByte(el_bmp180_t *device, uint16_t memAddress);
uint16_t _i2c_readTwoBytes(el_bmp180_t *device, uint16_t memAddress);
el_io_status_t _i2c_readBytes(el_bmp180_t *device, uint16_t memAddress, uint8_t pdata[], uint8_t len);
el_io_status_t _bmp180ReadChipId(el_bmp180_t *device);
void _i2c_writeOneByte(el_bmp180_t *device, uint16_t memAddress, uint8_t data);
void _bmp180ReadCalibration(el_bmp180_t *device);
void _bmp180ReadUP(el_bmp180_t *device);
void _bmp180ReadUT(el_bmp180_t *device);
void _bmp180ReadUpAndUt(el_bmp180_t *device);

uint8_t _i2c_readOneByte(el_bmp180_t *device, uint16_t memAddress)
{
	uint8_t data;
	__user_el_i2c_read_bytes(device->hi2c, device->addressR, memAddress, &data, 1);
	return data;
}

uint16_t _i2c_readTwoBytes(el_bmp180_t *device, uint16_t memAddress)
{
	uint8_t data[2];
	__user_el_i2c_read_bytes(device->hi2c, device->addressR, memAddress, data, 2);
	return (data[0] << 8) | data[1];
}

el_io_status_t _i2c_readBytes(el_bmp180_t *device, uint16_t memAddress, uint8_t pdata[], uint8_t len)
{
	return __user_el_i2c_read_bytes(device->hi2c, device->addressR, memAddress, pdata, len);
}

void _i2c_writeOneByte(el_bmp180_t *device, uint16_t memAddress, uint8_t data)
{
	__user_el_i2c_write_bytes(device->hi2c, device->addressW, memAddress, &data, 1);
}

void _bmp180ReadUP(el_bmp180_t *device)
{
	uint8_t pData[3];
	_i2c_writeOneByte(device, BMP180_ADDRESS_MEM, BMP180_REG_CMD_PRESS + (BMP180_OSS << 6));
	el_delay(10);
	_i2c_readBytes(device, BMP180_REG_R_MEM, pData, 3);
	device->calibration->up = ((pData[0] << 16) | (pData[1] << 8) | pData[2]) >> (8 - BMP180_OSS);
}

void _bmp180ReadUT(el_bmp180_t *device)
{
	_i2c_writeOneByte(device, BMP180_ADDRESS_MEM, BMP180_REG_CMD_TEMP);
	el_delay(5);
	device->calibration->ut = _i2c_readTwoBytes(device, BMP180_REG_R_MEM);
}

void _bmp180ReadUpAndUt(el_bmp180_t *device)
{
	_bmp180ReadUT(device);
	_bmp180ReadUP(device);
}

el_io_status_t _bmp180ReadChipId(el_bmp180_t *device)
{
	uint8_t chip_id = _i2c_readOneByte(device, BMP180_REG_R_CHIP_ID);
	return chip_id == BMP180_CHIP_ID ? EL_IO_OK : EL_IO_ERROR;
}

void _bmp180ReadCalibration(el_bmp180_t *device)
{
	device->calibration->cd_ac1 = _i2c_readTwoBytes(device, BMP180_REG_R_AC1);
	device->calibration->cd_ac2 = _i2c_readTwoBytes(device, BMP180_REG_R_AC2);
	device->calibration->cd_ac3 = _i2c_readTwoBytes(device, BMP180_REG_R_AC3);

	device->calibration->cd_ac4 = _i2c_readTwoBytes(device, BMP180_REG_R_AC4);
	device->calibration->cd_ac5 = _i2c_readTwoBytes(device, BMP180_REG_R_AC5);
	device->calibration->cd_ac6 = _i2c_readTwoBytes(device, BMP180_REG_R_AC6);
	
	device->calibration->cd_b1 = _i2c_readTwoBytes(device, BMP180_REG_R_B1);
	device->calibration->cd_b2 = _i2c_readTwoBytes(device, BMP180_REG_R_B2);

	device->calibration->cd_mb = _i2c_readTwoBytes(device, BMP180_REG_R_MB);
	device->calibration->cd_mc = _i2c_readTwoBytes(device, BMP180_REG_R_MC);
	device->calibration->cd_md = _i2c_readTwoBytes(device, BMP180_REG_R_MD);
}

el_bmp180_t *el_bmp180_regist(el_i2c_def *hi2c, const char* deviceName, uint16_t address7Bit)
{
	uint16_t targetAddress = address7Bit == NULL ? DF_BMP180_ADDRESS_7BIT : address7Bit;
	el_bmp180_calibration_t *cali = (el_bmp180_calibration_t *)malloc(sizeof(el_bmp180_calibration_t));
	el_bmp180_t *device = (el_bmp180_t *)malloc(sizeof(el_bmp180_t));
	device->hi2c = hi2c;
	device->addressR = targetAddress << 1;
	device->addressW = (targetAddress << 1) | 0x01;
	device->name = deviceName;
	device->calibration = cali;
	// chip id
	if (_bmp180ReadChipId(device) != EL_IO_OK)
	{
		printf("BMP180 check id faild\r\n");
		return NULL;
	}
	// calibration
	_bmp180ReadCalibration(device);
	return device;
}

void el_bmp180_readStart(el_bmp180_t *device)
{
	_bmp180ReadUpAndUt(device);
}

float el_bmp180_readTemperature(el_bmp180_t *device)
{
	int32_t x1 = ((device->calibration->ut - device->calibration->cd_ac6) * device->calibration->cd_ac5) >> 15;
	int32_t x2 = (device->calibration->cd_mc << 11) / (x1 + device->calibration->cd_md);
	device->calibration->cd_b5 = x1 + x2;
	device->temperature = ((device->calibration->cd_b5 + 8) >> 4) * 0.1;
	return device->temperature;
}

long el_bmp180_readPressure(el_bmp180_t *device)
{
	long p = 0;
	device->calibration->cd_b6 = device->calibration->cd_b5 - 4000;
	long x1 = (device->calibration->cd_b2 * ((device->calibration->cd_b6 * device->calibration->cd_b6) >> 12)) >> 11;
	long x2 = (device->calibration->cd_ac2 * device->calibration->cd_b6) >> 11;
	long x3 = x1 + x2;
	device->calibration->cd_b3 = (((device->calibration->cd_ac1 * 4 + x3) << BMP180_OSS) + 2) / 4;
	x1 = (device->calibration->cd_ac3 * device->calibration->cd_b6) >> 13;
	x2 = (device->calibration->cd_b1 * ((device->calibration->cd_b6 * device->calibration->cd_b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) / 4;
	device->calibration->cd_b4 = (device->calibration->cd_ac4 * (unsigned long)(x3 + 32768)) >> 15;
	device->calibration->cd_b7 = ((unsigned long)device->calibration->up - device->calibration->cd_b3) * (50000 >> BMP180_OSS);
	if (device->calibration->cd_b7 < 0x80000000) {
		p = (device->calibration->cd_b7 * 2) / device->calibration->cd_b4;
	} else {
		p = (device->calibration->cd_b7 / device->calibration->cd_b4) * 2;
	}
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p = p + ((x1 + x2 + 3791) >> 4);
	device->pressure = p;
	return device->pressure;
}

#endif // ENABLE_SENSOR_BMP180_I2C
