# DST set

`https://forums.kleientertainment.com/forums/topic/64441-dedicated-server-quick-setup-guide-linux/`

The Cluster Folder (<Cluster_Name>/)
This is the top-level directory for a specific server instance (your cluster).
Example: ~/.klei/DoNotStarveTogether/MyAwesomeServer/

Inside this folder, you'll find:

cluster.ini:

Purpose: This is the main configuration file for the entire cluster. It defines global settings that apply to all shards within this cluster.
Key Settings:
cluster_name: The name that appears in the in-game server browser.
cluster_description: A short description for the server browser.
cluster_password: Password for joining the server.
game_mode: (survival, endless, wilderness)
max_players
pvp: (true/false)
pause_when_empty: (true/false)
cluster_intention: (social, cooperative, competitive, madness)
autosaver_enabled: (true/false)
enable_vote_kick: (true/false)
shard_enabled: Crucial for caves! Set to true if you want to run multiple shards (like Master and Caves).
offline_cluster: (true/false) - if true, no token needed, but server won't be public.
Location: <Cluster_Name>/cluster.ini
cluster_token.txt:

Purpose: This file contains your unique server token, which links your dedicated server to your Klei account. It's essential for your server to appear in the public server browser.
How to get it: Generated from within the DST game client using the console command TheNet:GenerateClusterToken().
Location: <Cluster_Name>/cluster_token.txt
adminlist.txt:

Purpose: Lists Klei User IDs (KU_XXXXXXXX) of players who will have admin privileges on the server.
Format: One KU_ID per line.
Location: <Cluster_Name>/adminlist.txt
whitelist.txt:

Purpose: Lists Klei User IDs of players who are specifically allowed to join the server if it's whitelisted.
Format: One KU_ID per line.
Location: <Cluster_Name>/whitelist.txt
blocklist.txt:

Purpose: Lists Klei User IDs of players who are banned from the server.
Format: One KU_ID per line.
Location: <Cluster_Name>/blocklist.txt

Example Minimal Structure (with Caves):
~/.klei/DoNotStarveTogether/
└── MyAwesomeServer/
    ├── cluster.ini
    ├── cluster_token.txt
    ├── adminlist.txt (optional)
    ├── Master/
    │   ├── server.ini
    │   ├── worldgenoverride.lua
    │   ├── modoverrides.lua
    │   ├── dedicated_server_mods_setup.lua
    │   └── save/
    └── Caves/
        ├── server.ini
        ├── worldgenoverride.lua
        ├── modoverrides.lua
        ├── dedicated_server_mods_setup.lua
        └── save/

