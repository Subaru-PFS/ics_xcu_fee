#IFNDEF _fee_cmd
#DEFINE _fee_cmd

#include <feeCmdHandler.h>

#include <feeADC.h>
#include <feeSPI.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


#include <feeCmdHandlerHelper.h>

/*****************************************************************************/
/* MENU AND SUBMENU LISTS USED BY COMMAND HANDLERS                           */
/*****************************************************************************/
enum mainMenu {sCB,gCB,rCB,sOS,gOS,rOS,lDAC,sDAC,sPower,gPower,sFast,gSupply,cSupply,gTemp,gRevision,gSerial,sSerial,calADC};
char rom mainMenuItems[][8]=                   {setClockBias,
                                                getClockBias,
                                                rdClockBias,
                                                setCDS_OS,
                                                getCDS_OS,
                                                rdCDS_0S,
                                                loadDACPresets,
                                                saveDACPresets,
                                                setPowerEn,
                                                getPowerEn,
                                                setFast,
                                                getSupplyVoltage,
                                                calSupplyVoltage,
                                                getTemperature,
                                                getRevision,
                                                getSerialNumber,
                                                setSerialNumber,
                                                cal7689,
                                                "\0"};
                                      
enum biasSubMenu {Pp,Pn,DGp,DGn,Sp,Sn,SWp,SWn,RGp,RGn,OG,RD,OD,BB,bias_all=255};                   
char rom biasSubMenuItems[][8]=                {cb_Ppos,
                                                cb_Pneg,
                                                cb_DGpos,
                                                cb_DGneg,
                                                cb_Spos,
                                                cb_Sneg,
                                                cb_SWpos,
                                                cb_SWneg,
                                                cb_RGpos,
                                                cb_RGneg,
                                                cb_OG,
                                                cb_RD,
                                                cb_OD,
                                                cb_BB,
                                                cb_all
                                                "\0"};

enum biasSubSubMenu {bias0,bias1};                   
char rom biasSubSubMenuItems[][8]=             {cb_0,
                                                cb_1,
                                                "\0"};
                                              
enum offsetSubMenu {pO,n0,p1,n1,p2,n2,p3};
char rom offsetSubMenuItems[][8]=              {co_0pos,
                                                co_0neg,
                                                co_1pos,
                                                co_1neg,
                                                co_2pos,
                                                co_2neg,
                                                co_3pos,
                                                co_3neg,
                                                "\0"};
         
enum offsetSubSubMenu {os0,os1};                   
char rom offsetSubSubMenuItems[][8]=          { co_0,
                                                co_1,
                                                "\0"};                                         

enum presetSubMenu {pErase,pRead,pExpose,pWipe,pBT1,pOffset,pOT1};
char rom presetSubMenuItems[][8]=             { pb_erase,                                                
                                                pb_read,
                                                pb_expose,
                                                pb_wipe,
                                                pb_biasTest1,
                                                pb_offset,
                                                pb_osTest1,
                                                "\0"};
                                                                                              
                                              
enum powerSubMenu {p33,p5,p12,p24,p54,pPA,pLVDS,pVbb0,pVbb1,pClks,pAll};
char rom powerSubMenuItems[][8]=               {pe_3V3reg,
                                                pe_5Vreg,
                                                pe_12Vreg,
                                                pe_24Vreg,
                                                pe_54Vreg,
                                                pe_Preamp,
                                                pe_LVDS,
                                                pe_Vbb0,
                                                pe_Vbb1,
                                                pe_Clks,
                                                pe_all,
                                                "\0"}; 
                                        
enum powerSubSubMenu {pOn,pOff};
char rom powerSubSubMenuItems[][8]=            {pe_on,
                                                pe_off,
                                                "\0"}; 
                                            
enum fastSubMenu {fFast,fSlow};                                            
char rom fastSubMenuItems[][8]=                {sf_fast,     
                                                sf_slow,
                                                "\0"}; 
 
