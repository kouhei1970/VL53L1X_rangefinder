# VL53L1X_rangefinder

Using VL53L1X with the Raspberry Pi Pico
========================================

I2C configuration
-----------------

This example is using the *i2c0* on the Raspberry Pi Pico.
You must ensure that the pin you are using are those defined by I2C_SDA_PIN and I2C_SCL_PIN in [main.c](main.c).

Demo code
---------

This demo code allow you to calibrate the offset of the VL53L1X and to get continuous reading of the sensor.
The continuous reading is made in one of the most reliable modes of the sensor : short mode with a timing budget of 200ms. In this mode the ranging distance is limited to 1 meter (approximately).

The output of the demo code is optimized for [Teleplot](https://github.com/nesnes/teleplot/). I highly recommande to use the VS Code plugin for Teleplot.

Usage
-----

For now, this code does support several devices. The first argument of the functions of the VL53L1X API should be the 7 bits adresse of the device you want to use. Beware, all devices start with the same address. It is your job to use their *XSHUT* pin to disable all of them except one to change their I2C address. Design your electronic accordingly!
