The Shard Folders (<Cluster_Name>/Master/ and <Cluster_Name>/Caves/)
Inside your cluster folder, you'll have subdirectories for each shard.

Master/: This is for your Overworld (surface) shard.
Caves/: This is for your Caves shard (only if shard_enabled = true in cluster.ini).
Each shard folder contains configuration and save data specific to that shard:

server.ini:

Purpose: Configures shard-specific network and role settings.
Key Settings:
server_port: Crucial! Each shard needs a unique port. The Master shard typically uses 10999 (or default 11000 on some setups), and the Caves shard usually uses the next available port (e.g., 10998 or 11001).
is_master: true for the Master shard, false for the Caves shard.
master_port: For non-master shards only. Points to the Master shard's port, allowing inter-shard communication (e.g., master_port = 10999 in Caves/server.ini).
cluster_key: A shared secret key that allows shards in the same cluster to communicate. Must be identical across all shards in the cluster.
Location: <Cluster_Name>/Master/server.ini, <Cluster_Name>/Caves/server.ini
worldgenoverride.lua:

Purpose: Defines the world generation settings for that specific shard. This is where you configure biomes, resources, giants, etc.
Format: Lua table. It often starts with return { override_enabled = true, preset = "SURVIVAL_TOGETHER", overrides = { ... } }.
How to create/edit: You can generate this by hosting a game locally, configuring the world settings you want, then exiting and copying the leveldataoverride.lua (or worldgenoverride.lua) from your local save to your server's shard folder, and renaming it if necessary.
Location: <Cluster_Name>/Master/worldgenoverride.lua, <Cluster_Name>/Caves/worldgenoverride.lua
modoverrides.lua:

Purpose: Enables and configures individual mods for that specific shard.
Format: Lua table where each entry corresponds to a mod's workshop ID and its settings.
Key Setting: enabled = true to activate the mod.
Location: <Cluster_Name>/Master/modoverrides.lua, <Cluster_Name>/Caves/modoverrides.lua
dedicated_server_mods_setup.lua:

Purpose: Tells the server which mods to download from the Steam Workshop. This file is parsed when the server starts, and it fetches any new or updated mods.
Format: Simply a list of ServerModSetup("WORKSHOP_ID") calls.
Location: <Cluster_Name>/Master/dedicated_server_mods_setup.lua, <Cluster_Name>/Caves/dedicated_server_mods_setup.lua
save/ folder:

Purpose: Contains the actual save data for the world. This is where your world progress, player locations, base structures, etc., are stored.
Inside save/ you'll find subfolders named with session IDs (e.g., 0000000001). Each session folder contains the leveldata (the actual world data) and player-specific saves.
Location: <Cluster_Name>/Master/save/, <Cluster_Name>/Caves/save/
