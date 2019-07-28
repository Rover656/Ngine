@echo off

SETLOCAL EnableDelayedExpansion

for /f "usebackq tokens=*" %%i in (`redist\vswhere.exe -version 15.0 -property installationPath`) do (
    set InstallDir=%%i
)

if exist "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat" (
  call "!InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
	mkdir build-UWP
	cd build-UWP
	cmake .. -G "Visual Studio 15 2017" -DPLATFORM=UWP -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

	for /f %%i in ('dir /b Configure\') do powershell -File Configure\%%i
) else (
  echo "Could not find !InstallDir!\VC\Auxiliary\Build\vcvars64.bat"
)