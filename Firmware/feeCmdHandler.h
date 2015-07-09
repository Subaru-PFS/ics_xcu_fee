#define startCharacter '~'
#define endCharacter '\n'
#define delimiterCharacter ','

#define recEmpty 0
#define recActive 1
#define recFull '#'
#define cmdReady recFull

#define noRecordFound 255


//command set
/*------------------------------
command structure is as follows:
[startCharacter][cmd],[subcmd],[data][CR]
for example:
~sb,dft,ch0[CR]
set default biasses for channel 0

~sb,Pp,5.124
-------------------------------*/

// get code revision
#define getRevision "gr"

// Set/get the clock Bias Voltages
#define setClockBias "sb" // COMMAND
#define getClockBias "gb" 
   #define cb_Ppos      "Pp" // PARAMETER 1
   #define cb_Pneg      "Pn"
   #define cb_DGpos     "DGp"
   #define cb_DGneg     "DGn"
   #define cb_Spos      "Sp"
   #define cb_Sneg      "Sn"
   #define cb_SWpos     "SWp"
   #define cb_SWneg     "SWn"
   #define cb_RGpos     "RGp"
   #define cb_RGneg     "RGn"
   #define cb_OG        "OG"
   #define cb_RD        "RD"
   #define cb_OD        "OD"
   #define cb_BB        "BB"
      #define cb_0         "ch0" // PARAMETER 2
      #define cb_1         "ch1"
   
// Set/Get the CDS offset voltages
#define setCDS_OS    "so" //
#define getCDS_OS    "go" 
   #define co_0pos      "0p" // include voltage as decimal
   #define co_0neg      "0n"
   #define co_1pos      "1p"
   #define co_1neg      "1n"
   #define co_2pos      "2p"
   #define co_2neg      "2n"
   #define co_3pos      "3p"
   #define co_3neg      "3n"
      #define co_0         "ch0"
      #define co_1         "ch1"

// load/save bias presets
#define loadDACPresets  "lp"
#define saveDACPresets  "sp"
   #define pb_erase        "erase"
   #define pb_read         "read"
   #define pb_expose       "expose"
   #define pb_wipe         "wipe"
   #define pb_offset       "offset"
   #define pb_osTest1      "T1"
   #define pb_osTest2      "T2"
   #define pb_osTest3      "T3"
  
// Enable the power supplies   
#define setPowerEn   "se" // must include 0 or 1 for off or on
#define getPowerEn   "ge"
   #define pe_3V3reg   "3V3"
   #define pe_5Vreg    "5V"
   #define pe_12Vreg   "12V"
   #define pe_24Vreg   "24V"
   #define pe_54Vreg   "54V"
   #define pe_Preamp   "PA"
   #define pe_LVDS     "LVDS"
   #define pe_Vbb0     "Vbb0"// Bias amplifier enable
   #define pe_Vbb1     "Vbb1"// Bias amplifier enable
   #define pe_all      "all"
      #define pe_on     "on"
      #define pe_off    "off"

// Set Fast Operation
#define setFast      "sf"
   #define sf_fast      "fast"
   #define sf_slow      "slow"
    
// Read supply voltages
#define calSupplyVoltage  "cv"
#define getSupplyVoltage  "gv"
   #define gv_3V3Micro     "3V3M"
   #define gv_3V3Other     "3V3"
   #define gv_5Vpos        "5VP"
   #define gv_5Vneg        "5VN"
   #define gv_5Vpos_pa     "5VPpa"
   #define gv_5Vneg_pa     "5VNpa"
   #define gv_12Vpos       "12VP"
   #define gv_12Vneg       "12VN"
   #define gv_24Vneg       "24VN"
   #define gv_54Vpos       "54VP"
   
// Read temperature
#define getTemperature "gt"
   #define gt_CCD0      "ccd0"
   #define gt_CCD1      "ccd1"
   #define gt_Preamp    "PA"
   #define gt_FEE       "FEE"

/*****************************************************************************/
/* RESPONSE MESSAGES                                                            */
/*****************************************************************************/

#define badCmd          "INVALID COMMAND\r\n"
#define badParam        "INVALID PARAMETER\r\n"
#define badChannel      "INVALID CHANNEL\r\n"
#define outOfRange      "OUT OF RANGE\r\n"
#define success         "SUCCESS\r\n"
#define tbd             "NOT IMPLEMENTED YET\r\n"
                                             
                                                                                

                        
