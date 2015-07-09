#IFNDEF _AD7689
#DEFINE _AD7689

#define AD7689_CNV ADC_BIAS_CNV // convert pin
//#define AD7689_USE_SPI2

#IFDEF AD7689_USE_SPI2
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD7689_CNV, CLK=PIN_D6, DI=PIN_D5, DO=PIN_D4, SPI2, BITS=16, stream=AD7689_STREAM)
#ELSE   
   #USE SPI(MASTER, FORCE_HW, MODE=1, BAUD=1000000, ENABLE=AD7689_CNV, CLK=PIN_C3, DI=PIN_C4, DO=PIN_C5, SPI1, BITS=16, stream=AD7689_STREAM)
#ENDIF
// CONFIGURATION REGISTOR = INCC | INX | BW | REF .... etc.
// Note: 2 dummy conversions are required to update the configuration
// convert has to be low to transfer data..
#define CFG_noOverwrite    0
#define CFG_overwrite      8192

#define INCC_biDiffPairs   0
#define INCC_biRefCOM      2048
#define INCC_tempSense     3072
#define INCC_uniDiffPairs  4096
#define INCC_uniRefCOM     6144
#define INCC_uniRefGND     7168

#define INx_ch0            0
#define INx_ch1            128
#define INx_ch2            256
#define INx_ch3            384
#define INx_ch4            512
#define INx_ch5            640
#define INx_ch6            768
#define INx_ch7            896

#define BW_full            0
#define BW_quarter         64

#define REF_int2V5         0
#define REF_int4V096       8
#define REF_ext_tempEn     16
#define REF_extBuff_tempEn 24
#define REF_ext            48
#define REF_extBuff        56

#define SEQ_disabled       0
#define SEQ_upGfgDurSeq    2
#define SEQ_enPlusTemp     4
#define SEQ_enabled        6

#define RB_readCfgWdata    0
#define RB_readDataOnly    1

#ENDIF

void AD7689_setupSPI()
{
      spi_init(AD7689_STREAM);
}

signed int16 AD7689_readData(int16 config)
{
   union
   {
      unsigned int16 response;
      signed int16 result;
   }rData;
   
   spi_xfer(AD7689_STREAM,config);
   delay_us(10);
   spi_xfer(AD7689_STREAM,0);
   delay_us(10);
   rData.response=spi_xfer(AD7689_STREAM,0);
   return rData.result;
}
