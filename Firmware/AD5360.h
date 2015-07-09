#IFNDEF _AD5360
#DEFINE _AD5360


#define AD5360_CS0 _DAC_BIAS0_CS
#define AD5360_LD0 DAC_BIAS0_LOAD  

#define AD5360_CS1 _DAC_BIAS1_CS
#define AD5360_LD1 DAC_BIAS1_LOAD

#define AD5360_CS2 _DAC_CDS_CS
#define AD5360_LD2 DAC_CDS_LOAD

//#define AD5360_USE_SPI2

#IFDEF AD5360_USE_SPI2
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS0, ENABLE_DELAY=1, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM1)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS1, ENABLE_DELAY=1, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM2)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS2, ENABLE_DELAY=1, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM3)
#ELSE
   #USE SPI(MASTER, FORCE_HW, MODE=1, ENABLE=AD5360_CS0, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM1)
   #USE SPI(MASTER, FORCE_HW, MODE=1, ENABLE=AD5360_CS1, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM2)
   #USE SPI(MASTER, FORCE_HW, MODE=1, ENABLE=AD5360_CS2, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM3)
#ENDIF

//Command comprises of M | AorS plus 16 bits of data

#define M_wrDACdata     0b11000000
#define M_wrDACoffset   0b10000000
#define M_wrDACgain     0b01000000 
#define M_special       0b00000000

typedef enum { all_channels=0,
               all_group0_channels=1,
               all_group1_channels=2,
               channel_00=8,
               channel_01=9,
               channel_02=10,
               channel_03=11,
               channel_04=12,
               channel_05=13,
               channel_06=14,
               channel_07=15,
               channel_10=16,
               channel_11=17,
               channel_12=18,
               channel_13=19,
               channel_14=20,
               channel_15=21,
               channel_16=22,
               channel_17=23} DAC_channel;

// Special function register commands...
#define S_NOP           0b00000000

// Write to control register
#define S_ctrl          0b00000001 // write to control register
#define Sctrl_useRegA   0b00000000 // datawords (OR as needed)
#define Sctrl_useRegB   0b00000100 // use regB for output
#define Sctrl_enTshtdn  0b00000010 // enable over temp shutdown
#define Sctrt_shutdown  0b00000001 // enabale software shutdown

// Write to Offset register
#define S_wrOFS0        0b00000010 // add data word representing the offset
#define S_wr0SS1        0b00000011 // add data word represent offset

// Write to AB Select Register
#define S_wr_ABsel0     0b00000110 // data bits define ab reg switch for each channel
#define S_wr_ABsel1     0b00000111

// Write Block _A/B select registers
#define S_bWr_0toX2A    0b00001011

// Monitor 
#define S_monitor       0b00001100
#define Smon_en         0b0000000000100000 // dataword (OR with channel number)
///////////////////////////////////////////
// Read Register
#define S_rd_Reg        0b00000101 // read reg cmd
//Read data definitions
typedef enum { rd_X1A=  0b0000000000000000, // or this reg if with rd channel 
               rd_X1B=  0b0010000000000000, // or with channel 
               rd_C=    0b0100000000000000, // or with channel 
               r_dM=    0b0110000000000000 }DAC_rd_reg;// or with channel
               
typedef enum { rd_ch_00=0b0000010000000000,
               rd_ch_01=0b0000010010000000,
               rd_ch_02=0b0000010100000000,
               rd_ch_03=0b0000010110000000,
               rd_ch_04=0b0000011000000000,
               rd_ch_05=0b0000011010000000,
               rd_ch_06=0b0000010110000000,
               rd_ch_07=0b0000011110000000,
               rd_ch_10=0b0000100000000000,
               rd_ch_11=0b0000100010000000,
               rd_ch_12=0b0000100100000000,
               rd_ch_13=0b0000100110000000,
               rd_ch_14=0b0000101000000000,
               rd_ch_15=0b0000101010000000,
               rd_ch_16=0b0000101100000000,
               rd_ch_17=0b0000101110000000 }DAC_rd_channel;
               
#define S_rd_Ctrl       0b1000000010000000 // 
#define S_rd_OFSO       0b1000000100000000
#define S_rd_OFS1       0b1000000110000000
#define S_rd_ABsel0     0b1000001100000000
#define S_rd_ABsel1     0b1000001110000000



typedef enum {cb0, cb1, cdsOS}DAC_ID;           

void AD5360_writeData(int8 cmd, int16 data, DAC_ID id) // initalize SPI before sending to DAC
{
   union
   {
      unsigned int32 instruction;
      struct cdat{int16 data; int8  cmd;int8 nothing;}cmddata;;
   }sData;
   
   sData.cmddata.nothing=0;
   sData.cmddata.cmd=cmd;
   sData.cmddata.data=data;
   
  
   switch (id)
   {
      case cb0:
         spi_xfer(AD5360_STREAM1,sData.instruction); // transfers the LSB's 
      break;
      
      case cb1:
         spi_xfer(AD5360_STREAM2,sData.instruction); // transfers the LSB's 
      break;
      
      case cdsOS:
         spi_xfer(AD5360_STREAM3,sData.instruction); // transfers the LSB's 
      break;
  }
}

unsigned int16 AD5360_readReg(int16 data, DAC_ID id) // initalize SPI before sending to DAC
{ 
   AD5360_writeData(S_rd_Reg , data, id); // send command to set up
   
   union
   {
      unsigned int32 response;
      struct cdat{int16 data; int8  chksum;int8 nothing;}cmddata;;
   }sData;
   
   sData.response=0;
   
  
   switch (id)
   {
      case cb0:
         sData.response = spi_xfer(AD5360_STREAM1,sData.response); // transfers the LSB's 
      break;
      
      case cb1:
         sData.response = spi_xfer(AD5360_STREAM2,sData.response); // transfers the LSB's 
      break;
      
      case cdsOS:
         sData.response = spi_xfer(AD5360_STREAM3,sData.response); // transfers the LSB's 
      break;
  }
  
  return sData.cmddata.data;
}

void AD5360_setMUX(unsigned int8 channel, DAC_ID id)
{  
   unsigned int16 data= 0;
   switch (id) // disble the shared MUX channels on bias DAC 0 and 1
   {   
      case cb0:
      case cb1:
         AD5360_writeData(S_Monitor, data, id);
      break;
   }   
   data= sMon_en+channel; // select and enable the desired MUX channel
   AD5360_writeData(S_Monitor, data, id);
}

unsigned int16 AD5360_calcDACValue(float value, float scaler)
{
   value *= scaler;
   if (value > 32767) 
   {
      value= 65535;
   }
   else if (value < -32768)
   {
      value=0;
   }
   else
   {
      value += 32768;
   }
   return ((unsigned int16) value);
}
         
#ENDIF
