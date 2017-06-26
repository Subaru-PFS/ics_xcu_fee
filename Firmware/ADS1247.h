#IFNDEF _ADS1247
#DEFINE _ADS1247

//#DEFINE ADS1247_USE_SPI2 // comment out to use SPI1
#define ads1247_CS _ADC_TEMP_CS
// Routines to control the temperature sensor
// uses 4 channels of ADS1247 Temeperature Sensor IC
//#USE SPI (MASTER, CLK=PIN_E1, DO=PIN_E0, ENABLE=PIN_E2, CLOCK_HIGH=1, CLOCK_LOW=1, MODE=0, BITS=16, ENABLE_ACTIVE=0, STREAM=FCLK, MSB_FIRST)
#IFDEF ADS1247_USE_SPI2 
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=8, stream=ADS1247_STREAM)
#ELSE   
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=8, stream=ADS1247_STREAM)
#ENDIF
/*****************************************************************************/
/* COMMANDS ******************************************************************/
/*****************************************************************************/

#define cmd_wakeup      0x00  //Wakeup
#define cmd_sleep       0x02  //Sleep
#define cmd_sync        0x04  //Synchronize
#define cmd_reset       0x06  //Reset to default settings
#define cmd_nop         0XFF  //No Operation (NOP)

#define cmd_rData       0x12  //Read Data
#define cmd_sData_start 0x14  //Start streaming Data
#define cmd_sData_stop  0x16  //Stop streaming Data

#define cmd_rReg        0x20  //Read Register
#define cmd_wReg        0x40  //Write Register

#define cmd_calOS       0x60  //Calibrate offset
#define cmd_calGain     0x61  //Calibrate gain
#define cmd_calSelfOS   0x62  //Calibrate Self Offset


/*****************************************************************************/
/* REGISTER ADDRESSES ********************************************************/
/*****************************************************************************/


typedef enum
{
   ra_MUX0=0x00,
   ra_VBIAS=0x01,
   ra_MUX1=0x02,
   ra_SYS0=0x03,
   ra_OFC0=0x04,
   ra_OFC1=0x05,
   ra_OFC2=0x06,
   ra_FSC0=0x07,
   ra_FSC1=0x08,
   ra_FXC2=0x09,
   ra_IDAC0=0x0A,  
   ra_IDAC1=0x0B,
   ra_GPIOCFG=0x0C, // GPIO
   ra_GPIODIR=0X0D,
   ra_GPIODAT=0x0E
}rAddress;

/*****************************************************************************/
/* REGISTER VALUES ***********************************************************/
/*****************************************************************************/

// MUX0 = BCS+SP+SN... OR values together
typedef enum // burnout current source
{
   MUX0_BCS_off=  0b00000000,
   MUX0_BCS_500pA=0b01000000,
   MUX0_BCS_2uA=  0b10000000,
   MUX0_BCS_10uA= 0b11000000  
}MUX0_BCS;

typedef enum // Positive channel Select
{
   MUX0_SP_an0=   0b00000000,
   MUX0_SP_an1=   0b00001000,
   MUX0_SP_an2=   0b00010000,
   MUX0_SP_an3=   0b00011000,
   MUX0_SP_an4=   0b00100000,
   MUX0_SP_an5=   0b00101000,
   MUX0_SP_an6=   0b00110000,
   MUX0_SP_an7=   0b00111000
}MUX0_SP;

typedef enum // Negative channel Select
{
   MUX0_SN_an0=   0b00000000,
   MUX0_SN_an1=   0b00000001,
   MUX0_SN_an2=   0b00000010,
   MUX0_SN_an3=   0b00000011,
   MUX0_SN_an4=   0b00000100,
   MUX0_SN_an5=   0b00000101,
   MUX0_SN_an6=   0b00000110,
   MUX0_SN_an7=   0b00000111
}MUX0_SN;
//

typedef enum // bias voltage enables
{
   VBIAS_0_en=0x01,
   VBIAS_1_en=0x02,
   VBIAS_2_en=0x04,
   VBIAS_3_en=0x08
}VBIAS;

