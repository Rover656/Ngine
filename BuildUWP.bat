@echo off

SETLOCAL EnableDelayedExpansion

for /f "usebackq tokens=*" %%i in (`redist\vswhere.exe -version 15.0 -property installationPath`) do (
    set InstallDir=%%i
)

if exist "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat" (
  call "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
	mkdir build-UWP
	cd build-UWP

    REM X86
	mkdir x86
	cd x86
	cmake ../.. -G "Visual Studio 15 2017" -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
	cd ../

	REM X64
	mkdir x64
	cd x64
	cmake ../.. -G "Visual Studio 15 2017 Win64" -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
	cd ../

	REM ARM
    mkdir ARM
    cd ARM
    cmake ../.. -G "Visual Studio 15 2017 ARM" -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
    cd ../
) else (
  echo "Could not find !InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
)