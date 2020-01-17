# Building ROOT on Windows

## Calling cmake

    set PATH=%PATH%;C:\Qt\5.12.6\msvc2017\bin
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\root -Droot7=ON -Dpython=OFF -DCMAKE_CXX_STANDARD=14 -Dwebgui=ON -DQt5_DIR=C:\Qt\5.12.6\msvc2017\lib\cmake -Dqt5web=ON


## Start building

    cmake --build . --config Release -- /maxcpucount

## Initialize ROOT

    call C:\Programme\root\bin\thisroot.bat
    set PATH=%PATH%;C:\Qt\5.12.6\msvc2017\bin

## Initialize cygwin

Add (or create new) Cygwin.bat file with MSVC initialization

    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

Remove link.exe file from cygwin to avoid conflict with the MS linker
