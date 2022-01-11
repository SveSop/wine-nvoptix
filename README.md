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

## License:  
Parts of this project is released on the terms of MIT license, however various headers and sourcecode is released under NVIDIA Licese.  

## NVIDIA License:  
```
Copyright (c) 2021 NVIDIA Corporation.  All rights reserved.

NVIDIA Corporation and its licensors retain all intellectual property and proprietary
rights in and to this software, related documentation and any modifications thereto.
Any use, reproduction, disclosure or distribution of this software and related
documentation without an express license agreement from NVIDIA Corporation is strictly
prohibited.

TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGES
```
Also read: [https://docs.nvidia.com/sdk-manager/eula/index.html](https://docs.nvidia.com/sdk-manager/eula/index.html)  
