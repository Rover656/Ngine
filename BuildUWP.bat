@echo off

SETLOCAL EnableDelayedExpansion

for /f "usebackq tokens=*" %%i in (`vendor\windows-redist\vswhere.exe -property installationPath`) do (
   set InstallDir=%%i
)

if exist "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat" (
  call "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
	mkdir build-UWP
	cd build-UWP

    REM X86
	mkdir x86
	cd x86
	cmake ../.. -G "Visual Studio 16 2019" -A Win32 -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_CONFIGURATION_TYPES=Release;Debug
	cd ../

	REM X64
	mkdir x64
	cd x64
	cmake ../.. -G "Visual Studio 16 2019" -A x64 -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_CONFIGURATION_TYPES=Release;Debug
	cd ../

	REM ARM
    mkdir ARM
    cd ARM
    cmake ../.. -G "Visual Studio 16 2019" -A ARM -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_CONFIGURATION_TYPES=Release;Debug
    cd ../
) else (
  echo "Could not find !InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
)