enum voltageSubMenu {v3V3M,v3V3o,v5Vp,v5Vn,vPAp,vPAn,v12Vp,v12Vn,v24Vn,v54Vp};
char rom voltageSubMenuItems[][8]=             {gv_3V3Micro,       
                                                gv_3V3Other,
                                                gv_5Vpos,        
                                                gv_5Vneg,        
                                                gv_5Vpos_pa,     
                                                gv_5Vneg_pa,     
                                                gv_12Vpos,      
                                                gv_12Vneg,      
                                                gv_24Vneg,      
                                                gv_54Vpos,
                                                "\0"}; 
 
enum tempSubMenu {tCCD0,tCCD1,tPA,tFEE};
char rom tempSubMenuItems[][8]=                {gt_CCD0,
                                                gt_CCD1,     
                                                gt_Preamp,
                                                gt_FEE,
                                                "\0"}; 

enum cal7SubMenu {cBias,cCDS};
char rom cal7689SubMenuItems[][8]=             {c7_bias,
                                                c7_CDS,
                                                "\0"}; 
                                                
enum snSubMenu {snFEE,snADC,snPA0,snPA1};
char rom snSubMenuItems[][8]=                   {sn_FEE,
                                                sn_ADC,
                                                sn_PA0,
                                                sn_PA1,
                                                "\0"};                                                
/*****************************************************************************/
/* CLOCK BIAS COMMAND HANDLERS                                               */
/*****************************************************************************/
void exeSetClockBias()
{
   int8 chID;
   unsigned int16 value;
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<16)
      {
         int chID = biasParams[chIndex].channel;
         float voltage = atof(rxBuff[pCmd].Param[3]);
         if(voltage <= biasParams[chIndex].max && voltage >= biasParams[chIndex].min)
         {
            biasDef[chIndex+(dacID*16)]=voltage;
            float scaler = biasParams[chIndex].scaler;
            voltage *=scaler;
            value =32768 + (signed int16)voltage;
            setVoltage(value, dacID, chID);
            printf(success);
         }
         else
         {
            printf(outOfRange);
         }
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }
}

void exeGetClockBias()
{
   //int8 chID;
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<16)
      {
         printf("%f\n",biasDef[chIndex+(dacID*16)]);
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }
}
void exeRdClockBias()
{
   int8 dacID = getMenuIndex(rxBuff[pCmd].Param[2],biasSubSubMenuItems);
   if (dacID <=1)
   {
      int8 chIndex=getBiasRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<16)
      {
         int ADCchID = biasParams[chindex].ADCCh0;
         if (dacID==1) ADCchID = biasParams[chindex].ADCCh1;
         float offset = get7689Offset(ADCChID);
         float scaler = get7689Scaler(ADCChID);
         
         // first configure the DAC MUX
         setDACmux(dacID, (int16)chIndex);
         // then read the ADC voltage
         float result = get7689Voltage(ADCchID, scaler, offset);
         printf("%f\n",result);
      }
      else if (chIndex==bias_all)
      {
         for(int i=0; i<14; i++)
         {
            int ADCchID = biasParams[i].ADCCh0;
            if (dacID==1) ADCchID = biasParams[i].ADCCh1;
            float offset = get7689Offset(ADCChID);
            float scaler = get7689Scaler(ADCChID);
            
            // first configure the DAC MUX
            setDACmux(dacID, (int16)i);
            // then read the ADC voltage
            float result = get7689Voltage(ADCchID, scaler, offset);
            printf("%f ",result);
         }
         printf("\n");
      }
      else
      {
         printf(badChannel);
      }
   }
   else
   {
      printf(badChannel);
   }
}

/*****************************************************************************/
/* OFFSET COMMAND HANDLERS                                                   */
/*****************************************************************************/
void exeSetOffset()
{
   int8 chID;
   unsigned int16 value;
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<8)
      {
         int chID = offsetParams[chIndex].channel;
         chID=chID+(bankID*8);
         float voltage = atof(rxBuff[pCmd].Param[3]);
         if(voltage <= offsetParams[bankID].max && voltage >= offsetParams[chIndex].min)
         {
            
            offsetDef[chIndex+(bankID*8)]=voltage;
            float scaler = offsetParams[chIndex].scaler;
            voltage *=scaler;
            value =32768 + (signed int16)voltage;
            setVoltage(value, cdsOS, chID);
            printf(success);
         }
         else
         {
            printf(outofRange);
         }
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }       
}

