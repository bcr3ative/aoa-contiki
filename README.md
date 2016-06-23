# AoA Contiki

[JeeLocLib](https://github.com/darbula/JeeLocLib) port of the AoA module to Contiki platform.

## Installation:

For more information about other platforms please visit the following link https://github.com/contiki-os/contiki/tree/master/platform/ .


- Clone Contiki repository ( https://github.com/contiki-os/contiki )

- Download the ARM toolchain for linux ( https://launchpad.net/gcc-arm-embedded )

- Export the path to the toolchain's binary directory:

  `export PATH=$PATH:{ABSOLUTE_PATH_TOOLCHAIN_TO_DIRECTORY}/bin`

- Add the ftdi_sio kernel module and its vendor/product identifiers:

  ```
  modprobe ftdi_sio
  /bin/sh -c "echo 0403 a6d1 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id"
  ```
- To enable flashing and reading from the serial port, connect and power up the SmartRF06 EB with 2650 EM attached on it.
Two device nodes should appear ex. /dev/ttyUSB{0,1} . Give to others group read and write permissions on those nodes:

  `chmod o+rw /dev/ttyUSB{0,1}`

- Install `srecord` package from your distribution package manager to enable makefile to manipulate EPROM load files 

- Clone  AoA Contiki repository ( https://github.com/bcr3ative/aoa-contiki )

- Change the relative path pointed by the AoA Contiki's Makefile to the Contiki directory you previously cloned

- In the AoA Contiki directory execute the following command:

  `make`

- Before flashing press and hold SELECT + EM RESET buttons to get access to the bootloader

- Flashing on a specified port ( here /dev/ttyUSB1 ) can be done by issuing the following command:

  `make aoa_process.upload PORT=/dev/ttyUSB1`

## Notes:

- To make Contiki not disable the bootloader or change the access input combo (on CC2650) make the following changes in CONTIKI_FOLDER/cpu/cc26xx-cc13xx/lib/cc26xxware/startup_files/ccfg.c :

  Under bootloader settings:
  ```
  #define SET_CCFG_BL_CONFIG_BOOTLOADER_ENABLE            0xC5
  #define SET_CCFG_BL_CONFIG_BL_LEVEL                  	  0x0
  #define SET_CCFG_BL_CONFIG_BL_PIN_NUMBER                0x0B
  #define SET_CCFG_BL_CONFIG_BL_ENABLE                 	  0xC5
  ```
