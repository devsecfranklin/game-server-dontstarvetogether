#!/usr/bin/env bash

# SPDX-FileCopyrightText: ©2025 yaya pancho <pancho@xxx.xxx>
#
# SPDX-License-Identifier: MIT

# v0.1 07/04/2025 build script - ya ya pancho
# v0.2 07/06/25 tweak-a-rooni franklin

#set -euo pipefail # Exit on error, exit on unset variables, fail if any command in a pipe fails.
IFS=$'\n\t'       # Preserve newlines and tabs in word splitting.

# --- Some config Variables ----------------------------------------
CONTAINER=false
GO_VERSION="$(go version | awk '{print $3}')"
FYP_OPTIONS="-DFYP_CCACHE=ON"
CMAKE_OPTIONS="-DCMAKE_EXPORT_COMPILE_COMMANDS=True -DCMAKE_BUILD_TYPE=RelWithDebInfo"
COMBINED_OPTIONS="$FYP_OPTIONS $CMAKE_OPTIONS"
DEBIAN_PKG=(cmake ninja-build generate-ninja)
#DOCUMENTATION=false
KERNEL=$(uname -r)       # the kernel version
MACHINE_TYPE=$(uname -m) # the machine type
MY_DATE=$(date '+%Y-%m-%d-%H')
MY_OS=$(uname | tr '[:upper:]' '[:lower:]') # usually "linux"
MY_UNAME=$(uname)
OS_RELEASE="$(cat /etc/os-release | grep "^ID=" | cut -d"=" -f2)"
PRIV_CMD="sudo"
RAW_OUTPUT="/tmp/bootstrap_lab_${MY_DATE}.log"

DIR="${PWD}"
BUILD_DIR="${DIR}/build_linux"

# --- Terminal Colors ---
LRED='\033[1;31m'
LGREEN='\033[1;32m'
LBLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
LPURP='\033[1;35m'
NC='\033[0m' # No Color

# --- Helper Functions for Logging ---
log_header() {
	printf "\n${LPURP}# --- %s ${NC}\n" "$1"
}
log_info() { printf "${LBLUE}==>${NC} \e[1m%s\e[0m\n" "$1"; } # Using printf for Bold
log_warn() { printf >&2 "${CYAN}WARN:${NC} %s\n" "$1"; }
log_success() { printf "${LGREEN}==>${NC} \e[1m%s\e[0m\n" "$1"; } # Using printf for Bold
log_error() {
	printf "${LRED}ERROR: %s${NC}\n" "$1" >&2
	exit 1
}

check_container() {
  log_header "Check Container Status"
  
  if [ -f /.dockerenv ]; then
    log_info "Containerized build environment..."
    CONTAINER=true
  else
    log_info "NOT a containerized build environment..."
  fi
}

function check_python_version() {
  log_header "Check Python Version"
  if command -v python &>/dev/null; then
    PYTHON_VERSION=$(python -c 'import sys; print(sys.version_info.major)')
    if [[ "$PYTHON_VERSION" -eq 3 ]]; then
      echo -e "${LBLUE}The 'python' command points to Python 3.${NC}"
      # Use 'python'
      PYTHON_CMD="python"
    elif [[ "$PYTHON_VERSION" -eq 2 ]]; then
      echo -e "${LBLUE}The 'python' command points to Python 2.${NC}"

      # Decide what to do: try python3, or exit
      if command -v python3 &>/dev/null; then
        echo -e "Using 'python3' instead."
        PYTHON_CMD="python3"
      else
        echo -e "${LRED}Error: Python 3 not found. Exiting.${NC}"
        exit 1
      fi
    else
      echo -e "${LRED}The 'python' command points to an unknown Python version ($PYTHON_VERSION).${NC}"
      # Decide what to do
      if command -v python3 &>/dev/null; then
        echo "Attempting to use 'python3' instead.${NC}"
        PYTHON_CMD="python3"
      else
        echo -e "${LRED}Error: Python 3 not found. Exiting.${NC}"
        exit 1
      fi
    fi
  elif command -v python3 &>/dev/null; then
    echo "'python' command not found, using 'python3'.${NC}"
    PYTHON_CMD="python3"
  else
    echo -e "${LRED}Error: Neither 'python' nor 'python3' found. Please install Python 3. Exiting.${NC}"
    exit 1
  fi

  echo -e "${LBLUE}Using Python command: ${PYTHON_CMD}${NC}"
}