void exeGetOffset()
{
   int8 chID;
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<8)
      {
         printf("%f\n",offsetDef[chIndex+(bankID*8)]);   
      }
      else
      {
         printf(badChannel);
      }
   }   
   else
   {
      printf(badChannel);
   }       
}

void exeRdOffset()
{
   int8 bankID = getMenuIndex(rxBuff[pCmd].Param[2],offsetSubSubMenuItems);
   if (bankID <=1)
   {
      int8 chIndex=getOSRecordIndex(rxBuff[pCmd].Param[1]);
      if (chIndex<8)
      {
         int ADCchID = offsetParams[chindex].ADCCh0;
         float offset = get7689Offset(ADCChID);
         float scaler = get7689Scaler(ADCChID);
         
         // first configure the DAC MUX
         chIndex+=(bankId*8);
         setDACmux(cdsOS, (int16)chIndex);
         // then read the ADC voltage
         float result = get7689Voltage(ADCchID, scaler, offset);
         printf("%f\n",result);
      }
      else if (chIndex ==8) // read all
      {
         
      }
   }
}
/*****************************************************************************/
/* DAC PRESET COMMAND HANDLERS                                               */
/*****************************************************************************/
void exeLoadDAC()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],presetSubMenuItems);
   switch(menuIndex)
   {
      case pErase:
         loadPresetValues(erase);
         break;
      case pRead:
         loadPresetValues(read);
         break;
      case pExpose:
         loadPresetValues(expose);
         break;
      case pWipe:
         loadPresetValues(wipe);
         break;
      case pBT1:
         loadPresetValues(biasTest1);
         break;
      case pOffset:
         loadPresetValues(offset);
         break;
      case pOT1:
         loadPresetValues(osTest1);
         break;
      default:
         printf(badCmd);     
         return;
   }
   printf(success);
}

/*****************************************************************************/

void exeSaveDAC()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],presetSubMenuItems);
   switch(menuIndex)
   {
      case pErase:
         savePresetValues(erase);
         printf(success);
         break;
      case pRead:
         savePresetValues(read);
         printf(success);
         break;
      case pExpose:
         savePresetValues(expose);
         printf(success);
         break;
      case pWipe:
         savePresetValues(wipe);
         printf(success);
         break;
      case pBT1:
         savePresetValues(biasTest1);
         printf(success);
         break;   
      case pOffset:
         savePresetValues(offset);
         printf(success);
         break;
      case pOT1:
         savePresetValues(osTest1);
         printf(success);
         break;   
      default:
         printf(badParam);
         return;
   }
   printf(success);
}

