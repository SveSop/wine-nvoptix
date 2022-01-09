# nvOptiX-Relay for wine  

This project intend to relay functions from nvoptix.dll <-> libnvoptix  

Requires: Meson + Ninja + other usual suspects  

Compile with: ./package_release.sh /install/folder - eg: ./package_release.sh /home/user/  
The binaries will then be placed in /home/user/nvoptix  

Install the nvoptix.dll relay to your wineprefix by running the script:  
cd /home/user/nvoptix  
WINEPREFIX=/your/wine/prefix ./setup_nvoptix.sh install  
