#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: ©2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

#set -euo pipefail

# The special shell variable IFS determines how Bash
# recognizes word boundaries while splitting a sequence of character strings.

#IFS=$'\n\t'

GCC_VER="13.4.0" # check here for releases: https://gcc.gnu.org/pub/gcc/releases
log_header "Install new GCC compiler version: ${GCC_VER}"


if [ ! -f "/tmp/gcc-${GCC_VER}.tar.gz" ]; then
  wget -O /tmp/gcc-${GCC_VER}.tar.gz https://gcc.gnu.org/pub/gcc/releases/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz
  tar -xzf /tmp/gcc-${GCC_VER}.tar.gz -C /tmp
fi

pushd /tmp/gcc-${GCC_VER} || log_error "Unable to unpack new compiler"
# make distclean # there is no distclean directive
/tmp/gcc-${GCC_VER}/contrib/download_prerequisites
./configure --disable-multilib --enable-languages=c,c++ --program-suffix=-13
make -j3

sudo make install

sudo update-alternatives --install /usr/bin/cpp cpp /usr/local/bin/cpp-13 50
sudo update-alternatives --install /usr/bin/gcc gcc /usr/local/bin/gcc-13 50

popd || log_error "unable to do the thing"