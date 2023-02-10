standalone/README.md# Visual Pinball Standalone

*An open source **cross platform** pinball table simulator.*

This project is a modified version of [VPinballX](https://github.com/vpinball/vpinball) that is designed to run on non-Windows platforms.

[![Watch the video](https://img.youtube.com/vi/jK3TbGvTuIA/0.jpg)](https://www.youtube.com/watch?v=xjkgzIVL_QU)

## Background

Visual Pinball was built over 20 years ago using Microsoft technologies such as OLE Compound Documents, DirectX, VBScript, COM, and ATL -- many of which have no cross platform equivalents or support.

To make a successful port, we would need tackle several tasks:

> ***OLE Compound Documents***
> - Visual Pinball Table (`.vpx`) files are stored in [Compound File Binary Format](https://en.wikipedia.org/wiki/Compound_File_Binary_Format).
>
> **Solution**
> - Implement [POLE - portable library for structured storage](https://dimin.net/software/pole/) to load `.vpx` files.
> - Create `PoleStorage` wrapper to match `IStorage` interface.
> - Create `PoleStream` wrapper to match `IStream` interface.

> ***Windows Registry***
> - Visual Pinball stores and retrieves its settings from the Windows registry.
>
> **Solution**
> - Use the `ENABLE_INI` preprocessor definition which uses [RapidXML](https://github.com/timniederhausen/rapidxml) to save and retrieve settings from a file.

> ***VBScript***
> - Visual Pinball uses [VBScript](https://learn.microsoft.com/en-us/previous-versions/t0aew7h6(v=vs.85)) as the scripting engine for tables.
>
> **Solution**
> - Leverage the VBScript engine from [Wine](https://github.com/wine-mirror/wine/tree/master/dlls/vbscript).
> - Fix bugs in Wine's VBScript engine:
>   - [Bug 53644](https://bugs.winehq.org/show_bug.cgi?id=53644) - vbscript can not compile classes with lists of private / public / dim declarations
>   - ~~[Bug 53670](https://bugs.winehq.org/show_bug.cgi?id=53670) - vbscript can not compile if expressions with reversed gte, lte, (=>, =<)~~
>   - ~~[Bug 53676](https://bugs.winehq.org/show_bug.cgi?id=53676) - vbscript can not exec_script - invalid number of arguments for Randomize~~
>   - ~~[Bug 53678](https://bugs.winehq.org/show_bug.cgi?id=53678) - vbscript can not compile CaseClausules that do not use a colon~~
>   - ~~[Bug 53766](https://bugs.winehq.org/show_bug.cgi?id=53766) - vbscript fails to handle SAFEARRAY assignment, access, UBounds, LBounds~~
>   - [Bug 53767](https://bugs.winehq.org/show_bug.cgi?id=53767) - vbscript fails to handle ReDim when variable is not yet created
>   - ~~[Bug 53782](https://bugs.winehq.org/show_bug.cgi?id=53782) - vbscript can not compile ReDim with list of variables~~
>   - ~~[Bug 53783](https://bugs.winehq.org/show_bug.cgi?id=53783) - vbscript can not compile private const expressions~~
>   - ~~[Bug 53807](https://bugs.winehq.org/show_bug.cgi?id=53807) - vbscript fails to redim original array in function when passed byref~~
>   - [Bug 53844](https://bugs.winehq.org/show_bug.cgi?id=53844) - vbscript invoke_vbdisp not handling let property correctly for VT_DISPATCH arguments
>   - ~~[Bug 53866](https://bugs.winehq.org/show_bug.cgi?id=53866) - vbscript fails to handle SAFEARRAY in for...each~~
>   - ~~[Bug 53867](https://bugs.winehq.org/show_bug.cgi?id=53867) - vbscript fails to retrieve property array by index~~
>   - ~~[Bug 53868](https://bugs.winehq.org/show_bug.cgi?id=53868) - vbscript fails to return TypeName for VT_DISPATCH~~
>   - ~~[Bug 53873](https://bugs.winehq.org/show_bug.cgi?id=53873) - vbscript fails to compile Else If when If is on same line~~
>   - [Bug 53877](https://bugs.winehq.org/show_bug.cgi?id=53877) - vbscript compile_assignment assertion when assigning multidimensional array by indices
>   - ~~[Bug 53888](https://bugs.winehq.org/show_bug.cgi?id=53888) - vbscript does not allow Mid on non VT_BSTR~~
>   - [Bug 53889](https://bugs.winehq.org/show_bug.cgi?id=53889) - vbscript does not support Get_Item call on IDispatch objects
>   - [Bug 54221](https://bugs.winehq.org/show_bug.cgi?id=54221) - vbscript: missing support for GetRef
>   - [Bug 54234](https://bugs.winehq.org/show_bug.cgi?id=54234) - vbscript fails to compile when colon follows Else in If...Else
>   - [Bug 54291](https://bugs.winehq.org/show_bug.cgi?id=54291) - vbscript stuck in endless for loop when UBound on Empty and On Error Resume Next
>   - [Bug 54456](https://bugs.winehq.org/show_bug.cgi?id=54456) - vbscript memory leak in For Each with SafeArray as group
>   - ~~[Bug 54457](https://bugs.winehq.org/show_bug.cgi?id=54457) - vbscript memory leaks in interp_redim_preserve~~
>   - ~~[Bug 54458](https://bugs.winehq.org/show_bug.cgi?id=54458) - vbscript memory leaks in Global_Split~~
> - Add support for `Scripting.FileSystemObject` and `Scripting.Dictionary` leveraging Wine's `scrrun` code.
> - Add support for `E_NOTIMPL` commands to Wine's VBScript engine:
>   - `ExecuteGlobal`
>   - `Eval`

> ***COM / ATL***
> - Visual Pinball uses [COM](https://learn.microsoft.com/en-us/windows/win32/com/the-component-object-model) as a bridge between the scripting engine and itself. The scripting engine uses COM to access other components such as [VPinMAME](https://github.com/vpinball/pinmame) and [B2S.Server](https://github.com/vpinball/b2s-backglass).
> - Visual Pinball uses [ATL](https://learn.microsoft.com/en-us/cpp/atl/active-template-library-atl-concepts?view=msvc-170) to dispatch events from itself to the scripting engine, ex: `DISPID_GameEvents_Init` and `DISPID_HitEvents_Hit`
>
> **Solution**
> - Leverage just enough Wine source code to set up the platform to match Windows, ex: `wchar_t` is 2 bytes in Windows while 4 bytes on most other platforms. (`_WINE_UNICODE_NATIVE_`)
> - Provide stubs for as much COM and ATL code as possible by using Wine and [ReactOS](https://github.com/reactos/reactos/tree/master/sdk/lib/atl) source code.
> - Embed Wine's VBScript engine instead of trying to replicate Windows [RPC](https://learn.microsoft.com/en-us/windows/win32/rpc/rpc-start-page).
> - Replace Windows `typelib` magic with class methods for `GetIDsOfNames`, `Invoke`, `FireDispID`, and `GetDocumentation`
> - Create two `idlparser` utilities that read [Interface Definition File](https://learn.microsoft.com/en-us/windows/win32/midl/interface-definition-idl-file) and automatically generates C and C++ code for `GetIDsOfNames`, `Invoke`, `FireDispID`, and `GetDocumentation`
> - Embed `libpinmame` to provide support for PinMAME based games. [Future]

> ***Graphics***
> - Visual Pinball uses either DirectX or OpenGL for graphics.
>
> **Solution**
> - MacOS uses OpenGL 4.1.
> - Linux (Ubuntu 22.04) uses OpenGL 4.6.
> - Android, iOS, tvOS, and Raspberry PI use OpenGLES 3.0.

> ***Sound***
> - Visual Pinball uses DirectSound for playing `.wav` files and [BASS](http://www.un4seen.com/) for playing all other audio files.
>
> **Solution**
> - Use the `ONLY_USE_BASS` preprocessor definition which uses BASS for all audio files including `.wav` files.

> ***Input***
> - Visual Pinball uses XInput and either DirectInput 8 or SDL2 for getting keyboard and joystick info.
>
> **Solution**
> - Make a `SDLK_TO_DIK` translation table that converts SDL Keys to DirectInput Keys.
> - Add additional support for `SDL_KEYUP` and `SDL_KEYDOWN` events.

> ***Updating***
> - The Visual Pinball repository is constantly updated with new features and bug fixes.
>
> **Solution**
> - Updates must be easy to rebase, so all changes must be wrapped in:
>
>   ```
>   #ifdef __STANDALONE__
>   #endif
>   ```
>   or
>   ```
>   #ifndef __STANDALONE__
>   #else
>   #endif
>   ```

## Compiling

### MacOS (arm64)

In a terminal execute the following:
```
brew install cmake bison curl
export PATH="$(brew --prefix bison)/bin:$PATH"
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/macos
./external.sh
cd ../..
cp cmake/CMakeLists_gl-macos-arm64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

*Note:* Make sure `SDL2`, `SDL2_ttf`, and `freeimage` are not installed by `brew`, otherwise you may have compile issues

### iOS (arm64)

In a terminal execute the following:
```
brew install cmake bison curl ios-deploy
export PATH="$(brew --prefix bison)/bin:$PATH"
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/ios
./external.sh
cd ../..
cp cmake/CMakeLists_gl-ios-arm64.txt CMakeLists.txt
cmake -G Xcode -B XCode
open XCode/vpinball.xcodeproj
```

### tvOS

In a terminal execute the following:
```
brew install cmake bison curl ios-deploy
export PATH="$(brew --prefix bison)/bin:$PATH"
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/tvos
./external.sh
cd ../..
cp cmake/CMakeLists_gl-tvos-arm64.txt CMakeLists.txt
cmake -G Xcode -B XCode
open XCode/vpinball.xcodeproj
```

*Note:* BASS for tvOS is not publicly available for open source projects. You will need to buy an iOS license. More information can be found [here](https://www.un4seen.com/).

### Android (arm64-v8a)

```
brew install cmake bison curl
export PATH="$(brew --prefix bison)/bin:$PATH"
export JAVA_HOME=$(/usr/libexec/java_home -v 11.0.16.1)
export ANDROID_HOME=/Users/jmillard/Library/Android/sdk
export ANDROID_NDK=/Users/jmillard/Library/Android/sdk/ndk/25.1.8937393
export ANDROID_NDK_HOME=/Users/jmillard/Library/Android/sdk/ndk/25.1.8937393
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/android
./external.sh
cd ../..
cp cmake/CMakeLists_gl-android-arm64-v8a.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
cd standalone/android/android-project
./gradlew installDebug
$ANDROID_HOME/platform-tools/adb shell am start -n org.vpinball.app/org.vpinball.app.VPinballActivity
$ANDROID_HOME/platform-tools/adb -d logcat org.vpinball.app
```

### Linux (Ubuntu 22.04)

In a terminal execute the following:
```
sudo apt install git build-essential cmake bison curl
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/linux
./external.sh
cd ../..
cp cmake/CMakeLists_gl-linux-x64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

### Linux (Fedora 37)

In a terminal execute the following:
```
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc-c++ cmake bison curl
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/linux
./external.sh
cd ../..
cp cmake/CMakeLists_gl-linux-x64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

### Raspberry Pi 4

Start with a [Raspberry Pi OS Lite (64-Bit)](https://www.raspberrypi.com/software/operating-systems/#raspberry-pi-os-64-bit) image and execute the following:
```
sudo apt-get install git cmake bison zlib1g-dev libdrm-dev libgbm-dev libgles2-mesa-dev libudev-dev libgles2-mesa
git clone -b standalone https://github.com/vpinball/vpinball
cd vpinball/standalone/rpi
./external.sh
cd ../..
cp cmake/CMakeLists_gl-rpi-aarch64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

## Running

Go to the build directory:
```
cd build
```

To launch a table, execute the following:
```
./VPinballX_GL -play <table.vpx>
```

To launch a table in windowed mode, execute the following:
```
./VPinballX_GL -DisableTrueFullscreen -play <table.vpx>
```

To extract a table script, execute the following:
```
./VPinballX_GL -extractvbs <table.vpx>
```

## Joystick

The joystick is currently mapped to an XBox controller layout.

- Left Flipper: `Left Shoulder`
- Right Flipper: `Right Shoulder`
- Center Tilt: `D-pad Up`
- Left Title: `D-pad Left`
- Right Tilt: `D-pad Right`
- Plunger: `D-pad Down`
- Add Credit: `A`
- Start: `B`
- FPS: `X`
- Cancel: `Y`
- Fire: `Left Stick`

## Debugging

Debugging can be done using [Visual Studio Code](https://code.visualstudio.com/).

### MacOS

Perform the steps outlined above in *Compiling* and *Running*.

In Visual Studio Code:
  - Install the [`C/C++ Extension Pack`](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) extension.
  - Open the `vpinball` folder.
  - If prompted, select the latest version of clang, ex: `Clang 14.0.0 arm64-apple-darwin21.6.0`
  - Go to `Settings` -> `CMake: Debug Config` and click `Edit in settings.json`
  - Update `settings.json` with:
  ```
      "cmake.debugConfig": {
         "args": [ "-DisableTrueFullscreen", "-Play", "res/exampleTable.vpx" ],
      }
  ```
  - Click the bug button (to the left of the play button) in the bottom bar

### Raspberry Pi 4

Perform the steps outlined above in *Compiling* and *Running*.

Since we are using the Lite version of Raspberry Pi OS, there is no X11 to run the full version Visual Studio Code. Instead will we use the [Remote Development](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack) extension in Visual Studio Code running on a host computer.

**NOTE**: The 2GB version of the Raspberry Pi 4 can easily run out of memory and become unresponsive.

Make sure SSH is configured and enabled on your Raspberry Pi.

On the host computer, in Visual Studio Code:
  - Install the [`Remote Development`](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack) extension.
  - Select `Remote-SSH: Connect to Host...` from the Command Palette (F1, ⇧⌘P)
  - Enter the `user@address` of the Raspberry Pi.
  - Once connected, install the [`C/C++ Extension Pack`](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) extension.
  - If prompted, select the latest version of gcc, ex: `GCC 10.2.1 aarch64-linux-gnu`
  - Go to `Settings` (`Remote SSH`) -> `CMake: Debug Config` and click `Edit in settings.json`
  - Update `settings.json` with:
  ```
    "cmake.parallelJobs": 1,
    "cmake.generator": "Unix Makefiles",
    "cmake.debugConfig": {
        "args": [ "-Play", "res/exampleTable.vpx" ],
    }
  ```
  - Click the bug button (to the left of the play button) in the bottom bar

### Change Log

To keep up with all the changes in master, and make it easier to rebase, this branch is a single commit -- until most tables work with no vbs changes.

The downside of this approach is not accurately keeping track of history:

* 02/09/23
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/2141) Fix memory leak in owned safearray iterator
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/2142) Fix compile when colon follows Else on new line

* 02/08/23
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/2132) Fix memory leak in interp_redim_preserve
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/2131) Fix memory leak in Split()

* 02/07/23
  * Wine hack to fix memory leak with "owned" `SAFEARRAY` in `interp_newenum` and `interp_enumnext`
  * Add `EXTERNAL_DIR` and `APK` settings to `vpxm` script

* 02/06/23
  * Wine hack to fix memory leak in `interp_redim_preserve`

* 02/04/23
  * Fix several memory leaks (`GetRef` and `GlobalSplit`)
  * Fix issue where `CComVariant` would release additional reference after `Detach` in `Close`
  * Added an `AddRef` to `ITypeInfoImpl` which finally fixes deleting objects when ref count = 0!
  * Fixed Linux buffer overflows in `Global_FormatNumber`

* 01/31/23
  * Add `vpxm` script and move `vpx` script to `standalone/scripts`

* 01/29/23
  * Rework IDL generator to use binary search trees for `GetIDsOfNames` and `FireDispID`

* 01/28/23
  * Stub more of `IPinUpDisplay` interface

* 01/27/23
  * Start to implement `IPinUpDisplay` interface
  * Reorganize standalone includes from `/inc` to `/standalone/inc`

* 01/26/23
  * Wine hack to support `typename` for class objects. (Fixes lamps in BM)
  * Update Android to use SDL 2.26.2

* 01/25/23
  * More FlexDMD work (BM is alive!)
  * Replace `VariantCopy` with `VariantCopyInd` in proxies. (Fixes BM `swLeft/RightOrbTrigger2.uservalue` issues)
  * Wine hack to implement missing `GetLocale` and `SetLocale` functions
  * Tweak Game Controller support for iOS
  * Update iOS to use SDL 2.26.2

* 01/24/23
  * Add script to generate code from IDL files using WIDL
  * Rearrange some directories and clean up VPinMAME and WMP 
  * Start work on implementing FlexDMD

* 01/21/23
  * Finished `WMPCore`, `WMPSettings`, and `WMPControls` (BM has working music)

* 01/19/23
  * More work on `WMPCore`, `WMPSettings`, and `WMPControls` interfaces

* 01/18/23
  * Skip detecting Siri Remote as `SDL_GameController` in tvOS
  * Set `MaxTexDimension` to `2048` and disable HIGHDPI for tvOS (Fixes AC/DC)
  * Disable Wine `TRACE` and `DEBUG` messages
  * More work on `WMPCore` interfaces

* 01/17/23
  * Wine `ElseIf` hack to not require `NL` after `Then` (Fixed AC/DC)
  * Start to implement `WMPCore`, `WMPControls`, and `WMPSettings`
  * Switch `SDL_Joystick` to `SDL_GameController`
  * Implement `SDL_GameController` rumble support
  * Remapped `Start` and `Cancel` for iOS and tvOS

* 01/16/23
  * Fix `RtlCompareUnicodeStrings` to properly compare wide strings with specified lengths
  * Wine `invoke_variant_prop` hack to support getting array variables in classes, ie: `If Lampz.IsLight(5) = true`
  * Wine `assign_value_script_ctx` hack to support setting array variables in classes, ie: `Lampz.IsLight(5) = true`
  * Implement `tvOS` version
  * Update joystick to match XBox controller layout

* 01/13/23
  * Wine `Global_CreateObject` hack to return `Nothing` instead of error when `CreateObject` fails. (Most scripts check for `Nothing`)
  * Cache `BASS_ATTRIB_FREQ`. (Partially fixes [#224](https://github.com/vpinball/vpinball/issues/224)) 
  * Attempt to fix BASS looping and restarts. (Partially fixes [#224](https://github.com/vpinball/vpinball/issues/224)) 
  * Cleanup `vpx` shell script. (Add to your path to run `VPinballX_GL` from anywhere)
  * Created an IDL enum parser to add enums to `ScriptGlobalTable`, ie: `SequencerState` (support VPin Workshop tables)
  * Updated Basic shader to fix reflections on OpenGLES (Thanks @Niwak!)
  * Fixed `get_ChangedLamps` to return modern intensity-levels for `lampNo > 80` (Fixes GI in TWD!)

* 01/12/23
  * Wine `do_icall` hack to support `GetRef` variable calls with no params, ie: `MotorCallback` (Fixes flippers in TWD)
  * Wine `is_matching_key` hack to support `VT_VARIANT|VT_BYREF`

* 01/11/23
  * Wine scrrun `dictionary_get_HashVal` hack to return hash value for `VT_VARIANT|VT_BYREF` (TWD now plays!)
  * Wine vbscript `UBound`, and `LBound` hack to return `0` when `VT_EMPTY` (MM now plays with zero script changes!)
  * Added additional logging for `Eval`, `Execute`, `ExecuteGlobal`

* 01/10/23
  * Redesign `GetRef` again, this time as IDispatch (T2 now plays with zero script changes!)
  * Added delay to make sure PinMAME is running before trying to initialize switches. (CFTBL now runs with zero script changes!)
  * Reworked wine debug logging to use `plog`

* 01/09/23
  * Implement `VPinMAMEController` mechs

* 01/08/23
  * More work on `IGames`, `IGame`, and `IGameSettings` (Rock now works with zero script changes!)

* 01/07/23
  * Stub VPinMAME interfaces (ie, `IRom`, `IRoms`)
  * Add new `libpinmame` methods for setting `SolMask` and `GetLEDs` (Rock renders Alpha Numeric display)
   
* 01/03/23
  * Fix RPI4 `libsdl2-2.0.so.0` library `SONAME`

* 01/02/23
  * Remove `RPATH` and replace with `$ORIGIN` for Linux/RPI builds
  * Replace `RGB_FP16` with `RGBA_FP16` for OpenGLES builds (RPI4 finally renders!)

* 01/01/23
  * Happy New Year!
  * Work on RPI4 builds

* 12/31/22
  * Fixed startup issues when calling `PinmameGetChangedGIs`/`PinmameGetChangedLamps` before PinMAME is fully running
  * Fixed `VPinMAMEController::get_Version` `BSTR` allocation
  * Fixed non-defaulted `m_capExtDMD` which lead to crashes or incorrect DMD Shader assignment

* 12/30/22
  * Work on Linux builds

* 12/29/22
  * Fix `get_ChangedLamps` to support libpinmame `255` state 
  * Wine vbscript hack to allow statement separators after `ELSE` (fixes Cuphead)
  * Replaced `SDL_Log` with `plog` (`plog` moved to root directory)
  * Updated Android builds to recursively copy assets (ie to copy `pinmame/roms`)

* 12/28/22
  * Rework `GetRef` to fix `vpmCreateBall`
  * Convert `mQue` in `core.vbs` to multiple single dimension arrays
  * Updated `IDLParserToCpp` to use `VariantCopyInd` for `VARIANT` params
  * Update `Eval`, `Execute`, and `ExecuteGlobal` to convert `arg` to `BSTR` using `to_string`
  * More work on `VinMAMEController` (Rock/T2 partially working)

* 12/22/22
  * Rework macos builds to use external.sh and build SDL2, SDL2_ttf, FreeImage, and libpinmame
  * Fix music directory to be cross platform

* 12/21/22
  * Start to implement VPinMAMEController

* 12/20/22
  * Attempt to fix HIGHDPI rendering issues with decals and lights
  * Add shim to allow `controller.vbs` and `Controller.vbs` in `GetTextFile` for case sensitive filesystems
  * Update `check_script_collisions` to allow multiple `Dim`s of `B2SOn` and `Controller` in `ExecuteGlobal` (Fixes Volley)

* 12/17/22
  * Reworked `GetMyPath` to use `SDL_GetBasePath()`
  * Fixed repeating `SDL_KEYUP` and `SDL_KEYDOWN` events in Android build
  * Updated CI to make Android builds

* 12/15/22
  * Fixed several GLES 3.0 rendering issues
  * Added support for retina displays on MacOS and iOS via `SDL_WINDOW_ALLOW_HIGHDPI`

* 12/12/22
  * Added `android-project`
  * Cleaned up Android build instructions
  * Replaced `printf` and `std::cout` with `SDL_Log`

* 12/08/22
  * Started working on Android native library builds

* 12/03/22
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1561) Accept private and public const global declarations.
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1611) Handle "case" statements without separators.
  * Rework Wine debug stubs to fix strdup memory leak 
  * Update shader #defines to support OpenGLES compilation 
  * Replace shader integers with floats to support OpenGLES compilation 
  
* 11/27/22
  * OpenGLES updates to allow iOS app to run - working imgui, sound, input, and mouse support.

* 11/24/22
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1534) Handle another variant of LTE/GTE tokens.

* 11/23/22
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1495) Add support for redim byref. 
  * Add support for iOS simulator build (arm64 only)
  * Skip rendering when OpenGLES so app does not crash (plays, but no graphics)

* 11/20/22
  * `#ifndef __OPENGLES__` around code not available in OpenGLES
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1409) Handle index read access to array properties
  * More work on iOS build

* 11/16/22
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1385) Wine vbscript `Else` new line fix
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1368) Use CRT allocation functions
  * Start to work on iOS build
  * Partial Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1391) Implement Format functions.
  * Rearrange CMake search paths and fix SDL2 includes for Linux
  * Move RapidXML to separate folder

* 11/14/22
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1347) Wine vbscript `Global_TypeName` fix
  * Update to glad 2
  * Add Linux x64 build to github workflow

* 11/12/22
  * Wine scrrun hack to allow dictionary hash values for `VT_VARIANT|VT_BYREF`.
  * Remove vbscript hack to allow `VT_SAFEARRAY` and fix idl parser to use `VT_VARIANT|VT_ARRAY`.
  * Remove vbscript `interp_newenum` hack support `for each...next` with `SAFEARRAY`
  * Rearrange `main_standalone.h` to use `GetDocumentation` in `InternalAddRef` and `InternalRelease`
  * Temporarily disable delete in `Release` until tracking down extra `InternalRelease`
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1328) Wine vbscript `Redim` lists fix
  * Implement [offical](https://gitlab.winehq.org/wine/wine/-/merge_requests/1288) Wine vbscript `Global_Mid` fix
  * Implement [offical](https://bugs.winehq.org/attachment.cgi?id=73461&action=diff) Wine vbscript `Global_TypeName` patch
  * Remove saving width and height in `VPinballX.ini` when exiting table

* 11/07/22
  * Wine vbscript hack to allow `Redim` lists
  * Wine vbscript `Global_FormatNumber` hack add `FormatNumber` support
  * Wine vbscript `Global_Mid` hack to allow `Mid` on non VT_BSTR
  * Wine vbscript `do_icall` hack to support `Get_Item` in `Collection` (Fixes Road Race)

* 11/04/22
  * Implement [official](https://gitlab.winehq.org/wine/wine/-/merge_requests/1246) Wine vbscript `Global_Rnd` and `Global_Randomize` fixes 
  * Wine vbscript `Global_Eval` hack to fix `IsEmpty(Eval("BallSize"))`
  * Wine vbscript `Global_GetRef` hack to workaround `Set GICallback = GetRef("NullSub")`
  * Wine vbscript `parser.y` hack to allow `Private Const`
  * Wine vbscript `parser.y` hack to allow `Else If` on same line (Fixes Four Million B.C.)
  * Wine vbscript `parser.y` hack to allow `Case Else` without colon
  * Trim whitespace in `VPinballX.ini` and save player options when exiting table

* 11/02/22
  * First working non-example table with no vbscript errors (Grand Tour)
  * Wine vbscript `Global_TypeName` hack to fix `if TypeName(balls(x)) = "IBall" then` 
  * Wine vbscript `invoke_variant_prop` hack to support array access `aObj.ModIn(x)`
  * Wine vbscript `interp_newenum` hack to support `for each...next` with `SAFEARRAY`
  * Implement `GetIDsOfNames` and `Invoke` for `Scripting.Dictionary` in Wine `scrrun`
  * Implement `GetIDsOfNames` and `Invoke` for `Scripting.FileSystemObject` in Wine `scrrun`
  * Implement stubs for Wine `ReadFile`, `WriteFile`, `GetFullPathNameW`, `GetFileAttributesW`, `CreateFileW` so tables can read and write data
  * Rework `Invoke` for voinball interfaces to not use `CComVariant` wrappers
  * Several updates to `IDLParserToC` and `IDLParserToCpp` including adding `GetDocumentation` to support `TypeName`
  * Do not define `_rtol` in Linux

### Workarounds

Some scripts assign values to specific indicies of multi-dimensional arrays. For example:

```
DTArray(i)(4) = DTAnimate(DTArray(i)(0),DTArray(i)(1),DTArray(i)(2),DTArray(i)(3),DTArray(i)(4))
```

Currently the Wine VBScript engine does not support this, and unfortunately, many tables use this style for `DTArray`.

As a workaround, I suggest the following:

1) Add the following code:

```
Dim DTArray0, DTArray1, DTArray2, DTArray3, DTArray4
Redim DTArray0(UBound(DTArray)), DTArray1(UBound(DTArray)), DTArray2(UBound(DTArray)), DTArray3(UBound(DTArray)), DTArray4(UBound(DTArray))

Dim DTIdx
For DTIdx = 0 to UBound(DTArray)
   DTArray0(DTIdx) = DTArray(DTIdx)(0)
   DTArray1(DTIdx) = DTArray(DTIdx)(1)
   DTArray2(DTIdx) = DTArray(DTIdx)(2)
   DTArray3(DTIdx) = DTArray(DTIdx)(3)
   DTArray4(DTIdx) = DTArray(DTIdx)(4)
Next
```

2) Search and replace:

- `DTArray(i)(0)` to `DTArray0(i)`
- `DTArray(i)(1)` to `DTArray1(i)`
- `DTArray(i)(2)` to `DTArray2(i)`
- `DTArray(i)(3)` to `DTArray3(i)`
- `DTArray(i)(4)` to `DTArray4(i)`

### Miscellaneous

To build header file for VPinMAME.idl

```
brew install llvm llvm-mingw
git clone -b wine-7.20 git://source.winehq.org/git/wine.git
git clone git@github.com:vpinball/pinmame.git
cd wine
export MACOSX_DEPLOYMENT_TARGET="10.14"
export LDFLAGS="-Wl,-rpath,/opt/X11/lib"
export PATH="$(brew --prefix llvm)/bin:$PATH"
export PATH="$(brew --prefix bison)/bin:$PATH"
./configure --without-freetype
make -j10
tools/widl/widl -o ../vpinmame_i.h --nostdinc -Ldlls/\* -Iinclude -D__WINESRC__ -D_UCRT ../pinmame/src/win32com/VPinMAME.idl
```

*Note:* On ARM based MacOS, use [Wine 7.20](https://gitlab.winehq.org/wine/wine/-/tree/wine-7.20) as [Wine 7.21](https://gitlab.winehq.org/wine/wine/-/tree/wine-7.21) seg faults during compile and [Wine 7.22]((https://gitlab.winehq.org/wine/wine/-/tree/wine-7.22) errors with `PE cross-compilation is required for ARM64` during configure.

### Shoutouts

- Wine and the amazing people who support the VBScript engine
  - Robert Wilhelm
  - Nikolay Sivov

- The people who make this such an exciting hobby to be a part of
  - @Bord, @Cupiii, @ecurtz, @freezy, @Iaaki, @Lucidish, @mkalkbrenner, @Niwak, @Scottacus64, @Thalamus, @toxie
  - and the rest of the Visual Pinball community!
