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

#define PLUGIN_VERSION "1.0.0-dev"

#include "configlist/list"
#include "configlist/commands"

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
 * Plugin is loading.
 */
public OnPluginStart()
{
    Commands_Create();
}
