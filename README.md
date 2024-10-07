# VRC-Haptics-Firmware-ESP
Firmware for the DIY haptics solution, See the projects PC application for more info: [VRC-Haptics-Host](https://github.com/virtuallyaverage/VRC-Haptics-Host)

## Usage
### Setup Environment
1. This project uses the Visual Studio Code extension PlatformIO. Please install both:
	- [Visual Studio Code](https://code.visualstudio.com/)
	- [PlatformIO](https://docs.platformio.org/en/latest/integration/ide/pioide.html#platformio-for-vscode)
2. Download this repository:
	- Use the big green "Code" button on at the top of the page and select Download Zip. 
	- Extract this repository somewhere you can find it later.
		- We will reference this location (path) as `<repository>` going forward.
### Setup Configuration
1. Open Config file:
	- Open the folder`<repository>/VRC-Haptics-Firmware-ESP` in Visual Studio Code
	- In VSCode open the config.hpp file found in the `include` folder.
2. **IMPORTANT** Notes on formatting:
	- Being precise with typing is **VERY** important. Spaces, CAPITALS, lowercase, and commas (`,`) all have meanings. Missing one will likely throw an error. Likely nothing catastrophic, but probably a pain to diagnose without help.
		- Most lines start with a hashtag prefix, like: `#define` They require a space between the keyword, and it's definition: `#define keyword configuration`
	-  Green lines are comments and don't affect the program at all (start with `//`)
		- To "uncomment" remove the two backslashes preceding a line: `//#define ROLE_VEST` -> `#define ROLE_VEST`
		- To "comment" a line do the reverse: `#define ROLE_VEST` -> `//#define ROLE_VEST`
3. Set Configuration: 
	There are 2 main items to configure: WIFI and Device Role
	1. WIFI: 
		1. `WIFI_SSID` this is your WIFI name you see on your computer. It tells the haptic device which network to connect to.
		2. `WIFI_PASSWORD`This is the password to your network, like what is used to connect your computer to WIFI.
	2. Device Role:
		1. Uncomment the role of the device you built to use the default configurations. 
		2. e.g. If you have a head haptics device, remove the `//` from the `#define ROLE_HEAD` and add them before 
		3. **NOTE:** You should only have ONE device role uncommented at a time. 
### Upload Program 
Next and final step is uploading the program to your device. 
1. Connect Board to computer VIA USB
2. Expand your platform type from the PlatformIO (wasp logo) on the left sidebar
3. Build and upload firmware: `<PLATFORM NAME>` -> `General` -> `Upload`
### Enjoy!
If your configuration is accurate, your board is now capable of connecting to the server and driving your haptics. Have fun!