// MUX1 = REFCON+RESEL+CAL... OR values together
typedef enum // voltage reference control
{
   MUX1_REFCON_OFF=  0b00000000,
   MUX1_REFCON_ON=   0b00100000,
   MUX1_REFCON_AUTO= 0b01000000
}MUX1_REFCON;

typedef enum // voltage reference select
{
   MUX1_REFSEL_REF0=       0b00000000,
   MUX1_REFSEL_INT=        0b00010000,
   MUX1_REFSEL_INTtoREF0=  0b00011000
}MUX1_REFSEL;

typedef enum // calibration MUX
{
   MUX1_CAL_NORM=    0b00000000,
   MUX1_CAL_OS=      0b00000001,
   MUX1_CAL_GAIN=    0b00000010,
   MUX1_CAL_TEMP=    0b00000011,
   MUX1_CAL_REF0=    0b00000101,
   MUX1_CAL_AVDD=    0b00000110,
   MUX1_CAL_DVDD=    0b00000111   
}MUX1_CAL;
//

// SYS0 = PGAGain+OPdataRate... OR values together
typedef enum // PGA gains
{
   SYS0_PGAgain_1=   0b00000000,
   SYS0_PGAgain_g2=  0b00010000,
   SYS0_PGAgain_g4=  0b00100000,
   SYS0_PGAgain_g8=  0b00110000,
   SYS0_PGAgain_g16= 0b01000000,
   SYS0_PGAgain_g32= 0b01010000,
   SYS0_PGAgain_g64= 0b01100000,
   SYS0_PGAgain_g128=0b01110000
} SYS0_PGAgain;

typedef enum // output data Rate
{
   SYS0_OPdataRate_5sps=      0b00000000,
   SYS0_OPdataRate_10sps=     0b00000001,
   SYS0_OPdataRate_20sps=     0b00000010,
   SYS0_OPdataRate_40sps=     0b00000011,
   SYS0_OPdataRate_80sps=     0b00000100,
   SYS0_OPdataRate_1600sps=   0b00000101,
   SYS0_OPdataRate_320sps=    0b00000110,
   SYS0_OPdataRate_640sps=    0b00000111,
   SYS0_OPdataRate_1000sps=   0b00001000,
   SYS0_OPdataRate_2000sps=   0b00001001
} SYS0_OPdataRate;
//

typedef enum // IDAC control register
{
   IDAC0_D_Out=   0b00000000,
   IDAC0_D_Rdy=   0b00001000
}IDAC0_DRDY;

typedef enum //
{
   IDAC0_MAG_OFF=    0b00000000,
   IDAC0_MAG_50uA=   0b00000001,
   IDAC0_MAG_100uA=  0b00000010,
   IDAC0_MAG_250uA=  0b00000011,
   IDAC0_MAG_500uA=  0b00000100,
   IDAC0_MAG_750uA=  0b00000101,
   IDAC0_MAG_1000uA= 0b00000110,
   IDAC0_MAG_1500uA= 0b00000111
}IDAC0_MAG;

typedef enum DAC//
{
   IDAC1_I1DIR_an0=  0b00000000,
   IDAC1_I1DIR_an1=  0b00010000,
   IDAC1_I1DIR_an2=  0b00100000,
   IDAC1_I1DIR_an3=  0b00110000,
   IDAC1_I1DIR_an4=  0b01000000,
   IDAC1_I1DIR_an5=  0b01010000,
   IDAC1_I1DIR_an6=  0b01100000,
   IDAC1_I1DIR_an7=  0b01110000,
   IDAC1_I1_discon=  0b11110000
}IDAC1_I1DIR;

