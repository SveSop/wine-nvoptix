# wine-nvoptix a OptiX relay library for wine 

[Wine](https://www.winehq.org/) is a compability software allowing Linux and Mac OS users to run Windows software as native applications. Wine works by translating calls to windows libraries to native Linux / Mac OS libraries.

wine-nvoptix is a library to be used with wine. It aims at allowing Linux / Mac OS users to launch Windows software using the OptiX API. This API, developed by nVidia, is used to do Ray Tracing calculations using nVidia GPU, mostly in 3D environments. This library, wine-optix, works by creating a fake windows Dll which will translate calls to the Windows nVidia Optix API to the Linux / Mac OS native API.

This project is heavily based on the public documentation and source files from nVidia: https://raytracing-docs.nvidia.com/

This work is done freely to help the users community to run their Windows programs on Linux / Mac OS.

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
