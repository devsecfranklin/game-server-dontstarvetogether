#!/bin/bash

DST_SERVER_DIR="${HOME}/.local/share/Steam"
STEAM_USER="${STEAM_USER}"
STEAM_PASS="${STEAM_PASS}" # Be careful with this!

#cd /home/steam/steamcmd || { echo "Error: SteamCMD directory not found"; exit 1; }
#/home/dst/.local/share/Steam/steamapps
#cd /home/dst/.local/share/Steam/steamcmd

STEAM_COMMAND="+force_install_dir \"$DST_SERVER_DIR\""
STEAM_COMMAND+=" +login \"$STEAM_USER\" \"$STEAM_PASS\"" # Using the dedicated account for workshop mods
STEAM_COMMAND+=" +app_update $DST_APP_ID validate"
# ... rest of your commands ...
STEAM_COMMAND+=" +quit"

echo "RUnning steam command: /usr/games/steamcmd $STEAM_COMMAND"
/usr/games/steamcmd "${STEAM_COMMAND}"

exit 0

# source: https://github.com/zynl/DST_Server/blob/master/update_dst.sh

echo "Killing DST session(s) ..."
screen -ls | grep DST | cut -d. -f1 | awk '{print $1}' | xargs kill
echo "Session killed, will now update DST..."
cd ~/steamcmd/ && ./steamcmd.sh +@ShutdownOnFailedCommand 1 +@NoPromptForPassword 1 +login anonymous +force_install_dir /home/steam/steamapps/DST +app_update 343050 validate
+quit
echo "Update completed, will now start the server..."
~/DST_Server/start_dst.sh
echo "Server successfully started!"
