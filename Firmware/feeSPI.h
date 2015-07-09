#IFNDEF _SPI_comms
#DEFINE _SPI_comms
// This file includes all the SPI functions for the FEE
#include <ADS1247.h> // temperature sensor ADAC
#include <AD7689.h>
#include <AD5360.h>
#include <math.h>

//#include <ADxxxx.h> // 16 bit DAC's
//#include <ADyyyy.h> //16 bit ADC converter


//#use spi(MASTER, FORCE_HW, SPI1, stream=SPI_STREAM)

// CCD TEMPERATURE conversion to kelvin from counts (24bit full scale)
// k = 2E-13x^2 + 3E-05x + 143.31   (R^2=1)
#define CCD_a0 143.31
#define CCD_a1 3e-05F
#define CCD_a2 2e-13F

/*****************************************************************************/
/* HELPER ROUTINES ***********************************************************/
/*****************************************************************************/
 
 float poly(float x, float a0, float a1, float a2, float a3=0, float a4=0)
 {
   return((a4*pow(x,4))+(a3*pow(x,3))+(a2*pow(x,2))+(a1*x)+a0);
 }

/*****************************************************************************/
/* COMMON ROUTINES ***********************************************************/
/*****************************************************************************/

void deselectAllSPI()
{
   output_low(ADC_TEMP_START);
   output_high(_ADC_TEMP_CS);
   output_high(_DAC_RST);
   output_high(_DAC_BIAS0_CS);
   output_low(DAC_BIAS0_LOAD);
   output_high(_DAC_BIAS1_CS);
   output_low(DAC_BIAS1_LOAD);
   output_low(DAC_CDS_LOAD);
   output_high(_DAC_CDS_CS);
   output_high(ADC_BIAS_CNV);
}

/*****************************************************************************/
/* CCD TEMPERATURE SENSOR ROUTINES *******************************************/
/*****************************************************************************/

typedef enum  {ch0=0,
               ch1=1}T_source;


float get_CCDtemperature(T_source ch)
{
   deselectAllSPI();
   ADS1247_setupSPI();// configure SPI parameters for temp sensor
   output_high(ADC_TEMP_START); //wakes the device
   ADS1247_reset(); //set default settings
   delay_ms(100);
   ADS1247_stopStreaming(); // stop continuous reads (default mode)
   delay_ms(100);
   int8  data = 0;
   
   if (ch==0) 
   {
      data=(MUX0_SP_an0 | MUX0_SN_an1);
   }
   else 
   {
      data=(MUX0_SP_an2 | MUX0_SN_an3);
   }
   ADS1247_writeReg(data, ra_MUX0); // select channel to measure
   
   data=0;
   ADS1247_writeReg(data, ra_VBIAS); // disable VBIAS
   
   data=(MUX1_REFCON_ON | MUX1_REFSEL_REF0 | MUX1_CAL_NORM);
   ADS1247_writeReg(data, ra_MUX1); // use external reference
   
   data=(SYS0_PGAgain_1 | SYS0_OPdataRate_5sps);
   ADS1247_writeReg(data, ra_SYS0); // set PGA gain and ADC samplerate
   
   data=(IDAC0_D_Out | IDAC0_MAG_500uA); // set up current source 
   ADS1247_writeReg(data, ra_IDAC0);
   
   data=ADS1247_readReg(ra_IDAC0);
    
   if (ch==0) 
   {
      data=(IDAC1_I1DIR_an0 | IDAC1_I2DIR_an1);
   }
   else 
   {
      data=(IDAC1_I1DIR_an2 | IDAC1_I2DIR_an3);
   }
   ADS1247_writeReg(data,ra_IDAC1); // select channels to excite
   
   data=ADS1247_readReg(ra_IDAC1);
   
   output_low(ADC_TEMP_START);
   delay_us(10);
   output_high(ADC_TEMP_START);   // start conversion
   
   delay_ms(250); // allow time for the conversion to complete
   
   float x = (float)ADS1247_readData(); // counts in float form
   
   data=(IDAC0_D_Out | IDAC0_MAG_OFF); // disable current source 
   ADS1247_writeReg(data, ra_IDAC0);
   output_low(ADC_TEMP_START); // put device to sleep   
   
   return (poly(x, CCD_a0, CCD_a1, CCD_a2)); // result in Kelvin
}

/*****************************************************************************/
/* BIAS ADC ROUTINES *********************************************************/
/*****************************************************************************/

void initialize7869() // initialize with a dymmy read
{
   deselectAllSPI();
   unsigned int16 data;
   data= (CFG_overwrite | INCC_uniRefGND  | BW_quarter | REF_int2V5 | SEQ_disabled | RB_readDataOnly);
   AD7689_readData(data);
   return;
}

typedef enum  {vch0=INx_ch0,
               vch1=INx_ch1,
               vch2=INx_ch2,
               vch3=INx_ch3,
               vch4=INx_ch4,
               vch5=INx_ch5,
               vch6=INx_ch6,
               vch7=INx_ch7}V_source;

float get7689Voltage(int16 channel, float scaler, float offset)
{
   channel*=INx_ch1; // this coverts a int (0  to 7) to real channel ID
   deselectAllSPI();
   unsigned int16 data;
   data= (channel | CFG_overwrite | INCC_uniRefGND  | BW_quarter | REF_int2V5 | SEQ_disabled | RB_readDataOnly);
   
   float result = (float) AD7689_readData(data);
   result *=scaler;  // scale counts to volts
   result-=offset;   // subtract offset
   return result;
}

/*****************************************************************************/
/* BIAS DAC ROUTINES *********************************************************/
/*****************************************************************************/
void setDACmux(DAC_ID id, Int16 MUXChannel)
{
   deselectAllSPI();
   int8 cmd = S_monitor;
   if(id==cb0)
   {
      AD5360_writeData(cmd, 0, cb1); //disable MUX
   }
   else if(id==cb1)
   {
      AD5360_writeData(cmd, 0, cb0); //disable MUX
   }
   deselectAllSPI();
   int16 data = SMon_en | MUXChannel;
   AD5360_writeData(cmd, data, id);//select MUX channel
}

void setVoltage(int16 value, DAC_ID id, DAC_channel channel)
{
   deselectAllSPI();
   int8 cmd = M_wrDACdata;
   cmd+=channel;
   AD5360_writeData(cmd, value, id);
   delay_us(100);
   switch (id) // transfer to output
   {
      case cb0:
         delay_us(100);
         output_low(DAC_BIAS0_LOAD);
         delay_us(100);
         output_high(DAC_BIAS0_LOAD);
      break;
      
      case cb1:
         delay_us(100);
         output_low(DAC_BIAS1_LOAD);
         delay_us(100);
         output_high(DAC_BIAS1_LOAD);     
      break;
      
      case cdsOS:
         delay_us(100);
         output_low(DAC_CDS_LOAD);
         delay_us(100);
         output_high(DAC_CDS_LOAD);      
      break;
   
   }
}




#ENDIF
