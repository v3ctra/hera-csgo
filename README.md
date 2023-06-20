# Hera-csgo
[![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=plastic)](https://en.wikipedia.org/wiki/C%2B%2B) 
[![CS:GO](https://img.shields.io/badge/game-CS%3AGO-yellow.svg?style=plastic)](https://store.steampowered.com/app/730/CounterStrike_Global_Offensive/) 
![Windows](https://github.com/danielkrupinski/Osiris/workflows/Windows/badge.svg?branch=master&event=push)

Free and open-source cheat software for Counter-Strike: Global Offensive. Aimed at maximum security in order to avoid red-trustfactor issues.

## Features
 - **Aimbot** - aim assistance
 - **Triggerbot** - automatically fires when crosshair is on enemy
 - **SoundESP** - plays sound when crosshair is at aimbot's field-of-view

 ## Technical Details
  - Using handle-hijacking technique instead of creating an new one.
  
    The Software is accessing game's memory by hijacking an existing handle.<br>
    This reduces the possibility of being detected by game's anticheat.
    
- Designed to be hidden in another process's memory

  The cheat is designed to not have it's own separated process which may be exposed to anticheat.<br> The module is being injected into legitimate process, using the manual-mapping technique.

- Not writing to process's memory

  The cheat doesn't modify memory of game's process. <br>Mouse movements are being used instead of writing memory to view-angles, which is **detectable** by Valve Anti-Cheat.

- Using Syscalls instead of winapi calls.

  Using syscalls can provide higher performance and makes cheat harder to reverse-engineer.

## Getting Started
### Presequites
Microsoft Visual Studio 2019, platform toolset v143 and Windows SDK 10.0.x.x are required in order to compile project. 
### Compiling from source
Download and open hera-csgo.sln in Visual Studio, then change build configuration to:
 - Debug (x86) - In order to compile project as executable file, with debugging console.
 - Release (x86) - In order to compile an Dynamic-Link-Library, supposed to be injected into legitimate process (ex. Discord.exe)
and simply press Build solution.

## Acknowledgments
*   [Daniel Krupinski](https://github.com/danielkrupinski) for [x86RetSpoof](https://github.com/danielkrupinski/x86RetSpoof) and [hashing](https://github.com/danielkrupinski/Osiris/blob/master/Dependencies/fnv.h).
*   [open-source-parsers](https://github.com/open-source-parsers) for [jsoncpp](https://github.com/open-source-parsers/jsoncpp/tree/master)
*   Sebastien Andrivet for xorstr
