/*
 * ============================================================================
 *
 *  Config List
 *
 *  File:          configlist.sp
 *  Type:          Main
 *  Description:   Stores named lists of config files and lets you apply actions
 *                 on the list through console commands.
 *
 *  Copyright (C) 2013  Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

// Comment out to not require semicolons at the end of each line of code.
#pragma semicolon 1

#include <sourcemod>

#define PLUGIN_VERSION "1.1.0"

#include "configlist/list"
#include "configlist/commands"
#include "configlist/actions"

/*____________________________________________________________________________*/

/**
 * Record plugin info.
 */
public Plugin:myinfo =
{
    name = "Config List",
    author = "Richard Helgeby",
    description = "Stores lists of config files and let you apply actions on them.",
    version = PLUGIN_VERSION,
    url = "http://www.sourcemod.net"
};

/*____________________________________________________________________________*/

/**
 * Plugin is about to load. Register library so other plugins can use this
 * plugin.
 */
public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
    RegPluginLibrary("configlist");
    return APLRes_Success;
}

/*____________________________________________________________________________*/

/**
 * Plugin is loading.
 */
public OnPluginStart()
{
    CreateConVar("configlist_version", PLUGIN_VERSION, "ConfigList version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY);
    
    Commands_Create();
}

/*____________________________________________________________________________*/

/**
 * (from SMLIB 0.10.2)
 * 
 * Returns a random, uniform Integer number in the specified (inclusive) range.
 * This is safe to use multiple times in a function.
 * The seed is set automatically for each plugin.
 * Rewritten by MatthiasVance, thanks.
 * 
 * @param min			Min value used as lower border
 * @param max			Max value used as upper border
 * @return				Random Integer number between min and max
 */
#define SIZE_OF_INT		2147483647		// without 0
stock Math_GetRandomInt(min, max)
{
	new random = GetURandomInt();
	
	if (random == 0) {
		random++;
	}

	return RoundToCeil(float(random) / (float(SIZE_OF_INT) / float(max - min + 1))) + min - 1;
}
