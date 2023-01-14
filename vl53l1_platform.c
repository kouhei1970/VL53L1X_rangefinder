
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"


#define VL53L1X_7BITS_ADDRESS 0x29

#define I2C_SUCCESS 0
#define I2C_FAILED -1
#define I2C_BUFFER_EXCEEDED -2

#define MAX_I2C_BUFFER 100

/// @brief Blocking function allowing to write a register on an I2C device
/// @param address_7_bits 
/// @param index : register to write
/// @param values : values to write
/// @param count : number of byte to send
/// @return 0: Success, -1 or -2: Failed 
int8_t i2c_write_register(char adresse_7_bits, uint16_t index, uint8_t * values, uint32_t count){
    int statu;
	uint8_t buffer[MAX_I2C_BUFFER];
    uint8_t index_to_unint8[2];

	if(count > MAX_I2C_BUFFER - 2){
		return I2C_BUFFER_EXCEEDED;
	}

	index_to_unint8[0] =  (index >> 8) & 0xFF;
	index_to_unint8[1] =  index & 0xFF;

	buffer[0] = index_to_unint8[0];
	buffer[1] = index_to_unint8[1];
	
	for(uint32_t i=0; i<count; i++){
		buffer[2+i] = values[i];
	}

    statu = i2c_write_blocking (i2c0, adresse_7_bits, buffer, count + 2, 0);
    if(statu == PICO_ERROR_GENERIC){
        printf("Erreur ecrire registre\n");
        return I2C_FAILED;
    }

    return I2C_SUCCESS;
}


/// @brief Blocking function allowing to write a register on an I2C device
/// @param address_7_bits 
/// @param index : register to write
/// @param values : values to write
/// @param count : number of byte to send
/// @return 0: Success, -1 or -2: Failed 
int8_t i2c_read_register(char adresse_7_bits, uint16_t index, uint8_t *pdata, uint32_t count){
    int statu;
	uint8_t buffer[MAX_I2C_BUFFER];
    uint8_t index_to_unint8[2];

	index_to_unint8[0] =  (index >> 8) & 0xFF;
	index_to_unint8[1] =  index & 0xFF;

    statu = i2c_write_blocking (i2c0, adresse_7_bits, index_to_unint8, 2, 0);
    if(statu == PICO_ERROR_GENERIC){
        printf("I2C - Envoi registre Echec\n");
        return I2C_FAILED;
    }

    statu = i2c_read_blocking (i2c0, adresse_7_bits, pdata, count, 0);
    if(statu == PICO_ERROR_GENERIC){
        printf("I2C - Lecture registre Echec\n");
        return I2C_FAILED;
    }

    return I2C_SUCCESS;
}



int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	return i2c_write_register(VL53L1X_7BITS_ADDRESS, index, pdata, count);
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	return i2c_read_register(VL53L1X_7BITS_ADDRESS, index, pdata, count);
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	return i2c_write_register(VL53L1X_7BITS_ADDRESS, index, &data, 1);
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	uint8_t buffer[2]; 
	buffer[0] =  (data >> 8) & 0xFF;
	buffer[1] =  data & 0xFF;

	return VL53L1_WriteMulti(dev, index, buffer, 2);
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	uint8_t buffer[4]; 
	buffer[0] =  (data >> 24) & 0xFF;
	buffer[1] =  (data >> 16) & 0xFF;
	buffer[2] =  (data >> 8) & 0xFF;
	buffer[3] =  data & 0xFF;

	return VL53L1_WriteMulti(dev, index, buffer, 4);
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	return VL53L1_ReadMulti(dev, index, data, 1);
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	int8_t status;
	uint8_t buffer[2];
	status = VL53L1_ReadMulti(dev, index, buffer, 2);
	*data = (uint16_t) buffer[1] | (uint16_t) buffer[0] << 8;

	return status;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	int8_t status;
	uint8_t buffer[4];
	status = VL53L1_ReadMulti(dev, index, buffer, 4);
	*data = (uint32_t) buffer[0] << 24 |
			(uint32_t) buffer[1] << 16 | 
			(uint32_t) buffer[2] << 8 |
			(uint32_t) buffer[3];

	return status;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	sleep_ms(wait_ms);
	return 0;
}