/*****************************************************************************/
/* POWER COMMAND HANDLERS                                                    */
/*****************************************************************************/
void exeSetPower()
{
   int8 on_off= getMenuIndex(rxBuff[pCmd].Param[2],powerSubSubMenuItems);
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],powerSubMenuItems);
   if (on_off==pOff)
   {
      switch (menuIndex)
      {
         case p33:
            output_low(EN_P3V3);
            break;
         case p5:
            output_low(EN_5V);
            initialize7869();
            break;
         case p12:
            output_low(EN_12V);
            break;
         case p24:
            output_low(EN_24V);
            output_low(_EN_Clks);
            break;
         case p54:
            output_low(EN_54V);
            break;
         case pPA:
            output_low(EN_PA);
            break;
         case pLVDS:
            output_low(EN_LVDS);
            break;
         case pVbb0:
            output_low(EN_54V_AMP0);
            break;
         case pVbb1:
            output_low(EN_54V_AMP1);
            break;
         case pClks:           
            output_high(_EN_CLKS);
            break;
         case pAll:
            output_low(EN_P3V3);
            output_low(EN_5V);
            output_low(EN_PA);
            output_low(EN_LVDS);
            output_low(EN_12V);
            output_low(EN_24V);
            output_low(_EN_CLKS); // don't drive enable line after power removed
            output_low(EN_54V);
            output_low(EN_54V_AMP0);
            output_low(EN_54V_AMP1);
            initialize7869();
            break;
         default:
            printf(badParam);
            return;
      }
      printf(success);
   }
   else if(on_off==pOn)
   {
      switch (menuIndex)
      {
         case p33:
            output_high(EN_P3V3);
            break;
         case p5:
            output_high(EN_5V);
            break;
         case p12:
            output_high(EN_12V);
            break;
         case p24:
            output_high(_EN_CLKS);
            output_high(EN_24V);
            break;
         case p54:
            output_high(EN_54V);
            break;
         case pPA:
            output_high(EN_PA);
            break;
         case pLVDS:
            output_high(EN_LVDS);
            break;
         case pVbb0:
            output_high(EN_54V_AMP0);
            break;
         case pVbb1:
            output_high(EN_54V_AMP1);
            break;
          case pClks:
            if(EN_24V)
            {
               output_low(_EN_CLKS);
            }
            break;
         case pAll:
            output_high(EN_P3V3);
            output_high(EN_5V);
            output_high(EN_PA);
            output_high(EN_LVDS);
            output_high(EN_12V);
            output_high(EN_24V);
            output_high(EN_54V);
            output_high(EN_54V_AMP0);
            output_high(EN_54V_AMP1);
            break;
         default:
            printf(badChannel);
            return;
      }
      printf(success);
   }
   else
   {
      printf(badParam);
   }
}

/*****************************************************************************/

void exeGetPower()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],powerSubMenuItems);
   printf(tbd);
}

/*****************************************************************************/
/* FAST CLOCK COMMAND HANDLERS                                               */
/*****************************************************************************/
void exeSetFast()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],fastSubMenuItems);
   switch (menuIndex)
   {
      case fFast:
         output_high(FAST);
         break;
      case fSlow:
         output_low(FAST);
         break;
      default:
         printf(badParam);
         return;
   }
   printf(success);
}

/*****************************************************************************/
/* VOLTAGE COMMAND HANDLERS                                                  */
/*****************************************************************************/
void exeGetSupplyVoltage()
{
   int8 chIndex=getVoltageRecordIndex(rxBuff[pCmd].Param[1]);
   if (chIndex<10)
   {
      printf("%f\n",getVoltage(chIndex));
   }
   else
   {
      printf(badChannel);
   }
}

void exeCalSupplyVoltage()
{
   int8 chIndex=getVoltageRecordIndex(rxBuff[pCmd].Param[1]);
   if (chIndex<10)
   {
      float voltage = atof(rxBuff[pCmd].Param[2]);
      if (voltage > vParams[chIndex].min && voltage < vParams[chIndex].max)
      {
         updateVscaler(voltage, chIndex);
         printf(success);
      }
      else
      {
         printf(outOfRange);
      }
   }
   else
   {
      printf(badChannel);
   }
}


/*****************************************************************************/
/* TEMPERATURE COMMAND HANDLERS                                              */
/*****************************************************************************/
void exeGetTemperature()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],tempSubMenuItems);
   switch(menuIndex)
   {
      case tCCD0:
         printf("%f\n",get_CCDtemperature(ch0));
         break;
      case tCCD1:
         printf("%f\n",get_CCDtemperature(ch1));
         break;
      case tFEE:
         printf(tbd); // need to get ffe temp from the 7689
         break;
      case tPA:
         printf(tbd); 
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* CAL 7689 COMMAND HANDLER                                                  */
/*****************************************************************************/

void exeCal7689()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],cal7689SubMenuItems);
   switch(menuIndex)
   {
      case cBias:
         cal7689LVBiasChannel();
         printf(success);
         break;
      case cCDS:
         cal7689LVCDSChannel();
         printf(success);
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* GET SERIAL NUMBER                                                         */
/*****************************************************************************/

void exeGetSerial()
{
   int32 serial = 0;
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],snSubMenuItems);
   switch(menuIndex)
   {
      case snFEE:
         int32 *ptr = &serial;
         read_program_memory(0x2c, ptr, 4);
         printf("%Lu\n",serial);
         break;
      case snADC:
         serial = getSerNum(serADC);
         printf("%Lu\n",serial);
         break;
      case snPA0:
         serial = getSerNum(serPA0);
         printf("%Lu\n",serial);
         break;
      case snPA1:
         serial = getSerNum(serPA1);
         printf("%Lu\n",serial);
         break;
      default:
         printf(badParam);
         break;
   }
}