typedef enum //
{
   iDAC1_I2DIR_an0=  0b00000000,
   iDAC1_I2DIR_an1=  0b00000001,
   iDAC1_I2DIR_an2=  0b00000010,
   iDAC1_I2DIR_an3=  0b00000011,
   iDAC1_I2DIR_an4=  0b00000100,
   iDAC1_I2DIR_an5=  0b00000101,
   iDAC1_I2DIR_an6=  0b00000110,
   iDAC1_I2DIR_an7=  0b00000111,
   iDAC1_I2_discon=  0b00001111
}iDAC1_I2DIR;

/*****************************************************************************/
/* BASIC FUNCTIONS ***********************************************************/
/*****************************************************************************/
void ADS1247_setupSPI()
{
   
 //  #IFDEF ADS1247_USE_SPI2
      spi_init(ADS1247_STREAM);
 //  #ELSE
 //     spi_init(ADS1247_SP1_STREAM,100000);
 //  #ENDIF
}

void ADS1247_wakeup()  // enter operating state
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_wakeup);
   #ELSE
   spi_write(cmd_wakeup);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

void ADS1247_sleep() // enter sleep state
{  
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_sleep);
   #ELSE
   spi_write(cmd_sleep);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

void ADS1247_sync() // synchronize
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_sync);
   spi_write2(cmd_sync);
   #ELSE
   spi_write(cmd_sync);
   spi_write(cmd_sync);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

void ADS1247_reset() // reset device (default settings)
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_reset);
   #ELSE
   spi_write(cmd_reset);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

void ADS1247_startStreaming() // reset device (default settings)
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_sData_start);
   #ELSE
   spi_write(cmd_sData_start);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

void ADS1247_stopStreaming() // reset device (default settings)
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_sData_stop);
   #ELSE
   spi_write(cmd_sData_stop);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}


int32 ADS1247_readData() // read conversion results (should check if done)
{
   output_low(ads1247_CS);
   delay_us(10);
   
   
   union // used to convert (4x) bytes to 32bit result
   {
      char  data[4];
      signed int32 result;
   }rData;
   
   
   #IFDEF ADS1247_USE_SPI2
   spi_write2(cmd_rData);                // Send command to read data
   rData.data[2] = spi_read2(cmd_nop); //MSB
   rData.data[3] = rData.data[2] & 0b10000000;
   rData.data[2] = rdata.data[2] & 0b01111111;
   rData.data[1] = spi_read2(cmd_nop); 
   rData.data[0] = spi_read2(cmd_nop); //LSB
   #ELSE
   spi_write(cmd_rData);                // Send command to read data
   rData.data[2] = spi_read(cmd_nop); //MSB
   rData.data[3] = rData.data[2] & 0b10000000;
   rData.data[2] = rdata.data[2] & 0b01111111;
   rData.data[1] = spi_read(cmd_nop); 
   rData.data[0] = spi_read(cmd_nop); //LSB
   #ENDIF   
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
   return(rData.result);
   
}

void ADS1247_writeReg(int8 data, rAddress Reg)
{
   output_low(ads1247_CS);
   delay_us(10);
   #IFDEF ADS1247_USE_SPI2
   spi_write2((int8)Reg+cmd_wReg);   // Send write command with address
   spi_write2(0x00);                 // Send number of fields to write-1
   spi_write2(data);
   #ELSE
   spi_write((int8)Reg+cmd_wReg);   // Send write command with address
   spi_write(0x00);                 // Send number of fields to write-1
   spi_write(data);
   #ENDIF
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
}

int8 ADS1247_readReg(rAddress Reg)
{
   output_low(ads1247_CS);
   delay_us(10);
   #IF AD1247_USE_SPI2
   spi_write2((int8)Reg+cmd_rReg);  // Send read command + address
   spi_write2(0x00);                 // Send number of fields to read-1
   #ELSE
   spi_write((int8)Reg+cmd_rReg);  // Send read command + address
   spi_write(0x00);                 // Send number of fields to read-1
   #ENDIF
   int8 result=spi_read(cmd_nop);
   delay_us(10);
   output_high(ads1247_CS);
   delay_us(500);
   return result; // Return read data
}

#ENDIF
