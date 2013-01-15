/*
 * ============================================================================
 *
 *  Config List
 *
 *  File:          actions.inc
 *  Type:          Include
 *  Description:   List actions.
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

/**
 * Executes a list of config files sequentially.
 *
 * @param list      List of config file paths.
 *
 * @return          True if config exec commands was sent, false if list is
 *                  empty.
 */
bool:ListAction_ExecuteSequential(Handle:list)
{
    new len = GetArraySize(list);
    
    if (len == 0)
    {
        return false;
    }
    
    new String:path[PLATFORM_MAX_PATH];
    
    // Loop through config files.
    for (new i = 0; i < len; i++)
    {
        // Get path.
        GetArrayString(list, i, path, sizeof(path));
        
        // Send exec command.
        ListAction_ExecuteConfig(path);
    }
    
    return true;
}

/*____________________________________________________________________________*/

/**
 * Execute one random config file in the list.
 *
 * @param list      List of config file paths.
 *
 * @return          True if config exec command was sent, false if list is
 *                  empty.
 */
bool:ListAction_ExecuteRandom(Handle:list)
{
    new len = GetArraySize(list);
    
    if (len == 0)
    {
        return false;
    }
    
    // Get a random index.
    new index = Math_GetRandomInt(0, len - 1);
    
    // Get path.
    new String:path[PLATFORM_MAX_PATH];
    GetArrayString(list, index, path, sizeof(path));
    
    // Send exec command.
    ListAction_ExecuteConfig(path);
    return true;
}

/*____________________________________________________________________________*/

/**
 * Executes a config file.
 *
 * @param path      Config file path.
 */
ListAction_ExecuteConfig(const String:path[])
{
    ServerCommand("exec %s", path);
}
