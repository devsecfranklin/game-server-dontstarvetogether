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
YELLOW=$(tput setaf 3)
LPURP='\033[1;35m'
NC='\033[0m' # No Color

# --- Some config Variables ----------------------------------------
DST_SERVER_DIR="/home/franklin/workspace/game-server-dontstarvetogether/.local/share/Steam/steamapps/common/Don't Starve Together Dedicated Server"
cluster_name="MyDediServer"
dontstarve_dir="${HOME}/.klei/DoNotStarveTogether"

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

function game_update() {
  log_header "Updating game server files from Steam"
  # steamcmd +force_install_dir "$install_dir" +login anonymous +app_update 343050 validate +quit
  steamcmd +login "$STEAM_USER" "$STEAM_PASS" +app_update 343050 validate +quit
}

function set_password() {
  # make it so people need a password to get into server
  log_info "sed the server password"
  sed -i "s/cluster_password =/cluster_password = ${CLUSTER_TOKEN}/g" "${dontstarve_dir}/${cluster_name}/cluster.ini"
}

function main() {
  # Caves has a soft link to the same file
  log_header "Installing our custom server files"
  cp ${HOME}/saves/modoverrides.lua "${dontstarve_dir}/${cluster_name}/Master"
  cp ${HOME}/saves/dedicated_server_mods_setup.lua "${dontstarve_dir}/${cluster_name}"
  cp ${HOME}/saves/modoverrides.lua ${HOME}/saves/dedicated_server_mods_setup.lua "${DST_SERVER_DIR}/mods"
  cp ${HOME}/saves/worldgenoverride.lua "${dontstarve_dir}/$cluster_name/Master"
  cp ${HOME}/saves/worldgenoverride.lua "${dontstarve_dir}/${cluster_name}/" # is this necessary?
  cp ${HOME}/saves/worldgenoverride.lua-caves "${dontstarve_dir}/${cluster_name}/Caves/worldgenoverride.lua"
  log_header "Copying cluster token into server files"
  echo "${CLUSTER_TOKEN}" >"${dontstarve_dir}/${cluster_name}/cluster_token.txt"
  check_for_file "$dontstarve_dir/$cluster_name/cluster_token.txt"
  cp "${HOME}/saves/cluster.ini" "$dontstarve_dir/$cluster_name/cluster.ini"
  # set_password
  check_for_file "$dontstarve_dir/$cluster_name/cluster.ini"
  check_for_file "$dontstarve_dir/$cluster_name/Master/server.ini"
  check_for_file "$dontstarve_dir/$cluster_name/Caves/server.ini"
  log_warn "If any players cannot see the server in the list, update the game files from steam"
  #game_update
  pushd "/home/dst/.local/share/Steam/steamapps/common/Don't Starve Together Dedicated Server/bin64"
  log_info "Start servers"

  run_shared=(./dontstarve_dedicated_server_nullrenderer_x64)
  run_shared+=(-cluster "$cluster_name")
  "${run_shared[@]}" -shard Caves | sed 's/^/Caves:  /' &
  "${run_shared[@]}" -shard Master | sed 's/^/Master: /'

  #screen -dmS "DST Server" sh -c "cd /home/steam/steamapps/DST/bin; ./dontstarve_dedicated_server_nullrenderer"
  #screen -dmS "DST_caves Server" sh -c "cd /home/steam/steamapps/DST/bin; ./dontstarve_dedicated_server_nullrenderer -conf_dir DST_Cave"
  #/usr/bin/screen -S "DST" bash -c 'LD_LIBRARY_PATH=~/dst_lib ./dontstarve_dedicated_server_nullrenderer -console'

  log_success "Ready to play!"
}

main "$@"
