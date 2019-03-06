# Build script for openui5 package

Try to extract minimal possible set of openui5 libraries, used in ROOT

1. Download and unpack runtime tarball from openui5 website: https://openui5.org/download.html
   Tested with version 1.63.0

2. Run provided shell script:

    [shell] ./build.sh /path/to/unpacked/openui5

3. Move and unpack produced tarball in $JSROOTSYS/openui5 directory:

    [shell] mv openui5.tag.gz $JSROOTSYS/openui5
    [shell] cd $JSROOTSYS/openui5
    [shell] tar xzf openui5.tar.gz

4. Add entry with mid5 checksum into cmake/modules/SearchInstalledSoftware.cmake

    [shell] sha256sum openui5.tar.gz
    