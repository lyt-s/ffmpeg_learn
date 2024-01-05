#!bin/bash
#seript/install_thirdparty.sh

set -eux
# Important paths:
#current script location:
# SCRIPT_DIR=$(dirname -- "($readlink -f -- "$0")")
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
#project root directory:
PROJECT_ROOT_DIR=$(realpath "$SCRIPT_DIR/../")
# third_party directory:
THIRD_PARTY_DIR=$(realpath "$SCRIPT_DIR/../third_party/")
RET=0

if [ ! -d "$THIRD_PARTY_DIR" ]; then
    mkdir -p $THIRD_PARTY_DIR
fi

cd $THIRD_PARTY_DIR
#Install Boost
if [ ! -d "$THIRD_PARTY_DIR/ffmepg-4.4-ubuntu" ]; then
    rm -rf ffmepg-4.4-ubuntu n4.4.1.zip
    wget https://github.com/FFmpeg/FFmpeg/archive/refs/tags/n4.4.1.zip
    unzip n4.4.1.zip
    cd  ./FFmpeg-n4.4.1
    # 这里的configure 自己进行配置
    ./configure \
        --prefix=$THIRD_PARTY_DIR/ffmepg-4.4\
        --disable-static \
        --disable-stripping \
        --enable-gpl \
        --enable-shared \
        --enable-version3


    make -j8 || RET=$?
    make install || RET=$?
    cd ..
    rm -rf ffmepg-4.4-ubuntu n4.4.1.zip
    if [ $RET -ne 0 ]; then
        rm -rf ffmepg-4.4
        echo "Intall ffmpeg failed"
        exit 1
    fi
fi



#  make test		
# // 报错：You need tcl 8.5 or newer in order to run the Redis test

# // 报错：*** [err]: Active defrag big keys in tests/unit/memefficiency.tcl Expected condition ‘$max_latency <= 120’ to be true (137 <= 120)
# // 解决：vim tests/unit/memefficiency.tcl	改成 max_latency <= 150

# $ cd ..
# $ wget http://downloads.sourceforge.net/tcl/tcl8.6.1-src.tar.gz 
# $ tar xvf tcl8.6.1-src.tar.gz 
# $ cd tcl8.6.1/unix
# $ ./configure --prefix=/apps/bread					
# $ make & make install
