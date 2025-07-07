#!/usr/bin/env bash

# SPDX-FileCopyrightText: ©2025 yaya pancho <pancho@xxx.xxx>
#
# SPDX-License-Identifier: MIT

# v0.1 07/04/2025 build script - ya ya pancho
# v0.2 07/06/25 tweak-a-rooni franklin

set -euo pipefail # Exit on error, exit on unset variables, fail if any command in a pipe fails.
IFS=$'\n\t'       # Preserve newlines and tabs in word splitting.

# --- Some config Variables ----------------------------------------
FYP_OPTIONS="-DFYP_CCACHE=ON"
CMAKE_OPTIONS="-DCMAKE_EXPORT_COMPILE_COMMANDS=True -DCMAKE_BUILD_TYPE=RelWithDebInfo"
COMBINED_OPTIONS="$FYP_OPTIONS $CMAKE_OPTIONS -GNinja"
DEBIAN_PKG=(cmake ninja-build)
BUILD_DIR="${PWD}/bulid_linux"

# --- Terminal Colors ---
LRED='\033[1;31m'
LGREEN='\033[1;32m'
LBLUE='\033[1;34m'
# CYAN='\033[0;36m'
LPURP='\033[1;35m'
NC='\033[0m' # No Color

# --- Helper Functions for Logging ---
log_header() {
	printf "\n${LPURP}# --- %s ${NC}\n" "$1"
}

log_info() { printf "${LBLUE}==>${NC} \e[1m%s\e[0m\n" "$1"; } # Using printf for Bold
log_warn() { printf >&2 "${YELLOW}WARN:${NC} %s\n" "$1"; }
log_success() { printf "${LGREEN}==>${NC} \e[1m%s\e[0m\n" "$1"; } # Using printf for Bold

log_error() {
	printf "${LRED}ERROR: %s${NC}\n" "$1" >&2
	exit 1
}

function make_that_dir() {
	if [ ! -d "${1}" ]; then
    echo -e "\n${LPURP}# --- create folder -------------------------------------------\n${NC}"
    echo "mkdir $1"
		mkdir "${1}" || log_error "fail to create directory: $1"
	else
		log_info "The folder already exist: $1"
	fi
}

function install_deb_pkg() {
  echo -e "\n${LPURP}# --- install packages -------------------------------------------\n${NC}"
  sudo DEBIAN_FRONTEND=noninteractive apt-get install -y "${DEBIAN_PKG[@]}"
}

function main() {
  make_that_dir "${BUILD_DIR}"
  install_deb_pkg

  echo -e "\n${LPURP}# --- Running cmake -------------------------------------------\n${NC}"
	echo "Options: ${COMBINED_OPTIONS}"
	echo -e "\n${LPURP}# --- Building for Linux -------------------------------------------\n${NC}"
	cd build_linux
	cmake . "${COMBINED_OPTIONS}" -DCMAKE_TOOLCHAIN_FILE="../toolchain/linux.cmake"
	ninja
	cd -

	#echo "[$0] For Windows build using toolchain $toolchain_windows"
	#cd build_windows;cmake .. $COMBINED_OPTIONS  -DCMAKE_TOOLCHAIN_FILE=$toolchain_windows; ninja
}

main "$@"
