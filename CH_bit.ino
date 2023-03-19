#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68
#define startRegister 0x00
#define endRegister 0x07

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.print("Register");
  Serial.print("\t");
  Serial.println("Bit Values");
  Serial.println();

  writeNVRAM(0x00, B00000000);//enable oscillator and set seconds to 0
  //writeNVRAM(0x00, B10000000);//disable oscillator set CH bit

  for (int a = startRegister; a <= endRegister; a++)
  {
    byte b = readNVRAM(a);
    Serial.print("0X");
    if (a < 16)
      Serial.print("0");
    Serial.print(a, HEX);
    Serial.print("\t");
    Serial.print("\t");


    for (int i = 7; i >= 0; i-- )
    {
      Serial.print((b >> i) & 0X01);//shift and select first bit
    }
    Serial.println();
  }
}

void writeNVRAM(byte location, byte data)
// writes data to DS1307 NVRAM location
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(location);
  Wire.write(data);
  Wire.endTransmission();
}

byte readNVRAM(byte location)//// reads data from DS1307 NVRAM location
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(location);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  Wire.read();
}

void loop() {
}
