#define startCharacter '~'
#define startCharacterNoEcho '@'
#define endCharacter '\n'
#define delimiterCharacter ','
#define ignoreChr0   '\r'
#define ignoreChr1   ' '

#define recEmpty 0
#define recActive 1
#define recFull '#'
#define recOverrun 2
#define cmdReady recFull

#define allRecords 254
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
#define getRevision "gr"         // returns code revision

// Set/Get serial number
#define getSerialNumber "gs"
#define setSerialNumber "ss"
   #define sn_FEE       "FEE"    // FEE serial number is hardcoded
   #define sn_ADC       "ADC"    // ACC, PA0, PA1 can be written
   #define sn_PA0       "PA0"
   #define sn_PA1       "PA1"
   #define sn_CCD0      "CCD0"
   #define sn_CCD1      "CCD1"
   #define sn_all       "all"

// Set/get the clock Bias Voltages
#define setClockBias "sb" // COMMAND
#define getClockBias "gb" 
#define rdClockBias  "rb" // reads the actual voltage 
   #define cb_Ppos      "P_on"//"Pp" // PARAMETER 1
   #define cb_Pneg      "P_off"//"Pn"
   #define cb_DGpos     "DG_on"//"DGp"
   #define cb_DGneg     "DG_off"//"DGn"
   #define cb_Spos      "S_on"//"Sp"
   #define cb_Sneg      "S_off"//"Sn"
   #define cb_SWpos     "SW_on"//"SWp" // Summing Well
   #define cb_SWneg     "SW_off"//"SWn"
   #define cb_RGpos     "RG_on"//"RGp" // Reset Gate
   #define cb_RGneg     "RG_off"//"RGn"
   #define cb_OG        "OG"
   #define cb_RD        "RD"
   #define cb_OD        "OD"
   #define cb_BB        "BB"
   #define cb_all       "all"
   #define cb_all_off   "all_off"
      #define cb_0         "ch0" // PARAMETER 2
      #define cb_1         "ch1"


// Set/Get the CDS offset voltages
#define setCDS_OS    "so" //
#define getCDS_OS    "go" 
#define rdCDS_0S     "ro"
   #define co_0pos      "0p" // include voltage as decimal
   #define co_0neg      "0n"
   #define co_1pos      "1p"
   #define co_1neg      "1n"
   #define co_2pos      "2p"
   #define co_2neg      "2n"
   #define co_3pos      "3p"
   #define co_3neg      "3n"
   #define co_all       "all"
      #define co_0         "ch0"
      #define co_1         "ch1"

// load/save bias presets
#define loadDACPresets  "lp"
#define saveDACPresets  "sp"
   #define pb_erase        "erase"
   #define pb_read         "read"
   #define pb_expose       "expose"
   #define pb_wipe         "wipe"
   #define pb_idle         "idle"
   #define pb_fastRev      "fastRev"
   #define pb_offset       "offset"
   
#define getLastBiasMode "gp"   
  
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
   #define pe_Clks     "Clks"// Clock output enable
   #define pe_all      "all"
      #define pe_on     "on"
      #define pe_off    "off"

// Set Fast Operation
#define setFast      "sf"
#define getFast      "gf"
   #define sf_fast      "fast"
   #define sf_slow      "slow"
    
// Read supply voltages
#define calSupplyVoltage  "cv" // example: ~cv,3V3M,3.315
#define getSupplyVoltage  "rv"
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
   #define gv_all          "all"
   
// Read temperature
#define getTemperature "rt"
   #define gt_CCD0      "ccd0"
   #define gt_CCD1      "ccd1"
   #define gt_Preamp    "PA"
   #define gt_FEE       "FEE"
   #define gt_all       "all"
   
// Calibrate 7689
#define cal7689         "cal"
   #define c7_bias      "bias"
   #define c7_CDS       "CDS"

#define getIniByte      "gI" // this is set to 0xFF if Eeprom unititialized
#define setIniByte      "sI" // set to 0x00 with default initialation...
                             // sI byte can be written - it is non volitile 
#define getRunState   "gRS" // runState is volatile, and is reset on boot to null
#define setRunState   "sRS" // can be written. RS is an 8 character string.

#define getStatus       "gStat"
#define sysReset        "reboot"
/*****************************************************************************/
/* RESPONSE MESSAGES                                                            */
/*****************************************************************************/

#define badCmd          "INVALID COMMAND\n"
#define badParam        "INVALID PARAMETER\n"
#define badChannel      "INVALID CHANNEL\n"
#define outOfRange      "OUT OF RANGE\n"
#define overrun         "BUFFER OVERRUN\n"
#define success         "SUCCESS\n"
#define tbd             "NOT IMPLEMENTED YET\n"
#define unknown         "unknown\n"
#define functionDisabled "Function Disabled\n"
#define clkErr "Error: 24V must be enabled before clocks\n"
#define LVDSErr "Error: 12V must be enabled before LVDS\n"
#define badRS "INVALID RUN STATE"
                                             
                                                                                

                        
