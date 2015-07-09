#include <ADS1247.h> // 
#include <feeTempSensor.h>
#include <feemain.h>

#define TS_ST_CONV_PIN   ADC_TEMP_START  // defines start conversion pin
#define TS_CS_PIN        _ADC_TEMP_CS  


// Default converter settings
#define rMUX0setting MUX0_BCS_off||MUX0_SP_an0||MUX0_SN_an1
#define rMUX1setting MUX1_REFCON_AUTO||MUX1_REFSEL_REF0
#define rIDAC0setting IDAC0_MAG_500uA
#define rIDAC1setting IDAC1_I1DIR_an0||IDAC1_I2DIR_an1

void iniTempSensor()
{
   SPI_Select_TempADC;
   ADS1247_reset();
   ADS1247_writeReg(rMUX0setting,ra_MUX0);
   ADS1247_writeReg(rMUX1setting,ra_MUX1);
   ADS1247_writeReg(rIDAC0setting,ra_IDAC0); 
   ADS1247_writeReg(rIDAC1setting,ra_IDAC1);
}

void selectTempSensor(int8 sensorID)
{   
   SPI_Select_TempADC;
   if(sensorID, 0)
   {
      ADS1247_writeReg(MUX0_BCS_off||MUX0_SP_an0||MUX0_SN_an1,ra_MUX0); // choose sensor
      ADS1247_writeReg(iDAC1_I1DIR_an0||iDAC1_I2DIR_an1,ra_IDAC1); // set current pins
   }
   else
   {
      ADS1247_writeReg(MUX0_BCS_off||MUX0_SP_an2||MUX0_SN_an3,ra_MUX0); // choose sensor
      ADS1247_writeReg(iDAC1_I1DIR_an2||iDAC1_I2DIR_an2,ra_IDAC1);
   }
}

void startTempConversion()
{
   output_high(TS_ST_CONV_PIN); 
   output_low(TS_ST_CONV_PIN);
}

float rawToEngineering (int32 rawCount)
{
   return ((float)rawcount);
}

float getTempValue()
{
   int32 rawData = ADS1247_readData();
   float result = rawToEngineering(rawData);
   return (result);
   
}


