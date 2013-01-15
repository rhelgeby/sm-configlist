/*
 * ============================================================================
 *
 *  Config List
 *
 *  File:          commands.inc
 *  Type:          Include
 *  Description:   Manages console commands.
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

Commands_Create()
{
    RegAdminCmd("cfglist", Command_List, ADMFLAG_RCON, "Displays all named lists or the contents of a list. Usage: cfglist [name]");
    RegAdminCmd("cfglist_create", Command_Create, ADMFLAG_RCON, "Creates a named list. Usage: cfglist_create <name>");
    RegAdminCmd("cfglist_delete", Command_Delete, ADMFLAG_RCON, "Deletes a named list. Usage: cfglist_delete <name>");
    
    RegAdminCmd("cfglist_add", Command_Add, ADMFLAG_RCON, "Adds a config entry to a list. Usage: cfglist_add <name> <path>");
    RegAdminCmd("cfglist_remove", Command_Remove, ADMFLAG_RCON, "Removes a config entry from a list. Usage: cfglist_add <name> <path>");
    
    RegAdminCmd("cfglist_exec_random", Command_ExecRandom, ADMFLAG_RCON, "Execute a random config file in a list. Usage: cfglist_exec_random <name>");
    RegAdminCmd("cfglist_exec_list", Command_ExecList, ADMFLAG_RCON, "Executes every config file in a list sequentially. Usage: cfglist_exec_list <name>");
}

/*____________________________________________________________________________*/

public Action:Command_List(client, argc)
{
    if (argc == 0)
    {
        // Dump lists and info.
        ReplyToCommand(client, "Displays all named lists or the contents of a list. Usage: cfglist [name]\n");
        List_DumpNames(client);
        ReplyToCommand(client, "\nCommands: cfglist, cfglist_create, cfglist_delete, cfglist_add, cfglist_remove");
        
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Validate list.
    new Handle:list;
    if (!List_GetList(listName, list))
    {
        ReplyToCommand(client, "Invalid list name.");
        return Plugin_Handled;
    }
    
    // Dump list contents.
    List_DumpList(client, list);
    
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_Create(client, argc)
{
    if (argc == 0)
    {
        ReplyToCommand(client, "Creates a named list. Usage: cfglist_create <name>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Create list.
    if (!List_Create(listName))
    {
        ReplyToCommand(client, "List already exist.");
        return Plugin_Handled;
    }
    
    ReplyToCommand(client, "List created.");
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_Delete(client, argc)
{
    if (argc == 0)
    {
        ReplyToCommand(client, "Deletes a named list. Usage: cfglist_delete <name>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Delete list.
    if (!List_Delete(listName))
    {
        ReplyToCommand(client, "Invalid list name.");
        return Plugin_Handled;
    }
    
    ReplyToCommand(client, "List deleted.");
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_Add(client, argc)
{
    if (argc < 2)
    {
        ReplyToCommand(client, "Adds a config entry to a list. Usage: cfglist_add <name> <path>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Get path.
    new String:path[PLATFORM_MAX_PATH];
    GetCmdArg(2, path, sizeof(path));
    
    // Validate list.
    if (!List_GetList(listName))
    {
        ReplyToCommand(client, "Invalid list name: \"%s\"", listName);
        return Plugin_Handled;
    }
    
    // Check if it already exists.
    if (List_GetEntryIndex(listName, path) >= 0)
    {
        ReplyToCommand(client, "Config entry already exists in list: \"%s\"", path);
        return Plugin_Handled;
    }
    
    // Validate path.
    new String:fullPath[PLATFORM_MAX_PATH];
    Format(fullPath, sizeof(fullPath), "cfg/%s", path);
    if (!FileExists(fullPath, true))
    {
        ReplyToCommand(client, "File not found: \"%s\"", path);
        return Plugin_Handled;
    }
    
    // Add to list.
    new entry = List_AddEntry(listName, path, false);
    ReplyToCommand(client, "Config entry added at index %d.", entry);
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_Remove(client, argc)
{
    if (argc < 2)
    {
        ReplyToCommand(client, "Removes a config entry from a list. Usage: cfglist_add <name> <path>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Get path.
    new String:path[PLATFORM_MAX_PATH];
    GetCmdArg(2, path, sizeof(path));
    
    // Validate list.
    if (!List_GetList(listName))
    {
        ReplyToCommand(client, "Invalid list name: \"%s\"", listName);
        return Plugin_Handled;
    }
    
    // Validate path.
    if (List_GetEntryIndex(listName, path) < 0)
    {
        ReplyToCommand(client, "Invalid path", path);
        return Plugin_Handled;
    }
    
    // Remove from list.
    List_RemoveEntry(listName, path);
    ReplyToCommand(client, "Config entry removed.");
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_ExecRandom(client, argc)
{
    if (argc == 0)
    {
        ReplyToCommand(client, "Execute a random config file in a list. Usage: cfglist_exec_random <name>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Validate and get list.
    new Handle:list;
    if (!List_GetList(listName, list))
    {
        ReplyToCommand(client, "Invalid list name: \"%s\"", listName);
        return Plugin_Handled;
    }
    
    // Send exec command.
    if (!ListAction_ExecuteRandom(list))
    {
        ReplyToCommand(client, "List is empty.");
        return Plugin_Handled;
    }
    
    return Plugin_Handled;
}

/*____________________________________________________________________________*/

public Action:Command_ExecList(client, argc)
{
    if (argc == 0)
    {
        ReplyToCommand(client, "Executes every config file in a list sequentially. Usage: cfglist_exec_list <name>");
        return Plugin_Handled;
    }
    
    // Get list name.
    new String:listName[64];
    GetCmdArg(1, listName, sizeof(listName));
    
    // Validate and get list.
    new Handle:list;
    if (!List_GetList(listName, list))
    {
        ReplyToCommand(client, "Invalid list name: \"%s\"", listName);
        return Plugin_Handled;
    }
    
    // Send exec command.
    if (!ListAction_ExecuteSequential(list))
    {
        ReplyToCommand(client, "List is empty.");
        return Plugin_Handled;
    }
    
    return Plugin_Handled;
}
