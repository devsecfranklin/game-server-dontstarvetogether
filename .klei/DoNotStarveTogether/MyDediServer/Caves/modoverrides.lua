--- @file modoverrides.lua
-- @brief This file determines which mods to load. It must be identical for each shard. 
KLEI 1 return {
    ["workshop-3138571948"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-373991022"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-375859599"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-3361016346"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-398858801"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-1751811434"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-1077747217"] = {
        enabled = true,
        configuration_options = {
            ANNOUNCEMENT_INTERVAL=45,
            ANNOUNCEMENT_SWITCH = true,
            ANNOUNCEMENT_TEXTS = {
                "[SYSOPS] you have no chance to survive make your time",
                "[SYSOPS] all your base are belong to us", --  survive %pds days!",
                "[SYSOPS] server name is wonderland if you want tojoin the game",
                "[SYSOPS] watch the game stream at https://www.twitch.tv/s1y_b0rg"
            },
            ANNOUNCE_FIRST_TIME_JOIN = "welcome to the struggle", 
            SPEAK_DURATION_SHOW = 5,
            SPEAK_FIRST_TIME_JOIN = "welcome to wonderland",
            SPEAK_EVERY_TIME_JOIN = "welcome to wonderland",
            SPEAK_ON_DEATH = "i cant believe i died last night" , -- Death Say Text
            SPEAK_ON__RESURRECT = "I LIVE" , -- Resurrection Say Text
            SPEAK_SWITCH=true
        }
    }
}
