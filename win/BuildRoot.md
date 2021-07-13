# Building ROOT on Windows

## Call cmake for VirtualBox pathes

    set PATH=%PATH%;C:\Qt5\5.15.2\msvc2019\bin
    set CEF_ROOT=C:\Soft\cef
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\root -Droot7=ON -Dpython=OFF -Dtesting=ON -DCMAKE_CXX_STANDARD=17 -Dwebgui=ON -Dqt5web=ON

    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\root624 -Dpython=OFF -Dtesting=ON -DCMAKE_CXX_STANDARD=14 -Dwebgui=ON -Dqt5web=ON


## Call cmake for GSI Windows with many limitations

    set PATH=%PATH%;C:\Programme\Qt\5.12.6\msvc2017\bin
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\Users\linev\git\root -Droot7=ON -Dpython=OFF -DCMAKE_CXX_STANDARD=17 -Dwebgui=ON -Dqt5web=ON

To get some debug info:

    -DCMAKE_BUILD_TYPE=Debug, probably -DLLVM_BUILD_MODE=Debug


## Start building

    cmake --build . --config Release -- /maxcpucount

One also can try to make Debug build, but it may not work because of external libs

    cmake --build . --config Debug -- /maxcpucount

## Initialize ROOT

    call C:\Programme\root\bin\thisroot.bat
    set PATH=%PATH%;C:\Qt5\5.15.2\msvc2019\bin

## Debug ROOT

    devenv /debugexe root.exe ...


## Initialize cygwin (obsolete)

Add (or create new) Cygwin.bat file with MSVC initialization

    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

Remove link.exe file from cygwin to avoid conflict with the MS linker


# Building go4 with cmake on Windows

On VirtualBox machine do following - start VC x86 console

    set PATH=%PATH%;C:\Qt5\5.15.2\msvc2019\bin
    call C:\Soft\root624\bin\thisroot.bat
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 c:\git\go4
    cmake --build . --config Release -- /maxcpucount

# Create packages with cpack

First of all - install https://sourceforge.net/projects/nsis/
When after building ROOT and Go4, call for both:

    cpack -C Release

This should produce installers, which are half-ready for use.

# Create go4 windows package with cpack

1. Install and configure qt5
2. Build root with qt5web support
3. Create ROOT package and install it
4. Compile go4 with installed ROOT package
5. Finally call `cpack` for go4


# Building CEF on Windows

Downdlod win32 version, change /MT -> /MD compiler falgs for dynamic linking

    mkdir build
    cd build
    cmake -G"Visual Studio 16 2019" -A Win32 -Thost=x64 ..
    cmake --build . --config Release --target libcef_dll_wrapper

