--- @file dedicated_server_mods_setup.lua

-- @brief add the ServerModSetup line then run the gen script to create a fresh
--        modoverdires.lua this will all get copied into place by the runner script

-- @brief These are the permanent, working mods.
ServerModSetup("3138571948") -- Global Positions (CompleteSync) - Recommended over older Global Positions
ServerModSetup("373991022") -- Global Player Icons https://steamcommunity.com/sharedfiles/filedetails/?id=632082897
ServerModSetup("375859599") -- health indicators https://steamcommunity.com/sharedfiles/filedetails/?id=375859599
ServerModSetup("3361016346") -- finder (pink chest) https://steamcommunity.com/sharedfiles/filedetails/?id=3361016346
ServerModSetup("398858801") -- AFK detection https://steamcommunity.com/sharedfiles/filedetails/?id=398858801
ServerModSetup("1751811434") -- fast work https://steamcommunity.com/sharedfiles/filedetails/?id=1751811434

-- @brief These mods are under test, still suspect.
ServerModSetup("2880198735") -- skeleton revive https://steamcommunity.com/sharedfiles/filedetails/?id=2880198735!
-- cool but not ready yet ServerModSetup("2594707725") -- https://github.com/gyroplast/mod-dont-starve-chat-announcements
-- ServerModSetup("382177939") -- DST Storm Cellar
ServerModSetup("1077747217") -- Server Announcement https://steamcommunity.com/sharedfiles/filedetails/?id=1077747217
