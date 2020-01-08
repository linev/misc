# Building ROOT on Windows

## Calling cmake

    set PATH=%PATH%;C:\Programme\Qt\5.12.6\msvc2017\bin
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\Users\linev\git\root -Droot7=ON -Dpython=OFF -DCMAKE_CXX_STANDARD=14 -Dwebgui=ON -DQt5_DIR=C:\Programme\Qt\5.12.6\msvc2017\lib\cmake -Dqt5web=ON


## Start building

    cmake --build . --config Release -- /maxcpucount

## Initialize ROOT

    call C:\Programme\root\bin\thisroot.bat
    set PATH=%PATH%;C:\Programme\Qt\5.12.6\msvc2017\bin
