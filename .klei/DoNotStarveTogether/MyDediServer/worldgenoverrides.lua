-- SPDX-FileCopyrightText: ©2025 franklin <franklin@bitsmasher.net>
--
-- SPDX-License-Identifier: MIT

KLEI     1 return {
    override_enabled = true, -- Set to true to enable these overrides

    -- You can choose a preset, or leave it for full customisation
    -- preset = "SURVIVAL_TOGETHER", -- Or "SURVIVAL_DEFAULT_PLUS", "COMPLETE_DARKNESS", etc.

    overrides = {
        -- WORLD SIZE AND SHAPE
        world_size = "medium", -- "small", "medium", "large", "huge"
        loop = "always",       -- "never", "default", "always"
        branching = "default", -- "never", "default", "lots"
        wildfire = "default",  -- "none", "little", "default", "more", "lots"

        -- RESOURCE SPAWN RATES
        berries = "default",   -- "never", "rare", "default", "often", "always"
        carrots = "default",
        flint = "default",
        grass = "default",
        saplings = "default",
        mushrooms = "default",
        rocks = "default",
        trees = "default",
        reeds = "default",
        -- ... many more resources ...

        -- ANIMAL SPAWN RATES
        beefalo = "default",   -- "never", "rare", "default", "often", "always"
        bunnymen = "default",
        pigs = "default",
        spiders = "default",
        merms = "default",
        hounds = "default",
        tentacles = "default",
        lureplants = "default",
        -- ... many more animals/creatures ...

        -- SEASON LENGTHS AND FREQUENCY
        season_start = "default", -- "default", "spring", "summer", "autumn", "winter"
        autumn = "default",       -- "default", "short", "long", "veryshort", "verylong"
        winter = "default",
        spring = "default",
        summer = "default",
        long_events = "default",  -- "default", "never", "rare", "often"

        -- HAZARDS AND EVENTS
        lightning = "default", -- "none", "little", "default", "more", "tons"
        rain = "default",
        meteorshowers = "default", -- "none", "little", "default", "more", "lots"
        frograin = "default",

        -- WORLD STRUCTURE AND SPECIFIC SET PIECES
        set_pieces = "default", -- "default", "rare", "never" (affects things like Pig King, Chess Biome, etc.)
        boons = "default",      -- "none", "rare", "default", "often"
        touchstone = "default", -- "none", "rare", "default", "often", "always"
        portal_moon = "default", -- For the Lunar Island portal
        -- ... many more world features ...

        -- MISCELLANEOUS
        regrowth_multiplier = "default", -- "none", "veryslow", "slow", "default", "fast", "veryfast"
        day = "default",        -- "default", "longday", "longdusk", "longnight", "noday", "nodusk", "nonight"
        start_location = "cave", -- "default", "forest", "cave", "plus"
        task_set = "default",   -- "default", "classic", "cave_default"

        -- Example of a mod-specific override (if the mod uses worldgenoverrides)
        -- "MOD_ADVANCEDWORLDGEN_ENABLED" = true,
        -- "MOD_ADVANCEDWORLDGEN_SETTING_CAVES_ON_SURFACE" = "default",
    }
}
