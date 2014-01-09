
/******************************************************************
** Código creado en Electrónica 5Hz                              **
** www.5hz-electronica.com                                       **
** Por: Jens C Brynildsen                                        **
** Basado en el código de: www.sparkfun.com                      **
** Descripción del código:                                       **
**                                                               **
** Ejemplo para demostrar el uso del acelerómetro ADXL345 con    **
** comunicación i2c                                              **
*******************************************************************

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Uno / Duemilanove:
Arduino <-> Breakout board
Gnd      -  GND
3.3v     -  VCC
3.3v     -  CS
Analog 4 -  SDA
Analog 5 -  SCL

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Mega / Mega ADK:
Arduino <-> Breakout board
Gnd      -  GND
3.3v     -  VCC
3.3v     -  CS
20       -  SDA
21       -  SCL
*/

#include <Wire.h>

//definiciones/////////////////////////
#define DEVICE (0x9D) // Direccion del dispositivo como se especifica en la hoja de datos 

byte _buff[6];

char POWER_CTL = 0x2D;	//Registro de Control de Potencia
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//Dato Eje X 0
char DATAX1 = 0x33;	//Dato Eje X 1
char DATAY0 = 0x34;	//Dato Eje Y 0
char DATAY1 = 0x35;	//Dato Eje Y 1
char DATAZ0 = 0x36;	//Dato Eje Z 0
char DATAZ1 = 0x37;	//Dato Eje Z 1

void setup()
{
  Wire.begin();        // unirse a bus i2c (dirección opcional para master)
  Serial.begin(9600);  // inicia serial para la salida. Asegurarse de que se ajustó el Monitor Serial a la misma velocidad. 
  Serial.print("init");
  
  //Poner el ADXL345 en un rango de +/- 4G escribiendo el valor 0x01 al registro DATA_FORMAT. 
  writeTo(DATA_FORMAT, 0x01);
  //Poner el ADXL345 en Modo de Medición escribiendo el valor 0x08 al registro POWER_CTL. 
  writeTo(POWER_CTL, 0x08);
}

void loop()
{
  readAccel(); // lee la inclinación x/y/z 
  delay(100); // sólo lee cada 0.5 segundos 
}

void readAccel() {
  uint8_t howManyBytesToRead = 6;
  readFrom( DATAX0, howManyBytesToRead, _buff); //lee los datos de aceleración del ADXL345 

  // cada lectura de eje viene con una resolución de 10 bit, es decir, 2 bytes. ¡Primero el Byte menos significativo! 
  // de esta forma, estamos convirtiendo los dos bytes en un int 
  int x = (((int)_buff[1]) << 8) | _buff[0];   
  int y = (((int)_buff[3]) << 8) | _buff[2];
  int z = (((int)_buff[5]) << 8) | _buff[4];
  Serial.print("x: ");
  Serial.print( x );
  Serial.print(" y: ");
  Serial.print( y );
  Serial.print(" z: ");
  Serial.println( z );
}

void writeTo(byte address, byte val) {
  Wire.beginTransmission(DEVICE); // empieza la transmisión al dispositivo  
  Wire.write(address);             // envía dirección de registro 
  Wire.write(val);                 // envía el valor a escribir 
  Wire.endTransmission();         // termina transmisión 
}

// Lee num bytes empezando del registro address en el dispositivo y los guarda en el arreglo buff  
void readFrom(byte address, int num, byte _buff[]) {
  Wire.beginTransmission(DEVICE); // empieza la transmisión al dispositivo
  Wire.write(address);             // envía direeción a leer 
  Wire.endTransmission();         // termina transmisión 

  Wire.beginTransmission(DEVICE); // empieza la transmisión al dispositivo 
  Wire.requestFrom(DEVICE, num);    // solicita 6 bytes del dispositivo 

  int i = 0;
  while(Wire.available())         // el dispositivo podría mandar menos de lo solicitado (anormal) 
  { 
    _buff[i] = Wire.read();    // recibe un byte 
    i++;
  }
  Wire.endTransmission();         // termina transmisión 
}
