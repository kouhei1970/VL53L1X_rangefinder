# VL53L1X_rangefinder

Using VL53L1X with the Raspberry Pi Pico
========================================

I2C configuration
-----------------

This example is using the i2c0 on the Raspberry Pi Pico.
You must ensure that the pin you are using are those defined by I2C_SDA_PIN and I2C_SCL_PIN in main.c.

Demo code
---------

This demo code allow you to calibrate the offset of the VL53L1X and to get continuous reading of the sensor.
The continuous reading is made in one of the most reliable mode of the sensors : short mode with a timing budget of 200ms. In this mode the ranging distance is limited to 1 meter (approximately).

The output of the demo code is optimized for Teleplot. I highly recommande to use the VS Code plugin.

Limitations
-----------

For now, this code does not support several devices, the I2C address of the device is hard-coded in the functions VL53L1_WriteMulti, VL53L1_ReadMulti and VL53L1_WrByte. There is clearly something to do with the argument *uint16_t dev* in these function, it could become the I2C 7bits address.
