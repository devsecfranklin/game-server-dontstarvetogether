#!/bin/bash

STEAM_USER="${STEAM_USER}"
STEAM_PASS="${STEAM_PASS}" # Be careful with this!

#cd /home/steam/steamcmd || { echo "Error: SteamCMD directory not found"; exit 1; }
#/home/dst/.local/share/Steam/steamapps
cd /home/dst/.local/share/Steam/steamcmd

# Login with credentials, then perform actions and quit
/usr/bin/steamcmd +login "$STEAM_USER" "$STEAM_PASS" +force_install_dir /home/dst/.local/share/Steam +app_update 343050 validate +quit