/*****************************************************************************/
/* SET SERIAL NUMBER                                                         */
/*****************************************************************************/

void exeSetSerial()
{
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],snSubMenuItems);
   unsigned int32 serial=atoi32(rxBuff[pCmd].Param[2]);
   switch(menuIndex)
   {
      case snADC:
         setSerNum(serADC, serial);
         printf(success);
         break;
      case snPA0:
         setSerNum(serPA0, serial);
         printf(success);
         break;
      case snPA1:
         setSerNum(serPA1, serial);
         printf(success);
         break;
      default:
         printf(badParam);
         break;
   }
}


/*****************************************************************************/
/* MAIN COMMAND HANDLER                                                      */
/*****************************************************************************/

void executeCmd()
{
   if (rxBuff[pCmd].Status==recOverrun)
   {
      #IFDEF USE_LED
      output_high(LED);
      #ENDIF
      output_high(EN_RS232_DRV);// enable driver for response
      printf(overrun);
      rxBuff[pCmd].Status=recEmpty;
   }
   else if (rxBuff[pCmd].Status==cmdReady)
   {
      
      #IFDEF USE_LED
      output_high(LED);
      #ENDIF
      output_high(EN_RS232_DRV);// enable driver for response
   
      int8 menuIndex;
      menuIndex = getMenuIndex(rxBuff[pCmd].Param[0], mainMenuItems);
        
      switch(menuIndex)
      {
         case sCB: // setClockBias
            exeSetClockBias();
            break;        
         case gCB: // getClockBias
            exeGetClockBias();
            break;      
         case rCB: // read the actual clock bias via ADC
            exeRdClockBias();
            break;
         case sOS: // setOffset
            exeSetOffset();
            break;        
         case gOS: // getOffset
            exeGetOffset();
            break;      
         case rOS: // read the actual offset voltage via ADC
            exeRdOffset();
            break;
         case lDac: // load DAC with presets
            exeLoadDAC();
            break;        
         case sDAC: // save DAC presets
            exeSaveDAC();
            break;       
         case sPower: // enable/disable regulators
            exeSetPower();
            break;        
         case gPower: // get regulator status
            exeGetPower();
            break;        
         case sFast: // set Fast/Slow mode
            exeSetFast();
            break;         
         case gSupply: //get supply voltage
            exeGetSupplyVoltage();
            break;        
         case cSupply: //calibrate supply voltage
            exeCalSupplyVoltage();
            break;
         case gTemp: //get temperature
            exeGetTemperature();
            break;       
         case gRevision: // get firmware Revision
            printf(firmwareRev);
            break;
         case gSerial: // get serial numbers
            exeGetSerial(); 
//!            int32 serial = 0;
//!            int32 *ptr = &serial;
//!            read_program_memory(0x2c, ptr, 4);
//!            printf("%Lu\n",serial);
           break;
         case sSerial: // set serial numbers
            exeSetSerial();
            break;
         case calADC: // calibrate the 7869 ADC's
            exeCal7689();
            break;
         default:
            printf(badCmd);
            break; 
      }
      rxBuff[pCmd].Status=recEmpty;
   }    
   else if (pCmd!=pRec) // get next record but never move ahead of rx buffer
   {
      pCmd+=1;
      if (pCmd>=numRec)
      {
         pCmd=0;
      }
   } 
   delay_ms(100);
   //output_low(EN_RS232_DRV); // disable driver
   output_low(LED);
}

#ENDIF
