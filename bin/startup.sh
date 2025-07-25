#!/usr/bin/env bash

# SPDX-FileCopyrightText: ©2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

# v0.2 July 2, 2025 franklin@bitsmasher.net

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

# --- Some config Variables ----------------------------------------
CLUSTER_NAME="MyDediServer"
MASTER_NAME="Master"
CAVE_NAME="Caves"
CHECK_UPDATE_FREQ="20m"
DONT_STARVE_CLUSTER_DIR="$HOME/.klei/DoNotStarveTogether"
DONT_STARVE_DIR="/home/franklin/workspace/game-server-dontstarvetogether/.local/share/Steam/steamapps/common/Don't Starve Together Dedicated Server"
lv_file="/tmp/${CLUSTER_NAME}_latest_version"
DONT_STARVE_BIN="./dontstarve_dedicated_server_nullrenderer_x64"

# --- Functions ####

function needs_update() {
  klei_url="https://forums.kleientertainment.com/game-updates/dst/"
  latest_version=$(curl -s $klei_url | grep -Po "\s+\d{6,}$" | head -n 1)
  if [[ -e $lv_file ]]; then
    current_version=$(head -n 1 $lv_file)
  fi
  echo "${latest_version}" >$lv_file || log_error "unable to create latest version file"
  if [[ $current_version -lt $latest_version ]]; then
    echo "1"
  else
    echo "-1"
  fi
}

function exists() {
  if [[ ! -e $1 ]]; then
    log_error "File/Dir not found: $1"
  fi
}

function command_m() {
  screen -S ${CLUSTER_NAME}_${MASTER_NAME} -p 0 -X stuff "$1^M"
}

function command_c() {
  screen -S ${CLUSTER_NAME}_${CAVE_NAME} -p 0 -X stuff "$1^M"
}

function stop_server() {
  command_m 'c_announce("Server stopped by force! Shutting down!")'
  sleep "2s"
  command_c 'c_shutdown()'
  command_m 'c_shutdown()'

  # The server gets 20 seconds to shutdown normally #
  sleep "20s"

  # Should the server still be running; Kill it #
  if screen -list | grep -q ${CLUSTER_NAME}; then
    command_c "^C"
    command_m "^C"
  fi
  exit 1
}

trap stop_server 2
exists "$DONT_STARVE_CLUSTER_DIR/$CLUSTER_NAME/cluster.ini"
exists "$DONT_STARVE_CLUSTER_DIR/$CLUSTER_NAME/cluster_token.txt"
exists "$DONT_STARVE_CLUSTER_DIR/$CLUSTER_NAME/$MASTER_NAME/server.ini"
exists "$DONT_STARVE_CLUSTER_DIR/$CLUSTER_NAME/$CAVE_NAME/server.ini"
needs_update >/dev/null 2>&1

while true; do
  mv "$DONT_STARVE_DIR/mods/dedicated_server_mods_setup.lua" "${DONT_STARVE_DIR}/mods/dedicated_server_mods_setup.lua.bak"
  echo "Start updating the game."
  steamcmd +force_install_dir ${DONT_STARVE_DIR} +login anonymous +app_update 343050 validate +quit
  mv "$DONT_STARVE_DIR/mods/dedicated_server_mods_setup.lua.bak" "${DONT_STARVE_DIR}/mods/dedicated_server_mods_setup.lua"

  exists "$DONT_STARVE_DIR/bin64/dontstarve_dedicated_server_nullrenderer_x64"

  cd "${DONT_STARVE_DIR}/bin64" || log_error "fanny pack"
  echo "Starting ${CAVE_NAME}"
  screen -d -m -S ${CLUSTER_NAME}_${CAVE_NAME} $DONT_STARVE_BIN -cluster $CLUSTER_NAME -shard $CAVE_NAME
  echo "Starting ${MASTER_NAME}."
  screen -d -m -S ${CLUSTER_NAME}_${MASTER_NAME} $DONT_STARVE_BIN -cluster $CLUSTER_NAME -shard $MASTER_NAME

  while true; do
    sleep $CHECK_UPDATE_FREQ # Check for updates every interval #

    # If there is an update, we will start the shutdown process #
    result=$(needs_update)
    if [[ $result -gt 0 ]]; then
      echo "The server needs an update. Will restart in 15 minutes."
      command_m 'c_announce("Klei released an update! The server restarts in 15 minutes!")'
      sleep "10m"
      command_m 'c_announce("Klei released an update! The server restarts in 5 minutes!")'
      sleep "4m"
      command_m 'c_announce("Klei released an update! The server restarts in 1 minute!")'
      sleep "1m"
      command_m 'c_announce("Restarting now!")'
      command_c 'c_shutdown()'
      command_m 'c_shutdown()'
      break
    fi
  done

  # We wait till the game shuts down #
  echo "Waiting for shards to shut down."
  while true; do
    if ! screen -list | grep -q ${CLUSTER_NAME}; then
      echo "Shards are down. Restarting."
      break
    fi
  done
done
