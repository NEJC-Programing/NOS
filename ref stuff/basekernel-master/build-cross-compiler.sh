#!/bin/sh

PREFIX=`pwd`/cross
WORKDIR=`mktemp -d`

echo "Installing cross-compiler to $PREFIX"
echo "Building in directory $WORKDIR"

pushd "$WORKDIR"



# build and install gcc
mkdir gcc-7.2.0-elf-objs
cd gcc-7.2.0-elf-objs
../gcc-7.2.0/configure --prefix="$PREFIX" --target=i686-elf --disable-nls --enable-languages=c --without-headers
make all-gcc && make all-target-libgcc && make install-gcc && make install-target-libgcc
cd ..

popd
rm -rf "$WORKDIR"

