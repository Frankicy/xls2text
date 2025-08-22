#!/usr/bin/env bash
set -euo pipefail

# 0. 确认源码路径
LIBXLS_SRC="/home/test/libxls/libxls"   # 你的 libxls 源码目录
INSTALL_PREFIX="/opt/libxls"
BUILD_DIR="/home/test/xls_analys/build"

# 1. 安装系统依赖（已装也会跳过）
sudo apt-get update -qq
sudo apt-get install -y autoconf-archive libtool libtool-bin pkg-config

# 2. 编译并安装 libxls
cd "$LIBXLS_SRC"
autoreconf -fi
./configure --prefix="$INSTALL_PREFIX" --enable-shared
make -j"$(nproc)"
sudo make install

# 3. 让 pkg-config 能找到 .pc
export PKG_CONFIG_PATH="$INSTALL_PREFIX/lib/pkgconfig:$PKG_CONFIG_PATH"

# 4. 编译示例程序
cd /home/test/xls_analys
cmake -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"

# 5. 运行测试
echo "=== 编译完成，执行示例 ==="
"$BUILD_DIR/xls2text" input.xls output.txt || echo "请把 input.xls 放到当前目录"