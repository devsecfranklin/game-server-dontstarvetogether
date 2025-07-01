#!/bin/sh

# source: https://github.com/zynl/DST_Server/blob/master/update_dst.sh

echo "Killing DST session(s) ..."
screen -ls | grep DST | cut -d. -f1 | awk '{print $1}' | xargs kill
echo "Session killed, will now update DST..."
cd ~/steamcmd/ && ./steamcmd.sh +@ShutdownOnFailedCommand 1 +@NoPromptForPassword 1 +login anonymous +force_install_dir /home/steam/steamapps/DST +app_update 343050 validate +quit
echo "Update completed, will now start the server..."
~/DST_Server/start_dst.sh
echo "Server successfully started!"
