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
sudo chown user.user /usr/local/amiga

sudo apt-get install python
sudo apt-get install gcc
sudo apt-get install flex
sudo apt-get install autoconf
sudo apt-get install make
sudo apt-get install pkg-config
sudo apt-get install libtool
sudo apt-get install libpng-dev
sudo apt-get install graphicsmagick
sudo apt-get install graphicsmagick-libmagick-dev-compat
sudo apt-get install cmake
sudo apt-get install libxml2-dev
sudo apt-get install sox
sudo apt-get install libwebp-dev


cd repos
git clone https://github.com/jca02266/lha.git
cd lha
aclocal
autoheader
automake -a
autoconf
./configure --prefix=/usr/local
make
sudo make install
cd ../../


cd downloads
curl -O http://www.haage-partner.de/download/AmigaOS/NDK39.lha
cd ../build
lha x ../downloads/NDK39.lha
cp -r NDK_3.9/Include/include_i/* /usr/local/amiga/vgcc/ndk/include/
cp -r NDK_3.9/Include/include_h/* /usr/local/amiga/vgcc/ndk/include/
cp -r NDK_3.9/Include/linker_libs/* /usr/local/amiga/vgcc/ndk/lib
cd ..


cd repos
git clone git://github.com/pornel/pngquant.git
cd pngquant
./configure --prefix=/usr/local
cd lib
./configure --prefix=/usr/local
make
sudo mkdir /usr/local/include/pngquant
sudo cp *.h /usr/local/include/pngquant/
cp *.a /usr/local/lib
cd ../../..


cd repos
git clone https://github.com/baylej/tmx.git
cd tmx
mkdir build
cd build
cmake ..
sudo make install
cd ../../..


cd downloads
curl -OL http://sun.hasenbraten.de/vlink/release/vlink.tar.gz
cd ../build
tar zxfv ../downloads/vlink.tar.gz 
cd vlink
mkdir objects
make
sudo cp vlink /usr/local/amiga/vgcc/bin
cd ../../


cd downloads
curl -OL http://sun.hasenbraten.de/vasm/release/vasm.tar.gz
cd ../build
tar zxfv ../downloads/vasm.tar.gz
cd vasm
make CPU=m68k SYNTAX=mot
sudo cp vasmm68k_mot /usr/local/amiga/vgcc/bin/
cd ../../


cd repos
https://github.com/bebbo/gcc.git
cd gcc
git apply --apply -3 ~/bebbo-gcc-6-branch-vasm4.patch
./contrib/download_prerequisites
cd ../../build
mkdir gcc
cd gcc
../../repos/gcc/configure --prefix=/usr/local/amiga/vgcc --target=m68k-amigaosvasm --enable-languages=c --with-as=/usr/local/amiga/vgcc/bin/vasmm68k_mot
make -j4 all-gcc
make -j4 install-gcc
cd ../../
