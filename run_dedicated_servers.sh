#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: ©2021-2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

#set -euo pipefail

# The special shell variable IFS determines how Bash
# recognizes word boundaries while splitting a sequence of character strings.

#IFS=$'\n\t'

cluster_name="MyDediServer"
dontstarve_dir="$HOME/.klei/DoNotStarveTogether"

function fail()
{
	echo Error: "$@" >&2
	exit 1
}

function check_for_file()
{
	if [ ! -e "$1" ]; then
		fail "Missing file: $1"
	fi
}

check_for_file "$dontstarve_dir/$cluster_name/cluster.ini"
check_for_file "$dontstarve_dir/$cluster_name/cluster_token.txt"
check_for_file "$dontstarve_dir/$cluster_name/Master/server.ini"
check_for_file "$dontstarve_dir/$cluster_name/Caves/server.ini"

# steamcmd +force_install_dir "$install_dir" +login anonymous +app_update 343050 validate +quit
steamcmd +login anonymous +app_update 343050 validate +quit

# Caves has a soft link to the same file
cp ${HOME}/docs/modoverrides.lua "${dontstarve_dir}/${cluster_name}/Master"
cp ${HOME}/docs/dedicated_server_mods_setup.lua "${dontstarve_dir}/${cluster_name}"
cp ${HOME}/docs/worldgenoverrides.lua "${dontstarve_dir}/$cluster_name/Master"
cp ${HOME}/docs/worldgenoverrides.lua-caves "${dontstarve_dir}/${cluster_name}/Caves"

pushd "/home/dst/.local/share/Steam/steamapps/common/Don't Starve Together Dedicated Server/bin64"
run_shared=(./dontstarve_dedicated_server_nullrenderer_x64)
# run_shared+=(-console) # deprecated option
run_shared+=(-cluster "$cluster_name")
# run_shared+=(-monitor_parent_process $$)

"${run_shared[@]}" -shard Caves  | sed 's/^/Caves:  /' &
"${run_shared[@]}" -shard Master | sed 's/^/Master: /'

