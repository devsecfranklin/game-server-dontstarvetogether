#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2022-2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: MIT

MODS=$(cat dedicated_server_mods_setup.lua | grep "^Server" | cut -f2 -d'"')
# OUTFILE="${HOME}/saves/modoverrides.lua"
OUTFILE="./modoverrides.lua"

cat << EOF > $OUTFILE
--- @file modoverrides.lua
-- @brief This file determines which mods to load. It must be identical for each shard. 
KLEI 1 return {
EOF

# check the list for dupes!

for i in ${MODS[@]}; do
  echo "found mod: $i"
  #j=$(echo "|${i//[$'\t\r\n ']}|")
  echo "    [\"workshop-${i}\"]={" >>  $OUTFILE
  echo "        configuration_options={ }," >> $OUTFILE
  echo "        enabled=true" >> $OUTFILE
  echo "    }," >> $OUTFILE
done

#head -n -1 ${OUTFILE} > /tmp/modoverrides.lua && mv /tmp/modoverrides.lua ${OUTFILE}
cat "${HOME}/saves/announcemod.txt" >> "${OUTFILE}"
# echo -e "    }\n}" >> ${OUTFILE}
