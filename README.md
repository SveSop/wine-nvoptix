# wine-nvoptix a OptiX relay library for wine  

This project intend to relay functions from nvoptix.dll <-> libnvoptix  
This will only work with nVidia (tm) GPU with appropriate proprietary drivers  

## Building:  
Compile requirements: Meson + Ninja + other usual suspects like GCC and Wine  

Compile with: ./package_release.sh /install/folder - eg: ./package_release.sh /home/user/  
The binaries will then be placed in /home/user/nvoptix  

## Usage:  
Install the nvoptix.dll relay to your wineprefix by running the script:  
cd /home/user/nvoptix  
WINEPREFIX=/your/wine/prefix ./setup_nvoptix.sh install  

You need a working Wine version with wineprefix set up.  

## Requirements:  
DXVK-NVAPI - [https://github.com/jp7677/dxvk-nvapi](https://github.com/jp7677/dxvk-nvapi)  

Recommended: WINE-NVML - [https://github.com/Saancreed/wine-nvml](https://github.com/Saancreed/wine-nvml)  
