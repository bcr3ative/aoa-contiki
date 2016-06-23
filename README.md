# AoA Contiki

## Installation:

To be continued

## Notes:

- To make Contiki not disable the bootloader or change the access input combo (on CC2650) make the following changes in CONTIKI_FOLDER/cpu/cc26xx-cc13xx/lib/cc26xxware/startup_files/ccfg.c :

  Under bootloader settings:
  ```
  #define SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE            0xC5
  #define SET_CCFG_BL_CONFIG_BL_LEVEL                  	  0x0
  #define SET_CCFG_BL_CONFIG_BL_PIN_NUMBER                0x0B
  #define SET_CCFG_BL_CONFIG_BL_ENABLE                 	  0xC5
  ```
