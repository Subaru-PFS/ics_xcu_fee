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
enum mainMenu {sCB,gCB,sOS,gOS,lDAC,sDAC,sPower,gPower,sFast,gSupply,cSupply,gTemp,gRevision};
char rom mainMenuItems[][8]=                   {setClockBias,
                                                getClockBias,
                                                setCDS_OS,
                                                getCDS_OS,
                                                loadDACPresets,
                                                saveDACPresets,
                                                setPowerEn,
                                                getPowerEn,
                                                setFast,
                                                getSupplyVoltage,
                                                calSupplyVoltage,
                                                getTemperature,
                                                getRevision,
                                                "\0"};
                                      
enum biasSubMenu {Pp,Pn,DGp,DGn,Sp,Sn,SWp,SWn,RGp,RGn,OG,RD,OD,BB};                   
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

enum presetSubMenu {pErase,pRead,pExpose,pWipe,pOffset,pT1,pT2,pT3};
char rom presetSubMenuItems[][8]=              {pb_erase,
                                                pb_read,
                                                pb_expose,
                                                pb_wipe,
                                                pb_offset,
                                                pb_osTest1,
                                                pb_osTest2,
                                                pb_osTest3,
                                                "\0"};

enum powerSubMenu {p33,p5,p12,p24,p54,pPA,pLVDS,pVbb0,pVbb1,pAll};
char rom powerSubMenuItems[][8]=               {pe_3V3reg,
                                                pe_5Vreg,
                                                pe_12Vreg,
                                                pe_24Vreg,
                                                pe_54Vreg,
                                                pe_Preamp,
                                                pe_LVDS,
                                                pe_Vbb0,
                                                pe_Vbb1,
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
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],biasSubMenuItems);
   printf(tbd);
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
   int8 menuIndex = getMenuIndex(rxBuff[pCmd].Param[1],offsetSubMenuItems);
   printf(tbd);
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
      case pOffset:
         loadPresetValues(offset);
         break;
      case pT1:
         loadPresetValues(osTest1);
         break;
      case pT2:
         loadPresetValues(osTest1);
         break;
      case pT3:
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
      case pOffset:
         savePresetValues(offset);
         printf(success);
         break;
      case pT1:
         savePresetValues(osTest1);
         printf(success);
         break;
      case pT2:
         savePresetValues(osTest2);
         printf(success);
         break;
      case pT3:
         savePresetValues(osTest3);
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
            break;
         case p12:
            output_low(EN_12V);
            break;
         case p24:
            output_low(EN_24V);
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
            output_low(EN_54V_AMP0);
            break;
         case pAll:
            output_low(EN_P3V3);
            output_low(EN_5V);
            output_low(EN_PA);
            output_low(EN_LVDS);
            output_low(EN_12V);
            output_low(EN_24V);
            output_low(EN_54V);
            output_low(EN_54V_AMP0);
            output_low(EN_54V_AMP1);
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
      printf("%f\r\n",getVoltage(chIndex));
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
         printf("%f\r\n",get_CCDtemperature(ch0));
         break;
      case tCCD1:
         printf("%f\r\n",get_CCDtemperature(ch1));
         break;
      case tFEE:
         printf(tbd);
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
/* MAIN COMMAND HANDLER                                                      */
/*****************************************************************************/

void executeCmd()
{
   if (rxBuff[pCmd].Status!=cmdReady) // only execute if cmd is ready
   {
      pCmd+=1;
      if (pCmd>=numRec)
      {
         pCmd=0;
      }
   }
   else
   {
      rxBuff[pCmd].Status=0;
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
         case sOS: // setOffset
            exeSetOffset();
            break;        
         case gOS: // getOffset
            exeGetOffset();
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
         default:
            printf(badCmd);
            break;   
      }   
   }  
   delay_ms(100);
   //output_low(EN_RS232_DRV); // disable driver
   output_low(LED);
}
#ENDIF
