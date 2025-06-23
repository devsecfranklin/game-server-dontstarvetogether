#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: ©2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

#set -euo pipefail

# The special shell variable IFS determines how Bash
# recognizes word boundaries while splitting a sequence of character strings.

#IFS=$'\n\t'

cluster_name="MyDediServer"
dontstarve_dir="$HOME/.klei/DoNotStarveTogether"

function packages() {
  echo -e "\n${LPURP}# --- install packages -------------------------------------------\n${NC}"
  sudo DEBIAN_FRONTEND=noninteractive apt-get install podman containers-storage docker-compose -y
  sudo tee /etc/apt/sources.list.d/steam-stable.list <<'EOF'
deb [arch=amd64,i386 signed-by=/usr/share/keyrings/steam.gpg] https://repo.steampowered.com/steam/ stable steam
deb-src [arch=amd64,i386 signed-by=/usr/share/keyrings/steam.gpg] https://repo.steampowered.com/steam/ stable steam
EOF

  sudo dpkg --add-architecture i386
  sudo apt-get update
  sudo DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libgl1-mesa-dri:amd64 \
  libgl1-mesa-dri:i386 \
  libgl1-mesa-glx:amd64 \
  libgl1-mesa-glx:i386 \
  steam-launcher
}

function dst_user() {
  echo -e "\n${LPURP}# --- add dst user to games group  -------------------------------------------\n${NC}"
  # add to games group
  sudo useradd -d /home/dst -g 60 -m -s /bin/bash -u 6969 dst
}

function docker_setup() {
  echo -e "\n${LPURP}# --- containerized server -------------------------------------------\n${NC}"
  # clone the dst repo and put in /opt
  sudo git clone https://github.com/mathielo/dst-dedicated-server.git /opt/dst-dedicated-server
  sudo chown -R dst:games /opt/dst-dedicated-server
}

function main() {
  if [ ! -d "${dontstarve_dir}" ]; then mkdir -p "${dontstarve_dir}"; fi
  echo -e "\n${LPURP}# --- updating dedi server application -------------------------------------------\n${NC}"
  steamcmd +login anonymous +app_update 343050 validate +quit

  echo -e "\n${LPURP}# --- copy in config files -------------------------------------------\n${NC}"
  # Caves has a soft link to the same file
  cp ${HOME}/docs/modoverrides.lua "${dontstarve_dir}/${cluster_name}/Master"
  cp ${HOME}/docs/dedicated_server_mods_setup.lua "${dontstarve_dir}/${cluster_name}"
  cp ${HOME}/docs/worldgenoverrides.lua "${dontstarve_dir}/$cluster_name/Master"
  cp ${HOME}/docs/worldgenoverrides.lua-caves "${dontstarve_dir}/${cluster_name}/Caves"
}

main "$@"
