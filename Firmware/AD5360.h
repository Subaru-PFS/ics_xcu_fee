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
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS0, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM1)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS1, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM2)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD5360_CS2, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=24, stream=AD5360_STREAM3)
#ELSE
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=10000, ENABLE=AD5360_CS0, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM1)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=10000, ENABLE=AD5360_CS1, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM2)
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=10000, ENABLE=AD5360_CS2, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=24, stream=AD5360_STREAM3)
#ENDIF

//Commnd comprises of M | AorS plus 16 bits of data

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
               
#define S_NOP           0b00000000
#define S_ctrl          0b00000001
   #define Sctrl_useRegA  0b00000000 // datawords (OR as needed)
   #define Sctrl_useRegB  0b00000100
   #define Sctrl_enTshtdn 0b00000010
   #define Sctrt_shutdown 0b00000001
#define S_wrOFS0        0b00000010 // add data word representing the offset
#define S_wr0SS1        0b00000011
#define S_wr_ABsel0     0b00000110 // data bits define ab reg switch for each channel
#define S_wr_ABsel1     0b00000111
#define S_bWr_0toX2A    0b00001011
#define S_monitor       0b00001100
   #define Smon_en        0b00100000 // dataword (OR with channel number)


//!
//!typedef enum{dac0, dac1, dac2}DAC_ID;
//!void AD5360_setupSPI(DAC_ID id)
//!{
//!   switch(id)
//!   {
//!      case dac0:
//!         spi_init(AD5360_STREAM1);
//!      break;
//!      
//!      case dac1:
//!         spi_init(AD5360_STREAM2);
//!      break;
//!      
//!      case dac2:
//!         spi_init(AD5360_STREAM3);
//!      break;     
//!   }
//!}

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
         
#ENDIF
