/************************************************************************
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU License V2.                            *
* This program is distributed in the hope that it will be useful,      *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU General Public License, version 2 for more details               *
*                                                                      *
* Bare bones ADXL345 i2c example for Arduino 1.0                       *
* by Jens C Brynildsen <http://www.flashgamer.com>                     *
* This version is not reliant of any external lib                      *
* (Adapted for Arduino 1.0 from http://code.google.com/p/adxl345driver)*
*                                                                      *
* Demonstrates use of ADXL345 (using the Sparkfun ADXL345 breakout)    *
* with i2c communication. Datasheet:                                   *
* http://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf *
* If you need more advanced features such as freefall and tap          *
* detection, check out:                                                *
* https://github.com/jenschr/Arduino-libraries                         *
***********************************************************************/

// Cabling for i2c using Sparkfun breakout with an Arduino Uno / Duemilanove:
// Arduino <-> Breakout board
// Gnd      -  GND
// 3.3v     -  VCC
// 3.3v     -  CS
// Analog 4 -  SDA
// Analog 5 -  SLC

// Cabling for i2c using Sparkfun breakout with an Arduino Mega / Mega ADK:
// Arduino <-> Breakout board
// Gnd      -  GND
// 3.3v     -  VCC
// 3.3v     -  CS
// 20       -  SDA
// 21       -  SLC

#include <Wire.h>

#define DEVICE (0x9D) // Device address as specified in data sheet 

byte _buff[6];

char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output. Make sure you set your Serial Monitor to the same!
  Serial.println("init");
  
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeTo(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeTo(POWER_CTL, 0x08);
  
  Serial.println("**********************************************");
  Serial.println("***************Electronica 5 Hz***************");
  Serial.println("******Programa de prueba para el ADXL345******");
  Serial.println("**********************************************");
}

void loop()
{
  menu();
  do{}while(Serial.available() == 0);
  switch (Serial.read())
  {
    case '1':
      //read the x tilt
      do{readAccel(1);}while(Serial.read() != '*');
    break;
    case '2':
      //read the y tilt
      do{readAccel(2);}while(Serial.read() != '*');
    break;
    case '3':
      //read the z tilt
      do{readAccel(3);}while(Serial.read() != '*');
    break;
    default:
      Serial.println("intenta otra vez :)");
    break;
  }
}

void menu()
{
  Serial.println("Que eje quieres medir?");
  Serial.println("1.- x");
  Serial.println("2.- y");
  Serial.println("3.- z");
}

void readAccel(int eje) {
  uint8_t howManyBytesToRead = 6;
  readFrom( DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  if(eje == 1)
  {
    int x = (((int)_buff[1]) << 8) | _buff[0];  
    for (int barx = map(x, -255, 255, 0, 30); barx>0;barx--)
    {
      Serial.print("x");
    }
    Serial.println(x);
    delay(100);
  }
  else if(eje == 2)
  {
    int y = (((int)_buff[3]) << 8) | _buff[2];
    for (int bary = map(y, -255, 255, 0, 30); bary>0; bary--)
    {
      Serial.print("y");
    }
    Serial.println(y);
    delay(100);
  }
  else if(eje == 3)
  {
    int z = (((int)_buff[5]) << 8) | _buff[4];
    for (int barz = map(z, -255, 255, 0, 30); barz>0;barz--)
    {
      Serial.print("z");
    }
    Serial.println(z);
    delay(100);
  }
}

void writeTo(byte address, byte val) {
  Wire.beginTransmission(DEVICE); // start transmission to device 
  Wire.write(address);             // send register address
  Wire.write(val);                 // send value to write
  Wire.endTransmission();         // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void readFrom(byte address, int num, byte _buff[]) {
  Wire.beginTransmission(DEVICE); // start transmission to device 
  Wire.write(address);             // sends address to read from
  Wire.endTransmission();         // end transmission

  Wire.beginTransmission(DEVICE); // start transmission to device
  Wire.requestFrom(DEVICE, num);    // request 6 bytes from device

  int i = 0;
  while(Wire.available())         // device may send less than requested (abnormal)
  { 
    _buff[i] = Wire.read();    // receive a byte
    i++;
  }
  Wire.endTransmission();         // end transmission
}
