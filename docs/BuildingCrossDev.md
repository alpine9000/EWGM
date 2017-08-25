installing the cross development environment
============================================

Built on OSX should work on any unix environment with a bit of work.

Notes: 
   * My /usr/local is writable by me. You will probable need to add "sudo" to any "make install" lines
   * There are a bunch of Makefiles in devenv that might do all of this for you

0. build dir
    ```
    mkdir ~/project
    mkdir ~/project/downloads
    mkdir ~/project/build
    mkdir ~/project/repos
    mkdir /usr/local/amiga
    mkdir /usr/local/amiga/vgcc
    mkdir /usr/local/amiga/vgcc/bin
    mkdir /usr/local/amiga/vgcc/ndk
    mkdir /usr/local/amiga/vgcc/ndk/include
    mkdir /usr/local/amiga/vgcc/ndk/lib
    cd ~/project
```

1. autoconf
    ```
    cd downloads
    curl -OL http://ftpmirror.gnu.org/autoconf/autoconf-2.69.tar.gz
    cd ../build
    tar xzf ../downloads/autoconf-2.69.tar.gz
    cd autoconf-2.69
    ./configure --prefix=/usr/local
    make
    make install
    cd ../..
```

2. automake
    ```
    cd downloads
    curl -OL http://ftpmirror.gnu.org/automake/automake-1.15.tar.gz
    cd ../build
    tar xzf ../downloads/automake-1.15.tar.gz
    cd automake-1.15
    ./configure --prefix=/usr/local
    make
    make install
    cd ../../
```

3. lha
    ```
    cd repos
    git clone https://github.com/jca02266/lha.git
    cd lha
    aclocal
    autoheader
    automake -a
    autoconf
    ./configure --prefix=/usr/local
    make
    make install
    cd ../../
```

4. Install the NDK
    ```
    cd downloads
    curl -O http://www.haage-partner.de/download/AmigaOS/NDK39.lha
    cd ../build
    lha x ../downloads/NDK39.lha
    cp -r NDK_3.9/Include/include_i/* /usr/local/amiga/vgcc/ndk/include/
    cp -r NDK_3.9/Include/include_h/* /usr/local/amiga/vgcc/ndk/include/
    cp -r NDK_3.9/Include/linker_libs/* /usr/local/amiga/vgcc/ndk/lib
    cd ..
```

5. pkg-config
    ```
    cd downloads 
    curl -OL https://pkg-config.freedesktop.org/releases/pkg-config-0.29.tar.gz
    cd ../build
    tar zxf ../downloads/pkg-config-0.29.tar.gz
    cd pkg-config-0.29
    ./configure CC=clang --with-internal-glib --prefix=/usr/local LDFLAGS="-framework CoreFoundation -framework Carbon"
    make -j4
    make install
    cd ../../
```

6. libtool
    ```
    cd downloads
    curl -OL http://ftpmirror.gnu.org/libtool/libtool-2.4.6.tar.gz
    cd ../build    
    tar zxfv ../downloads/libtool-2.4.6.tar.gz
    cd libtool-2.4.6
    ./configure --prefix=/usr/local
    make
    make install
    cd ../../
```

7. libpng
    ```
   cd downloads
   curl -OL ftp://ftp.simplesystems.org/pub/png/src/libpng16/libpng-1.6.28.tar.gz
   cd ../build
   tar zxfv ../downloads/libpng-1.6.28.tar.gz
   cd libpng-1.6.28
   ./configure --prefix=/usr/local
   make
   make install
   cd ../..
```

8. pngquant
    ```
    cd repos
    git clone git://github.com/pornel/pngquant.git
    cd pngquant
    ./configure --prefix=/usr/local
    cd lib
    ./configure --prefix=/usr/local
    make
    mkdir /usr/local/include/pngquant
    cp *.h /usr/local/include/pngquant/
    cp *.a /usr/local/lib
    cd ../../..
```

9. GraphicsMagick
    ```
    cd downloads
    curl -OL http://78.108.103.11/MIRROR/ftp/GraphicsMagick/1.3/GraphicsMagick-1.3.23.tar.gz
    cd build
    tar zxfv ../downloads/GraphicsMagick-1.3.23.tar.gz
    cd GraphicsMagick-1.3.23
    ./configure --prefix=/usr/local
    make
    make install
    cd ../../
```

10. CMake
    ```
    cd downloads
    curl -OL  https://cmake.org/files/v3.5/cmake-3.5.1-Darwin-x86_64.tar.gz
    cd ../build
    tar zxfv ../downloads/cmake-3.5.1-Darwin-x86_64.tar.gz
    mv cmake-3.5.1-Darwin-x86_64/CMake.app /Applications
    cd ..
```

11. TMX C Loader
    ```
    cd repos
    git clone https://github.com/baylej/tmx.git
    cd tmx
    mkdir build
    cd build
    /Applications/CMake.app/Contents/bin/cmake ..
    make install
    cd ../../..
```

12. SOX
    ```
    cd repos
    git clone git://sox.git.sourceforge.net/gitroot/sox/sox
    cd sox
    autoreconf -i
    ./configure --prefix=/usr/local
    make install
    cd ../../
```

13. vlink
  ``` 
    cd downloads
    curl -OL http://sun.hasenbraten.de/vlink/release/vlink.tar.gz
    cd ../build
    tar zxfv ../downloads/vlink.tar.gz 
    cd vlink
    mkdir objects
    make
    cp vlink /usr/local/amiga/vgcc/bin
    cd ../../
```

14. vasm
    ```
    cd downloads
    curl -OL http://sun.hasenbraten.de/vasm/release/vasm.tar.gz
    cd ../build
    tar zxfv ../downloads/vasm.tar.gz
    cd vasm
    make CPU=m68k SYNTAX=mot
    cp vasmm68k_mot /usr/local/amiga/vgcc/bin/
    cd ../../
```

15. fake wget (if you don't have wget)
    ```
    echo "#\!/bin/sh" > /usr/local/bin/wget
    echo "curl -OL \$*" >>  /usr/local/bin/wget
    chmod +x /usr/local/bin/wget
```

16. vasm amiga gcc
    ```
    cd repos   
    git clone https://github.com/bebbo/gcc
    cd gcc
     ./contrib/download_prerequisites
    cd ../../build
    mkdir gcc
    cd gcc
    ../../repos/gcc/configure --prefix=/usr/local/amiga/vgcc --target=m68k-amigaosvasm --enable-languages=c --with-as=/usr/local/amiga/vgcc/bin/vasmm68k_mot --with-ld=/usr/local/amiga/vgcc/bin/vlink
    make -j4 all-gcc
    make -j4 install-gcc
    cd ../../
```

17. vegeta
    ```
    cd repos
    git clone https://github.com/alpine9000/vegeta.git
    cd vegeta
    make
``` 
