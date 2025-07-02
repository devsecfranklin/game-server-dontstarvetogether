-- SPDX-FileCopyrightText: ©2025 franklin <franklin@bitsmasher.net>
--
-- SPDX-License-Identifier: MIT

return {
    ["workshop-3138571948"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-3436020204"]={
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
    ["workshop-382177939"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-2594707725"]={
        configuration_options={ },
        enabled=true
    },
    ["workshop-665868350"]={
        configuration_options={ },
        enabled=true
    },
        ["workshop-1077747217"]={ configuration_options={
        ANNOUNCE_OPEN = true , -- Announce
	SAY_OPEN = true , -- Say
	ANNOUNCE_CD = 20 , -- Announce Interval (min)
	ANNOUNCE_ORDER = true , -- Announce Order
	SAY_SHOWTIME = 3 , -- Say Show Time(sec)
	SAY_DELAY = 1 , -- Say Delay (sec)
	ANNOUNCE_TEXT = {
		-- Random players:%pds days %pns name, the number of players:%nws world %ncs cave.
		"we are streaming live at https://twitch.tv/s1y_b0rg",
		"thanks for playing!",
	} , -- Announce Message
		ANNOUNCE_FIRSTJION_TEXT = "everybody act cool, %pds is here" , -- First Join Announce Text
		SAY_FIRSTJION_TEXT = "welcome %pns. we are streaming live at https://twitch.tv/s1y_b0rg" , -- First Join Say Text
		SAY_EVERYJION_TEXT = "welcome back %pns, nerd. time to die." , -- Every Join Say Text
		SAY_DEATH_TEXT = "%pns u dead lol" , -- Death Say Text
		SAY_RESURRECTION_TEXT = "%pns returns from beyond the grave!" , -- Resurrection Say Text
    }, enabled=true },
}
