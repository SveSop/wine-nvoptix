# wine-nvoptix a OptiX relay library for wine

[Wine](https://www.winehq.org/) is a compability software allowing Linux and Mac OS users to run Windows software as native applications. Wine works by translating calls to windows libraries to native Linux / Mac OS libraries.

wine-nvoptix is a library to be used with wine. It aims at allowing Linux / Mac OS users to launch Windows software using the OptiX API. This API, developed by NVIDIA, is used to do Ray Tracing calculations using NVIDIA GPU, mostly in 3D environments. This library, wine-optix, works by creating a fake windows Dll which will translate calls to the Windows NVIDIA Optix API to the Linux / Mac OS native API.

This project is heavily based on the public documentation and source files from NVIDIA: [https://raytracing-docs.nvidia.com/](https://raytracing-docs.nvidia.com/)

This work is done freely to help the users community to run their Windows programs on Linux / Mac OS.

Current release: v0.3  

## Building

Compile requirements: Meson + Ninja + other usual suspects like GCC and Wine-9.0 dev  

Compile with: ./package_release.sh /install/folder - eg: `./package_release.sh /home/user/`  
The binaries will then be placed in `/home/user/nvoptix`  

## Usage

Install the nvoptix.dll relay to your wineprefix by running the script:  
`cd /home/user/nvoptix`  
`WINEPREFIX=/your/wine/prefix ./setup_nvoptix.sh install`  

You need a working Wine version with wineprefix set up (see below for requirements), and a correctly configured NVIDIA Graphics adapter using proprietary NVIDIA drivers 535 or later  

## Requirements

[DXVK-NVAPI](https://github.com/jp7677/dxvk-nvapi)  
[CUDA]Standalone CUDA implementation for Wine [NVCUDA](https://github.com/SveSop/nvcuda)  

Recommended: [WINE-NVML](https://github.com/Saancreed/wine-nvml) for hardware function detection from NVIDIA GPU  

## License

Parts of this project is released on the terms of MIT license, however various headers and sourcecode is released under under different license.  
Files and headers in the `include` folder is released under GNU Lesser General Public License - Wine  
Header files in the `src` folder is released under NVIDIA License.  

## Wine License

```text
Copyright (c) 1993-2022 the Wine project authors (see the file AUTHORS
for a complete list)

Wine is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version. 

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

A copy of the GNU Lesser General Public License is included in the
Wine distribution in the file COPYING.LIB. If you did not receive this
copy, write to the Free Software Foundation, Inc., 51 Franklin St,
Fifth Floor, Boston, MA 02110-1301, USA.
```

## NVIDIA License

```text
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