function detect_hardware() {
  # FIXME: need to detect ubuntu running on nvidia
  log_header "Hardware Detection"

  echo -e "${LBLUE}Machine type: ${MACHINE_TYPE}${NC}"
  echo -e "${LBLUE}Kernel: ${MACHINE_TYPE}${NC}"

  # ---------- # RASPBERRY PI #----------#
  # $PYTHON_CMD -c "import platform; print ('raspberrypi') in platform.uname()"
  # check /proc/cpuinfo for "Model"
  IS_RASPI="$(grep Model /proc/cpuinfo | cut -f2 -d':')"
  if [ -n "${IS_RASPI}" ]; then
    echo -e "${YELLOW}Found Raspberry Pi: ${IS_RASPI}${NC}"
    #install_raspberry_pi
    cat /proc/device-tree/model
  fi
}

function detect_os() {
  # FIXME: need to detect ubuntu running on nvidia
  log_header "Operating System Detection"

  # first know the OS
  if [ -n "${MY_OS}" ]; then
    echo -e "${LBLUE}Operating System based on uname: ${MY_OS}${NC}"
  fi

  case $MY_OS in

  linux)
    # if linux, check the distro. check for the /etc/os-release file
    if [ -n "${OS_RELEASE}" ]; then
      echo -e "${LBLUE}Found /etc/os-release file: ${OS_RELEASE}${NC}"
      #if [ "${OS_RELEASE}" == "debian" ]; then install_debian; fi
    fi

    #if [ "$(grep -Ei 'debian|buntu|mint' /etc/*release)" ]; then
    #  echo -e "${CYAN}Detected Debian/Ubuntu/Mint${NC}"
    #fi

    if [ -f "/etc/redhat-release" ]; then
      echo -e "${CYAN}Detected Red Hat/CentOS/RHEL${NC}\n"
      MY_OS="rh"
      #install_redhat
    fi

    if grep -q Microsoft /proc/version; then
      echo -e "${CYAN}Detected Windows pretending to be Linux${NC}\n"
      MY_OS="win"
    fi
    ;;
  openbsd)
    echo -e "${LBLUE}Detected OpenBSD${NC}\n"
    PRIV_CMD="doas" # there is no sudo
    #install_openbsd
    ;;
  darwin)
    echo -e "${CYAN}Detected MacOS${NC}\n"
    #check_installed brew
    #install_macos
    ;;
  *)
    echo -e "${YELLOW}Unrecongnized architecture. Time to panic!${NC}\n"
    exit 1
    ;;
  esac
}

function make_that_dir() {
	if [ ! -d "${1}" ]; then
    log_header "create folder"
    echo "mkdir $1"
		mkdir "${1}" || log_error "fail to create directory: $1"
	else
		log_info "The folder already exist: $1"
	fi
}

function install_deb_pkg() {
  log_header "install packages"
  log_warn "$(/mnt/clusterfs/build/bin/cmake --version)" || log_error "Unable to find cmake"
  sudo DEBIAN_FRONTEND=noninteractive apt-get install -y "${DEBIAN_PKG[@]}"
}

function main() {
  check_python_version
  detect_os
  detect_hardware

  # cp "${PWD}/toolchain/linux.cmake" "${PWD}/build_linux"	
  make_that_dir "${BUILD_DIR}"
  install_deb_pkg

  log_header "Running cmake"
  log_info  "Options chosen for this build:    ${COMBINED_OPTIONS}"
  log_header "Building for Linux"
  cmake . "${COMBINED_OPTIONS}" -DCMAKE_TOOLCHAIN_FILE="${DIR}/toolchain/linux.cmake" -DCC="/usr/bin/gcc" -DCXX="/usr/bin/g++" -GNinja
  ninja

  #log_info "[$0] For Windows build using toolchain $toolchain_windows"
  #cd build_windows;cmake .. $COMBINED_OPTIONS  -DCMAKE_TOOLCHAIN_FILE=$toolchain_windows; ninja
}

main "$@"
