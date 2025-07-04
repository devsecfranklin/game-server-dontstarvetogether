KLEI     1 return {
	--
    -- This is an example worldgenoverrides.lua file for Don't Starve Together.
-- Copy this file into your Cluster_X/Master/ and/or Cluster_X/Caves/ directories
-- to customize world generation settings.
--
-- For the most accurate configuration, it's recommended to set up your desired
-- world settings when creating a new world in the game client, then copy the
-- 'worldgenoverrides.lua' file from your local Klei save folder
-- (e.g., Documents/Klei/DoNotStarveTogether/MyLocalCluster/Master/)
--

    -- The version of the world generation override system. Do not change this.
    _version = 3,

    -- Global settings for the world (e.g., preset, season lengths)
    data = {
        -- Base preset. Options include:
        -- "DEFAULT", "LIGHTSOUT", "NO_SWEAT", "NO_REGEN", "RICH", "COMPLICATED", "WINTERFEAST", "HALLOWED_NIGHTS"
        -- If you set specific values below, they will override the preset's defaults.
        preset = "WINTERFEAST",

        -- Season lengths (in number of days)
        season_start = "AUTUMN", -- AUTUMN, WINTER, SPRING, SUMMER
        autumn = "VERY_LONG",         -- SHORT, DEFAULT, LONG, VERY_LONG
        winter = "SHORT",      -- SHORT, DEFAULT, LONG, VERY_LONG
        spring = "LONG",      -- SHORT, DEFAULT, LONG, VERY_LONG
        summer = "SHORT",      -- SHORT, DEFAULT, LONG, VERY_LONG

        -- World size
        world_size = "HUGE",   -- SMALL, MEDIUM, LARGE, HUGE, LUSH, WINTER, DESERT, Default Plus

        -- Starting resources
        start_action = "ALWAYS", -- NEVER, SOMETIMES, ALWAYS (e.g., fire pit, science machine)
        start_node = "DEFAULT",  -- DEFAULT, FOREST, GRASS, MARSH, SAVANNA, ROCKY, MIXED

        -- Weather settings
        weather = "LESS",     -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        lightning = "LESS",   -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        earthquakes = "LESS", -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        acidrain = "NONE",    -- NONE, LESS, DEFAULT, MORE, VERY_MORE
	meteorshowers = "default", -- "none", "little", "default", "more", "lots"
        frograin = "LESS",

        -- Disease (e.g., for berry bushes, birchnuts)
        disease = "DEFAULT",     -- NONE, SLOW, DEFAULT, FAST, VERY_FAST

        -- Branching (how biomes connect)
        branching = "DEFAULT",   -- NEWEVERYTHING, DEFAULT, NO_BRANCHING

        -- Loop (world edge connection)
        loop = "DEFAULT",        -- DEFAULT, NO_LOOP

        -- Connected (biomes more connected)
        connected = "DEFAULT",   -- DEFAULT, ALWAYS

        -- Biome distribution
        biome = "DEFAULT",       -- DEFAULT, MORE_ROCKS, MORE_FOREST, MORE_GRASS, MORE_MARSH, MORE_SAVANNA, NO_MARSH, NO_SAVANNA, NO_GRASS, NO_FOREST, NO_ROCKS

        -- Resource abundance (overall)
        resources = "VERY_MORE",   -- NONE, LESS, DEFAULT, MORE, VERY_MORE

        -- Creature settings
        mobs = "DEFAULT",        -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        giants = "DEFAULT",      -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        hound_attacks = "NONE",-- NONE, LESS, DEFAULT, MORE, VERY_MORE
        bees = "DEFAULT",        -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        spiders = "DEFAULT",     -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        penguins = "DEFAULT",    -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        walrus = "DEFAULT",      -- NONE, LESS, DEFAULT, MORE, VERY_MORE

        -- Specific feature settings
        berrybushes = "MORE", -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        grass = "DEFAULT",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        trees = "MORE",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        reeds = "MORE",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        rock = "MORE",        -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        flint = "DEFAULT",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        saplings = "DEFAULT",    -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        flowers = "DEFAULT",     -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        ponds = "DEFAULT",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        boulders = "DEFAULT",    -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        carrots = "DEFAULT",     -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        mushtrees = "DEFAULT",   -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        cacti = "DEFAULT",       -- NONE, LESS, DEFAULT, MORE, VERY_MORE
        lunar_plants = "DEFAULT",-- NONE, LESS, DEFAULT, MORE, VERY_MORE
        volcano_spawn = "DEFAULT",-- NONE, LESS, DEFAULT, MORE, VERY_MORE

        -- Player start location control
        start_location = "DEFAULT", -- DEFAULT, NO_DEFAULT

        -- Custom start parameters (often used by mods or very specific setups)
        -- Example: start_parameters = { ["start_area"] = "forest" }

        -- Custom overrides for specific biomes/features if needed
        -- For example:
        -- custom_resources = {
        --     ['FOREST'] = {
        --         ['TREES'] = "MORE",
        --         ['BERRYBUSHES'] = "NONE",
        --     },
        -- },
    }
}
