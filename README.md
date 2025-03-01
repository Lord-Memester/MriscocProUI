# Professional Firmware for the Voxelab Aquila Printer

![GitHub contributors](https://img.shields.io/github/contributors/classicrocker883/MriscocProUI.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/classicrocker883/MriscocProUI.svg)
[![Build Status](https://github.com/classicrocker883/MriscocProUI/workflows/CI/badge.svg)](https://github.com/classicrocker883/MriscocProUI/actions)
[![Build Configs](https://github.com/classicrocker883/MriscocProUI/actions/workflows/compile-configs.yml/badge.svg?event=release)](https://github.com/classicrocker883/MriscocProUI/releases/latest)

## Universal RET6/RCT6 chips: G32, N32, H32, Creality 4.2.7 and 4.2.2 boards
#### Don't forget to checkout the [thingiverse](https://www.thingiverse.com/classicrocker883/designs) models I designed
[Minimalist Dual Cooling Fan Shroud](https://www.thingiverse.com/thing:5263939) for Aquila / Ender 3 V2
<img src="https://cdn.thingiverse.com/assets/1a/21/e9/e9/f5/card_preview_c60b15d6-69cf-469a-8641-40f6bef37992.png" style="float:left;width:40px;height:40px;" align="left"/>  

[Tool Box for Middle Extrusion](https://www.thingiverse.com/thing:5181522) "**VOXELAB**" engraved
<img src="https://cdn.thingiverse.com/assets/39/e7/eb/bd/9f/card_preview_Voxelab_WideMiddle_Shell.png" style="float:left;width:40px;height:40px;" align="left"/>  

[Fantastic Filament Guide](https://www.thingiverse.com/thing:4974802) for Aquila
<img src="https://cdn.thingiverse.com/assets/46/79/c1/80/14/card_preview_FantasticFilamentGuideAquilaL.png" style="float:left;width:40px;height:40px;" align="left"/>

[Raspberry Pi Zero Case Mount 30mm case width](https://www.thingiverse.com/thing:5166780) for Aquila
<img src="https://cdn.thingiverse.com/assets/73/1e/84/38/f1/card_preview_Pi_Zero_Case_holder.png" style="float:left;width:40px;height:40px;" align="left"/>  

### This custom repository:
[![Build Configs](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/compile-configs.yml/badge.svg)](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/compile-configs.yml)
[![CI](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/test-builds.yml/badge.svg)](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/test-builds.yml)
[![Bump Distribution Date](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/bump-date.yml/badge.svg)](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/bump-date.yml)
[![Close Stale Issues](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/close-stale.yml/badge.svg)](https://github.com/Lord-Memester/MriscocProUI/actions/workflows/close-stale.yml)


---

### - Please read this: -
Many Updates have been included in the most recent MriscocProUI in the **2023-July** release.<br>
H32->HC32 support is currently only in the branch **HC32-July**. You can find the original repo here [@shadow578/Marlin-H32](https://github.com/shadow578/Marlin-H32)<br>
#### Important Info regarding HC32:
* Not all features or options have been fully tested with this chip.  
* * <ins>**FOREWARNING**</ins>: So far everything has been working great - as normal - except there are reports of it restarting after saving with no other negative effects. So far, that is the only issue. I just wanted to give advance notice and if there is anything else to report please do so under [Issues tab](https://github.com/classicrocker883/MriscocProUI/issues).
<br>

General information about the firmware and updates is located in the _Wiki_ page [What's New In This Release](https://github.com/classicrocker883/MriscocProUI/wiki/What's-New-in-this-Release). <br> Other changes and updates are [addressed here](https://github.com/classicrocker883/MriscocProUI/releases/latest) and are [addressed here](https://github.com/mriscoc/Ender3V2S1/releases/latest).

#### One important change to note is the `Mesh Inset` now saves upon restart -- it is working as normal.

If you start printing and it says `Advance Pause` while **Runout** is enabled, you may have to change state it triggers no filament to **HIGH**, or **LOW** (depending on what is already selected). This is found in the _Prepare_ menu/ _Filament Management_/ _Filament Settings_ -> _Runout Active_. <br>
A small issue which may occur is if you are in the <i>Print</i> menu and you quickly select to print between several printable *Gcode* files in a short amount of time. The screen can freeze for a moment and the printer will restart - that is it.<br><br>
If you encounter any issues please feel free to post it on the issues tab, or if anything is going well please leave a comment. 

I will be working on more upgrades and features and tweaks along the way. Enjoy using this fork of Marlin as I intend it to be the best. It is easy to use and convenient. So far I really enjoy the new settings and toolbar for the main menu. There is a variety of parameters and options that can be changed without having to reflash the firmware.

### MarlinSimulator.exe Instructions

There is a *MarlinSimulator.exe* file provided to test out for yourself. It simulates a pre-built configuration of this firmware.

To create your own MarlinSimulator with you own build, start by changing in Configuration.h **MOTHERBOARD** to *BOARD_SIMULATED*, also disable `ENDSTOP_INTERRUPTS_FEATURE` and `PROUI_EX`, and then in platformio.ini **default_envs =** change to *simulator_windows*. The MarlinSimulator can only compile for Manual Mesh Bed Leveling for now, so make sure your configuration is set for `MESH_BED_LEVELING`.

Then you have to extract **SDL2-devel-2.28.0-mingw.zip** from /**docs**. To install SDL, navigate to the SDL2 directory in *MSYS2 MINGW64* terminal and type
>make native

64-bit files are in \x86_64-w64-mingw32. Then copy the contents of ~\SDL2-2.28.0\x86_64-w64-mingw32\include\SDL2\* into ~\.pio\libdeps\simulator_windows\imgui\misc

Once all that is done, just build like you would normally and *MarlinSimulator.exe* will be created.

For MacOS or Linux, you're on your own...sorry.
<br>

[Linear Advance Information](https://github.com/MarlinFirmware/MarlinDocumentation/blob/master/_features/lin_advance.md)

The Precompiled binary files of this firmware can work with STM32 (STM32F103RET6/RCT6 - STMicroelectronics) and it's clones: G32 (GD32F103Rx - GigaDevice), N32 (N32F103Rx / N32G455x - Nation), and H32 (HC32F460x - Huada Semiconductor/HDSC). They can be downloaded from:
[Latest Release](https://github.com/classicrocker883/MriscocProUI/releases/latest)

<img height=260 src="https://enfss.voxelab3dp.com/10001/picture/2021/09/b849845bd0ffa889f00a782aae76ccf3.jpg" align="left" />
<img height=260 src="https://enfss.voxelab3dp.com/10001/picture/2021/09/677b721574efca3daa5c0d39e438fee6.jpg" align="middle" /> 
<img height=260 src="buildroot/share/pixmaps/Ender-3V2.jpg" align="left" />
<img height=300 src="buildroot/share/pixmaps/Ender-3S1.jpg" align="middle"  />
<BR/>

## Donations
Please consider making a donation, as large or as small and as often as you'd like.
Thank you for your support, I receive donations through [Paypal](https://www.paypal.com/paypalme/andrewleduc)   

[<img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif">](https://www.paypal.com/donate/?business=PFNSKQX9WQQ8W&no_recurring=0&currency_code=USD)   

## Wiki
 - [How to install the firmware](https://github.com/mriscoc/Ender3V2S1/wiki/How-to-install-the-firmware)
 - [Installing a 3D/BLTouch](https://github.com/mriscoc/Ender3V2S1/wiki/3D-BLTouch)
 - [Color themes](https://github.com/mriscoc/Ender3V2S1/wiki/Color-Themes)
 - [How to use with Octoprint](https://github.com/mriscoc/Ender3V2S1/wiki/Octoprint)
  
## Community links
* [Voxelab Aquila Facebook Group](https://www.facebook.com/groups/voxelabaquila/?ref=share&mibextid=NSMWBT) 
* [Telegram](https://t.me/ender3v2s1firmware)
* [r/VoxelabAquila on Reddit](https://www.reddit.com/r/VoxelabAquila)
* [r/ender3V2 on Reddit](https://www.reddit.com/r/ender3v2) 
* [r/Ender3v2Firmware on Reddit](https://www.reddit.com/r/Ender3v2Firmware) 
* [E3V2 Facebook](https://www.facebook.com/groups/ender3v2firmware)
* [E3S1 Facebook](https://www.facebook.com/groups/ender3s1printer)

<!--[](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/screenshots/main.jpg)-->

## Marlin Support

The Issue Queue is reserved for Bug Reports and Feature Requests. To get help with configuration and troubleshooting, please use the following resources:

- [Marlin Documentation](https://marlinfw.org) - Official Marlin documentation
- [Marlin Discord](https://discord.gg/n5NJ59y) - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"](https://www.facebook.com/groups/1049718498464482/)
- RepRap.org [Marlin Forum](https://forums.reprap.org/list.php?415)
- Facebook Group ["Marlin Firmware for 3D Printers"](https://www.facebook.com/groups/3Dtechtalk/)
- [Marlin Configuration](https://www.youtube.com/results?search_query=marlin+configuration) on YouTube

## Credits

Thanks to Reddit u/schuh8 and @whasupjohn for donating his board to help test the firmware.
<br>
/u I*U*2
</br>


Find me on [Facebook](https://www.facebook.com/yoboyyy) 

Join the Voxelab Aquila [Facebook Group](https://www.facebook.com/groups/voxelabaquila/)

This fork of Mriscoc's ProUI firmware is maintained by [@classicrocker883](https://github.com/classicrocker883) (yours truly)

ProUI is a Marlin based firmware maintained by [@mriscoc](https://github.com/mriscoc)

The fork for H32|HC32 firmware is maintained by [@shadow578](https://github.com/shadow578)

Marlin is maintained mainly by [@thinkyhead](https://github.com/thinkyhead) 

This work would not be possible without me spending time working on it for free.

I would greatly appreate supporters, helpers and betatesters whenever possible.

Please consider making a donation or show your support or input if you end up using this firmware.

It wasn't easy getting it to this point. I am just a basic programmer and the work is mostly trial and error. Thank goodness for VS Code's compiler which shows me what changes need to be made as I make them.

Marlin firmware is an Open Source project hosted on Github, [Marlin](https://marlinfw.org/) is owned and maintained by the maker community.  

VS Code is an IDE program owned and maintained by Microsoft.

## Disclaimer  

THIS FIRMWARE AND ALL OTHER FILES IN THE DOWNLOAD ARE PROVIDED FREE OF CHARGE WITH NO WARRANTY OR GUARANTEE. SUPPORT IS NOT INCLUDED JUST BECAUSE YOU DOWNLOADED THE FIRMWARE. WE ARE NOT LIABLE FOR ANY DAMAGE TO YOUR PRINTER, PERSON, OR ANY OTHER PROPERTY DUE TO USE OF THIS FIRMWARE. IF YOU DO NOT AGREE TO THESE TERMS THEN DO NOT USE THE FIRMWARE.

## LICENSE
For the license, check the header of each file, if the license is not specified there, the project license will be used. Marlin is licensed under the GPL.
