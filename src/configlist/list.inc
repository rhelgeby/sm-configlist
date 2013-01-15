/*
 * ============================================================================
 *
 *  Config List
 *
 *  File:          list.inc
 *  Type:          Include
 *  Description:   Manages a list of config lists.
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
 * Trie with list names mapped to lists.
 */
new Handle:List = INVALID_HANDLE;

/**
 * Creates a new list.
 *
 * @param listName      Name of list.
 *
 * @return              True if successful, false if list name already exist.
 */
bool:List_Create(const String:listName[])
{
    if (List_GetList(listName))
    {
        // List already exist.
        return false;
    }
    
    // Create array for storing config file paths.
    new Handle:list = CreateArray(ByteCountToCells(PLATFORM_MAX_PATH));
    
    SetTrieValue(List, listName, list);
    return true;
}

/**
 * Deletes a list.
 *
 * @param listName      Name of list.
 *
 * @return              True if successful, false if invalid list name.
 */
bool:List_Delete(const String:listName[])
{
    new Handle:list;
    
    if (!List_GetList(listName, list))
    {
        // List doesn't exist.
        return false;
    }
    
    // Delete list.
    CloseHandle(list);
    
    // Update name index.
    RemoveFromTrie(List, listName);
    
    return true;
}

/**
 * Adds an entry to the list.
 *
 * @param listName      Name of list.
 * @param createList    (Optional) Create the list if it doesn't exist. Default
 *                      is true.
 *
 * @return              Entry index.
 *
 * @error               Invalid list name or entry already exist.
 */
List_AddEntry(const String:listName[], const String:entryPath[], bool:createList = true)
{
    new Handle:list;
    
    // Validate and get list.
    if (!List_GetList(listName, list))
    {
        if (!createList)
        {
            ThrowError("Invalid list name (%s).", listName);
        }
        if (!List_Create(listName))
        {
            ThrowError("List name already exist (%s).", listName);
        }
    }
    
    // Validate entry.
    new entry = List_GetEntryIndex(listName, entryPath);
    if (entry >= 0)
    {
        ThrowError("Entry already exist (%s).", entryPath);
    }
    
    return PushArrayString(list, entryPath);
}

/**
 * Gets a list entry at the specified index.
 *
 * @param listName      Name of list.
 * @param index         Entry index.
 * @param buffer        (Output) Buffer to store path to config file.
 * @param maxlen        Size of buffer.
 *
 * @return              Number of characters copied.
 *
 * @error               Invalid list name or entry index.
 */
List_GetEntryAt(const String:listName[], index, String:buffer[], maxlen)
{
    new Handle:list;
    
    // Validate and get list.
    if (!List_GetList(listName, list))
    {
        ThrowError("Invalid list name (%s).", listName);
    }
    
    // Validate entry index.
    if (!List_IsValidEntryIndex(list, index))
    {
        ThrowError("Invalid entry index (%d).", index);
    }
    
    return GetArrayString(list, index, buffer, maxlen);
}

/**
 * Removes a list entry.
 *
 * @param listName      Name of list.
 * @param entryPath     Path to config file entry to remove.
 *
 * @error               Invalid list name or entry path.
 */
List_RemoveEntry(const String:listName[], const String:entryPath[])
{
    new Handle:list;
    
    // Validate and get list.
    if (!List_GetList(listName, list))
    {
        ThrowError("Invalid list name (%s).", listName);
    }
    
    // Validate entry.
    new entry = List_GetEntryIndex(listName, entryPath);
    if (entry < 0)
    {
        ThrowError("Invalid entry (%s).", entryPath);
    }
    
    RemoveFromArray(list, entry);
}

/**
 * Removes a list entry at the specified index.
 *
 * @param listName      Name of list.
 * @param index         Entry index.
 *
 * @error               Invalid list name or entry index.
 */
List_RemoveEntryAt(const String:listName[], index)
{
    new Handle:list;
    
    // Validate and get list.
    if (!List_GetList(listName, list))
    {
        ThrowError("Invalid list name (%s).", listName);
    }
    
    // Validate entry.
    if (!List_IsValidEntryIndex(list, index))
    {
        ThrowError("Invalid entry index (%d).", index);
    }
    
    RemoveFromArray(list, index);
}

/**
 * Gets the index of an entry.
 *
 * @param listName      Name of list.
 * @param entryPath     Path to config file entry to remove.
 *
 * @return              Entry index, or -1 if entry does not exist.
 *
 * @error               Invalid list name.
 */
List_GetEntryIndex(const String:listName[], const String:entryPath[])
{
    new Handle:list;
    
    // Validate and get list.
    if (!List_GetList(listName, list))
    {
        ThrowError("Invalid list name (%s).", listName);
    }
    
    return FindStringInArray(list, entryPath);
}

/**
 * Gets the specified list.
 *
 * @param listName      List name to validate.
 * @param list          (Output) Buffer to store list handle, if valid list
 *                      name.
 *
 * @return              True if succesful, false if invalid list name.
 */
bool:List_GetList(const String:listName[], &Handle:list = INVALID_HANDLE)
{
    if (GetTrieValue(List, listName, list))
    {
        return true;
    }
    
    return false;
}

/**
 * Returns whether a entry index is valid.
 *
 * @param list          Handle to list.
 * @param index         Index to validate.
 *
 * @return              True if valid, false otherwise.
 */
bool:List_IsValidEntryIndex(Handle:list, index)
{
    new len = GetArraySize(list);
    if (len == 0 || index < 0 || index >= len)
    {
        return false;
    }
    
    return true;
}