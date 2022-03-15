#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <BH1750.h>
#include<SoftwareSerial.h>
SoftwareSerial s(5,6);//Rx Tx /Arduino code
#define I2C_address 0x23
BH1750 lightMeter;

String temperatureString;
String humidityString;
String pressureString;
String altitudeString;
String uvindexString;
String lightintensityString;
String sensorData;
float const ALTITUDE = 4.0;              // Altitude at my location in meters
float const SEA_LEVEL_PRESSURE = 1013.25;  // Pressure at sea level
Adafruit_BME280 bme; // I2C
//===============================================================================
//  Initialization
//===============================================================================
void setup(void) {

  s.begin(9600);
  Wire.begin();
  Serial.begin(9600);
  delay(1000);

  lightMeter.begin();
  Serial.println(F("BH1750 Test"));
  bool status;
  // default settings
  status = bme.begin(0x76);  // The I2C address of the sensor is 0x76
  if (!status) {             // Loop if sensor not found
    Serial.print("GY-BMP280 Sensor Error");
    while (1);
  }
}


//===============================================================================
//  Main
//===============================================================================
void loop() {

  
  sensorData="";
  getTemperature();  // Get sensor data
  getHumidity();
  getPressure();
  //getAltitude();
  getUVIndex();
  getLightIntensity();
  
  s.print("D");
  s.print(sensorData);
  s.print("*");
  delay(15000);     // Update readings every 15 seconds
}

void getLightIntensity()
{
  float lux = lightMeter.readLightLevel();
  lightintensityString=String(lux);
  Serial.print("  Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  sensorData+="&field5="+lightintensityString;
  
}
//===============================================================================
//  getTemperature - Subroutine to get and print temperature
//===============================================================================
void getTemperature()
{
  float temperature = bme.readTemperature();
  //temperature = temperature * 9 / 5 + 32; // Convert C to F
  temperatureString = String(temperature, 1); // One decimal position
  Serial.print("Temperature: ");
  Serial.print(temperatureString);
  sensorData="&field1="+temperatureString;
}

//===============================================================================
//  getHumidity - Subroutine to get and print humidity
//===============================================================================
void getHumidity()
{
  float humidity = bme.readHumidity();
  humidityString = String(humidity, 0);
  Serial.print(" Humidity: ");
  Serial.print(humidityString);
  sensorData+="&field2="+humidityString;
  
}
//===============================================================================
//  getPressure - Subroutine to get and print pressure
//===============================================================================
void getPressure()
{
  float pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE, pressure);
  pressure = pressure / 3386.39;    // Convert hPa to in/Hg
  pressureString = String(pressure, 2);
  Serial.print(" Pressure: ");
  Serial.print(pressureString);
  sensorData+="&field3="+pressureString;
  
}
//===============================================================================
//  getAltitude - Subroutine to get and print temperature
//===============================================================================
void getAltitude()
{
  float altitude = bme.readAltitude(SEA_LEVEL_PRESSURE);
  altitude = altitude * 3.28084;  // Convert meters to feet
  altitudeString = String(altitude, 0);
  Serial.print(" Altitude: ");
  Serial.print(altitudeString);
  
}

void getUVIndex()
{
  float sensorValue;
  sensorValue = analogRead(A0);
  float uvindex=(sensorValue-50)/93;
  uvindexString=String(uvindex, 1);
  Serial.print("  UV-Index : ");
  Serial.print(uvindex);
  sensorData+="&field4="+uvindexString;
  
}
