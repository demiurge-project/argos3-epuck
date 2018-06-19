# Uploading the E-Puck firmware

Compiler: `xc16-v1.34`

OS: `Ubuntu 16.04 x64`

## 1- Install the XC16 compiler

Download the XC16 installer: 
```
xc16-v1.34-full-install-linux-installer.run
```
Change the permissions for the installer:
```
chmod u+x xc16-v1.34-full-install-linux-installer.run
```
Run the installer and accept to add XC16 to the `$PATH` environment variable:
```
sudo ./xc16-v1.34-full-install-linux-installer.run
```
Reboot the terminal to load `$PATH`.

## 2- Compile the E-Puck firmware

Clone the E-Puck repository:
```
git clone https://github.com/demiurge-project/argos3-epuck.git argos3-epuck
```
Compile the firmware:
```
cd argos3-epuck/firmware/firmware
mkdir build
cd build
cmake ..
make
```
Generate the `epuck_firmware.hex` file:
```
cd firmware
xc16-bin2hex epuck_firmware
```

## 3- Upload the firmware to the E-Puck:

Install bluetooth tools:
```
sudo apt-get install bluez
sudo apt-get install bluez-tools
sudo apt-get install bluez-utils
sudo apt-get install libbluetooth-dev
```
Scan the available devices and make sure the robot is available:
```
hcitool scan
```
Compile the uploader `epuckuploadbt`:
```
cd argos3-epuck/firmware/uploader-linux-libbluetooth
make
```
Pair the robot and the computer:
```
bluetoothctl
scan on (wait to the desired device)
scan off
info [MAC of the robot]
trust [MAC of the robot]
agent on 
pair [MAC of the robot] // The PIN code corresponds to the ID of the robot
info [MAC of the robot] // Just to check
exit
```
Upload the firmware:
```
./epuckuploadbt [path/to/epuck_firmware.hex] [robot_ID]
```
The following message will appear on the terminal:
```
Press enter and then reset on the robot
```
After pushing the reset button the terminal will display the indicator:
```
Uploading: #####################
```
























