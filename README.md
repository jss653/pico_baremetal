# pico_baremetal

To install the Raspberry Pi Pico SDK on Ubuntu, follow these steps:

Ensure your system has the necessary build tools installed:

``sudo apt update``  
``sudo apt install -y build-essential cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib``  

Install additional packages required for the SDK setup:

``sudo apt-get install jq minicom make cmake gdb-multiarch automake autoconf libtool libftdi-dev libusb-1.0-0-dev pkg-config clang-format -y``  

Clone the Pico SDK repository into a preferred directory, such as ./pico:
``cd <YOUR BASE DIR>``  
``mkdir -p pico``  
``cd pico``  
``git clone -b master https://github.com/raspberrypi/pico-sdk.git``  
``cd pico-sdk``  
``git submodule update --init``  

Verify the installation by checking the SDK and tool versions:

``arm-none-eabi-gcc --version``  
``gdb-multiarch --version``  
``cmake --version``  
``openocd --version``  

Clone this repo into the pico folder  
``cd ../..``  
``git clone git@github.com:jss653/pico_baremetal.git``  

Set the PICO_SDK_PATH environment variable by adding it to your shell profile:

``echo 'export PICO_SDK_PATH=$HOME/pico/pico-sdk' >> ~/.bashrc``  
``source ~/.bashrc``  

or update CMakeLists.txt with the path. ``set(PICO_SDK_PATH "/home/jss653/projects/software/pico/pico-sdk")``  

NOTE. You need to provide the file /pico/pico_baremetal/src/wifi/wifinet.h: or you'll receive an error : No such file or directory
   31 | #include "wifinet.h"
      |          ^~~~~~~~~~~  
../pico/pico_baremetal/src/wifi/wifinet.h  

``#ifndef __WIFINET_H__``  
``#define __WIFINET_H__``  

``#define WIFI_SSID "network"``  
``#define WIFI_PASSWORD "password"``  

``#endif //__WIFINET_H__``  


Build this project with:  
``mkdir build``  
``cd build``  
``cmake ..``  
``make``  

If you want, you can create a CodeBlocks project using cmake
``cmake . -G "CodeBlocks - Unix Makefiles"``  

To load your program using a JTAG interface
``sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program build/baremetal.elf verify reset exit"``  


