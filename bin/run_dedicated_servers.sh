#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: ©2021-2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

set -euo pipefail # Exit on error, exit on unset variables, fail if any command in a pipe fails.
IFS=$'\n\t'       # Preserve newlines and tabs in word splitting.

# --- Terminal Colors ---
LRED='\033[1;31m'
LGREEN='\033[1;32m'
LBLUE='\033[1;34m'
CYAN='\033[0;36m'
LPURP='\033[1;35m'
NC='\033[0m' # No Color

# --- Some config Variables ----------------------------------------
BIN_DIR="/home/dst/.local/share/Steam/steamapps/common/Don\'t\ Starve\ Together/bin64"
cluster_name="MyDediServer"
dontstarve_dir="/home/dst/.klei/DoNotStarveTogether"

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

function fail() {
  echo Error: "$@" >&2
  exit 1
}

function check_for_file() {
  log_info "Check for file: $1"
  if [ ! -e "$1" ]; then
    fail "Missing file: $1"
  fi
}

function main() {
  log_header "Updating game server files from Steam"
  # steamcmd +force_install_dir "$install_dir" +login anonymous +app_update 343050 validate +quit
  steamcmd +login anonymous +app_update 343050 validate +quit

  # Caves has a soft link to the same file
  log_header "Installing our custom server files"
  cp /home/dst/saves/modoverrides.lua "${dontstarve_dir}/${cluster_name}/Master"
  cp /home/dst/saves/dedicated_server_mods_setup.lua "${dontstarve_dir}/${cluster_name}"
  cp /home/dst/saves/worldgenoverrides.lua "${dontstarve_dir}/$cluster_name/Master"
  cp /home/dst/saves/worldgenoverrides.lua-caves "${dontstarve_dir}/${cluster_name}/Caves/worldgenoverrides.lua"

  log_header "Copying cluster token into server files"
  echo "${CLUSTER_TOKEN}" >"${dontstarve_dir}/${cluster_name}/cluster_token.txt"
  check_for_file "$dontstarve_dir/$cluster_name/cluster_token.txt"
  if [ ! -f "$dontstarve_dir/$cluster_name/cluster.ini" ]; then 
    login_info "copying cluster.ini"
    cp "/home/dst/saves/cluster.ini" "$dontstarve_dir/$cluster_name/cluster.ini"
  fi
  log_info "sed the secret token into the files"
  sed -i "s/cluster_password =/cluster_password = ${CLUSTER_TOKEN}/g" "${dontstarve_dir}/${cluster_name}/cluster.ini"
  check_for_file "$dontstarve_dir/$cluster_name/cluster.ini"
  check_for_file "$dontstarve_dir/$cluster_name/Master/server.ini"
  check_for_file "$dontstarve_dir/$cluster_name/Caves/server.ini"

  pushd "${BIN_DIR}" || fail "Unable to cd to bin dir"
  run_shared=(./dontstarve_dedicated_server_nullrenderer_x64)
  run_shared+=(-cluster "${cluster_name}")
  log_header "start cave shard"
  "${run_shared[@]}" -shard Caves | sed 's/^/Caves:  /' &
  log_header "start master shard"
  "${run_shared[@]}" -shard Master | sed 's/^/Master: /'
  popd || fail "could not return successfully"

  log_success "Ready to play!"

}

main "$@"
