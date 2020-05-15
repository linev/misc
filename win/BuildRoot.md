# Building ROOT on Windows

## Calling cmake

## For VirtualBox pathes
    set PATH=%PATH%;C:\Qt\5.12.6\msvc2017\bin
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\root -Droot7=ON -Dpython=OFF -DCMAKE_CXX_STANDARD=14 -Dwebgui=ON -DQt5_DIR=C:\Qt\5.12.6\msvc2017\lib\cmake -Dqt5web=ON

## For GSI Windows with many limitations
    set PATH=%PATH%;C:\Programme\Qt\5.12.6\msvc2017\bin
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\Users\linev\git\root -Droot7=ON -Dpython=OFF -DCMAKE_CXX_STANDARD=14 -Dwebgui=ON -DQt5_DIR=C:\Programme\Qt\5.12.6\msvc2017\lib\cmake -Dqt5web=ON

To get some debug info:

    -DCMAKE_BUILD_TYPE=Debug, probably -DLLVM_BUILD_MODE=Debug


## Start building

    cmake --build . --config Release -- /maxcpucount
    cmake --build . --config Debug -- /maxcpucount

## Initialize ROOT

    call C:\Programme\root\bin\thisroot.bat
    set PATH=%PATH%;C:\Qt\5.12.6\msvc2017\bin

## Debug ROOT

    devenv /debugexe root.exe ...


## Initialize cygwin

Add (or create new) Cygwin.bat file with MSVC initialization

    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

Remove link.exe file from cygwin to avoid conflict with the MS linker




# Building go4 with cmake on Windows

On VirtualBox machine do following - start VC x86 console

    set PATH=%PATH%;C:\Qt\5.12.6\msvc2017\bin
    call C:\Soft\root\bin\thisroot.bat

When calling cmake:

     cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\go4 -DCMAKE_CXX_STANDARD=14 -DQt5_DIR=C:\Qt\5.12.6\msvc2017\lib\cmake

When building:

    cmake --build . --config Release -- /maxcpucount
    cmake --build . --config Debug -- /maxcpucount

