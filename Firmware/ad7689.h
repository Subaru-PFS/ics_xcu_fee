#IFNDEF _AD7689
#DEFINE _AD7689

#define AD7689_CNV ADC_BIAS_CNV // convert pin
//#define AD7689_USE_SPI2

#IFDEF AD7689_USE_SPI2
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=100000, ENABLE=AD7689_CNV, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=16, stream=AD7689_STREAM)
#ELSE   
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=100000, ENABLE=AD7689_CNV,CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=16, stream=AD7689_STREAM)
#ENDIF
// CONFIGURATION REGISTOR = INCC | INX | BW | REF .... etc.
// Note: 2 dummy conversions are required to update the configuration
// convert has to be low to transfer data..
#define CFG_noOverwrite    0b0000000000000000
#define CFG_overwrite      0b1000000000000000

#define INCC_biDiffPairs   0b0000000000000000
#define INCC_biRefCOM      0b0010000000000000
#define INCC_tempSense     0b0011000000000000
#define INCC_uniDiffPairs  0b0100000000000000
#define INCC_uniRefCOM     0b0110000000000000
#define INCC_uniRefGND     0b0111000000000000

#define INx_ch0            0b0000000000000000
#define INx_ch1            0b0000001000000000
#define INx_ch2            0b0000010000000000
#define INx_ch3            0b0000011000000000
#define INx_ch4            0b0000100000000000
#define INx_ch5            0b0000101000000000
#define INx_ch6            0b0000110000000000
#define INx_ch7            0b0000111000000000

#define BW_full            0b0000000100000000
#define BW_quarter         0b0000000000000000

#define REF_int2V5         0b0000000000000000
#define REF_int4V096       0b0000000000100000
#define REF_ext_tempEn     0b0000000001000000
#define REF_extBuff_tempEn 0b0000000001100000
#define REF_ext            0b0000000011000000
#define REF_extBuff        0b0000000011100000

#define SEQ_disabled       0b0000000000000000
#define SEQ_upGfgDurSeq    0b0000000000001000
#define SEQ_enPlusTemp     0b0000000000010000
#define SEQ_enabled        0b0000000000011000

#define RB_readCfgWdata    0b0000000000000000
#define RB_readDataOnly    0b0000000000000100

void AD7689_setupSPI()
{
      spi_init(AD7689_STREAM);
}

unsigned int16 AD7689_readData(int16 config)
{
   union
   {
      unsigned int16 response;
      signed int16 result;
   }rData;

   spi_xfer(AD7689_STREAM,config);
   delay_us(15);
   spi_xfer(AD7689_STREAM,config);
   delay_us(15);
   spi_xfer(AD7689_STREAM,config);
   delay_us(15);
   rData.response=spi_xfer(AD7689_STREAM,config);
   return rData.response;
}



#ENDIF
