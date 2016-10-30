#include "scriptmine.h"
#include <dirent.h>

duk_context *ctx = NULL;
lua_State *Lg = NULL;
const char *name = NULL;

const char *air = "air";

const char *blockIDToNodeName[][2] =
{
 {"1", "default:stone"},
 {"2", "default:dirt_with_grass"},
 {"3", "default:dirt"},
 {"4", "default:cobble"},
 {"5", "default:wood"},
 {"5:1", "default:pine_wood"},
 {"12", "default:sand"},
 {"13", "default:gravel"},
 {"14", "default:stone_with_gold"},
 {"15", "default:stone_with_iron"},
 {"17", "default:tree"},
 {"17:1", "default:pine_tree"},
 {"31", "default:grass_1"},
 {"35", "wool:white"},
 {"35:1", "wool:orange"},
 {"35:2", "wool:magenta"},
 {"35:4", "wool:yellow"},
 {"35:5", "wool:green"},
 {"35:6", "wool:pink"},
 {"35:9", "wool:cyan"},
 {"35:10", "wool:violet"},
 {"35:11", "wool:blue"},
 {"35:12", "wool:brown"},
 {"35:13", "wool:dark_green"},
 {"35:14", "wool:red"},
 {"35:15", "wool:black"},
 {"37", "flowers:dandelion_yellow"},
 {"38", "flowers:rose"},
 {"41", "default:goldblock"},
 {"48", "default:mossycobble"},
 {"49", "default:obsidian"},
 {"50", "default:torch"},
 {"57", "default:diamondblock"},
 {"64", "doors:door_wood"},
 {"65", "default:ladder_wood"},
 {"98", "default:stonebrick"},
 {"102", "xpanes:pane"},
 {"109", "stairs:stair_stonebrick"},
 {"126", "stairs:slab_wood"},
 {"134", "stairs:stair_pine_wood"},
 {"188", "default:fence_pine_wood"}
};

const char *blockID_to_node_name(const char *blockIDOrig)
{
 char blockID[255];
 size_t len = strlen(blockIDOrig);
 if (len > 254)
  return air;
 strcpy(blockID, blockIDOrig);
 if ((len > 2) && (strcmp(blockIDOrig + len - 2, ":0") == 0))
  blockID[len - 2] = 0;
 int maxBlockID = sizeof(blockIDToNodeName) / 2 / (sizeof(const char *));
 for (int i = 0; i < maxBlockID; ++i)
 {
  if (strcmp(blockID, blockIDToNodeName[i][0]) == 0)
   return blockIDToNodeName[i][1];
 }
 return air;
}

const char *node_name_to_blockID(const char *node_name)
{
 int maxBlockID = sizeof(blockIDToNodeName) / 2 / (sizeof(const char *));
 for (int i = 0; i < maxBlockID; ++i)
 {
  if (strcmp(node_name, blockIDToNodeName[i][1]) == 0)
   return blockIDToNodeName[i][0];
 }
 return "0";
}

duk_ret_t echo(duk_context *ctx)
{
 const char *text = duk_to_lstring(ctx, 0 /*index*/, NULL);
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");
 lua_getfield(Lg, -1, "chat_send_player");
 lua_remove(Lg, -2);
 lua_pushstring(Lg, name);
 lua_pushstring(Lg, text);
 lua_pushboolean(Lg, 0);
 lua_call(Lg, 3, 1);
 return 1;
}

duk_ret_t readFile(duk_context *ctx)
{
 const char *text = duk_to_string(ctx, 0);
 if (strcmp(text, "drone") == 0)
  text = "drone/index";
 char *filename = malloc(strlen(text) + 200);
 strcpy(filename, "js/");
 strcat(filename, text);
 strcat(filename, ".js");
 FILE *f = fopen(filename, "r");
 if (NULL == f)
 {
  strcpy(filename, "js/drone/");
  strcat(filename, text);
  strcat(filename, ".js");
  f = fopen(filename, "r");
  if (NULL == f)
  {
   duk_push_undefined(ctx);
   return 1;
  }
 }
 fclose(f);
 duk_push_string_file(ctx, filename);
 free(filename);
 return 1;
}

duk_ret_t get_node(duk_context *ctx)
{
printf("get_node\n");
 int z = duk_to_int(ctx, -3);
 int y = duk_to_int(ctx, -2);
 int x = duk_to_int(ctx, -1);
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");  // [minetest]
 lua_getfield(Lg, -1, "get_node");  // [minetest get_node]
 lua_remove(Lg, -2);  // [get_node]
 lua_newtable(Lg);
 lua_pushnumber(Lg, x);
 lua_setfield(Lg, -2, "x");
 lua_pushnumber(Lg, y);
 lua_setfield(Lg, -2, "y");
 lua_pushnumber(Lg, z);
 lua_setfield(Lg, -2, "z");
 lua_call(Lg, 1, 0);
 lua_getfield(Lg, -1, "name");
 const char *node_name = lua_tostring(Lg, -1);
 const char *blockID = node_name_to_blockID(node_name);
 duk_idx_t ary_indx = duk_push_array(ctx);
 duk_push_string(ctx, blockID);
 duk_put_prop_string(ctx, ary_indx, "typeId");
 duk_push_string(ctx, node_name);
 duk_put_prop_string(ctx, ary_indx, "nodeName");
 lua_remove(Lg, -1);
 lua_remove(Lg, -1);
 return 1;
}

duk_ret_t place_node(duk_context *ctx)
{
 int z = duk_to_int(ctx, -4);
 int y = duk_to_int(ctx, -3);
 int x = duk_to_int(ctx, -2);
 const char *blockID = duk_to_string(ctx, -1);
 const char *nodeName = blockID_to_node_name(blockID);
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");  // [minetest]
 lua_getfield(Lg, -1, "place_node");  // [minetest new_node]
 lua_remove(Lg, -2);  // [set_node]
 lua_newtable(Lg);
 lua_pushnumber(Lg, x);
 lua_setfield(Lg, -2, "x");
 lua_pushnumber(Lg, y);
 lua_setfield(Lg, -2, "y");
 lua_pushnumber(Lg, z);
 lua_setfield(Lg, -2, "z");
 lua_newtable(Lg);
 lua_pushstring(Lg, nodeName);
 lua_setfield(Lg, -2, "name");
 lua_call(Lg, 2, 0);
 return 1;
}

duk_ret_t set_node(duk_context *ctx)
{
 int z = duk_to_int(ctx, -4);
 int y = duk_to_int(ctx, -3);
 int x = duk_to_int(ctx, -2);
 const char *blockID = duk_to_string(ctx, -1);
 const char *nodeName = blockID_to_node_name(blockID);
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");  // [minetest]
 lua_getfield(Lg, -1, "set_node");  // [minetest new_node]
 lua_remove(Lg, -2);  // [set_node]
 lua_newtable(Lg);
 lua_pushnumber(Lg, x);
 lua_setfield(Lg, -2, "x");
 lua_pushnumber(Lg, y);
 lua_setfield(Lg, -2, "y");
 lua_pushnumber(Lg, z);
 lua_setfield(Lg, -2, "z");
 lua_newtable(Lg);
 lua_pushstring(Lg, nodeName);
 lua_setfield(Lg, -2, "name");
 lua_call(Lg, 2, 0);
 return 1;
}

duk_ret_t get_player_location(duk_context *ctx)
{
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");  // [minetest]
 lua_getfield(Lg, -1, "get_player_by_name");  // [minetest get_player_by_name]
 lua_remove(Lg, -2);  // [get_player_by_name]
 lua_pushstring(Lg, name);  // [get_player_by_name name]
 lua_call(Lg, 1, 1);  // [player]
 lua_getmetatable(Lg, -1);  // [player metatable]
 lua_pushstring(Lg, "__index");  // [player metatable __index]
 lua_rawget(Lg, -2);  // [player metatable __index]
 lua_pushstring(Lg, "getpos");  // [player metatable __index getpos]
 lua_rawget(Lg, -2);  // [player metatable __index getpos]
 lua_pushvalue(Lg, -4);  // [player metatable __index getpos player]
 lua_call(Lg, 1, 1); // [player metatable __index postable]
 lua_getfield(Lg, -1, "x");
 int x = lua_tointeger(Lg, -1);
 lua_remove(Lg, -1);
 lua_getfield(Lg, -1, "y");
 int y = lua_tointeger(Lg, -1);
 lua_remove(Lg, -1);
 lua_getfield(Lg, -1, "z");
 int z = lua_tointeger(Lg, -1);
 lua_remove(Lg, -1);
 lua_remove(Lg, -1); // [player metatable __index]
 lua_pushstring(Lg, "get_look_dir");
 lua_rawget(Lg, -2); // [player metatable __index get_look_dir]
 lua_pushvalue(Lg, -4);  // [player metatable __index get_look_dir player]
 lua_call(Lg, 1, 1); // [player metatable __index postable]
 lua_getfield(Lg, -1, "x");
 double xv = lua_tonumber(Lg, -1);
 lua_remove(Lg, -1);
 lua_getfield(Lg, -1, "z");
 double zv = lua_tonumber(Lg, -1);
 lua_remove(Lg, -1);
 lua_remove(Lg, -1); // [player metatable __index]
 lua_remove(Lg, -1); // [player metatable]
 lua_remove(Lg, -1); // [player]
 lua_remove(Lg, -1);
 int facing = 0;
 if (fabs(xv) > fabs(zv))
 {
  if (xv > 0)
  {
   facing = 1;
  }
  else
   facing = 3;
 }
 else
 {
  if (zv > 0)
  {
   facing = 0;
  }
  else
  {
   facing = 2;
  }
 }
 y += 1;
 duk_idx_t ary_indx = duk_push_array(ctx);
 duk_push_int(ctx, z);
 duk_put_prop_string(ctx, ary_indx, "x");
 duk_push_int(ctx, y);
 duk_put_prop_string(ctx, ary_indx, "y");
 duk_push_int(ctx, x);
 duk_put_prop_string(ctx, ary_indx, "z");
 duk_push_int(ctx, facing);
 duk_put_prop_string(ctx, ary_indx, "facing");
 return 1;
}

static int javascript(lua_State *L)
{
 const char *cmd = lua_tostring(L, -1);
 name = lua_tostring(L, -2);
 Lg = L;
 duk_peval_string(ctx, cmd);
 return 1;
}

static void loadplugins(const char *modpath, const char *pluginpath)
{
 char fullpath[2048];
 strcpy(fullpath, modpath);
 strcat(fullpath, "/");
 strcat(fullpath, pluginpath);
 DIR *dir = opendir(fullpath);
 if (dir)
 {
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
   if (entry->d_type & DT_DIR)
   {
    if ((strcmp("..", entry->d_name) != 0) && (strcmp(".", entry->d_name) != 0))
    {
     char *tmppath = malloc(strlen(pluginpath) + 2 + strlen(entry->d_name));
     strcpy(tmppath, pluginpath);
     strcat(tmppath, "/");
     strcat(tmppath, entry->d_name);
     loadplugins(modpath, tmppath);
     free(tmppath);
    }
   }
   else
   {
    int len = strlen(entry->d_name);
    if ((len > 3) && (strcmp(entry->d_name + len - 3, ".js") == 0))
    {
     char *tmppath = malloc(strlen(pluginpath) + 2 + len);
     strcpy(tmppath, pluginpath);
     strcat(tmppath, "/");
     strcat(tmppath, entry->d_name);
     tmppath[strlen(pluginpath) + 1 + len - 3] = 0;
     duk_push_global_object(ctx);
     duk_push_string(ctx, "require");
     duk_get_prop(ctx, -2);
     duk_push_string(ctx, tmppath);
     duk_call(ctx, 1);
     duk_pop_n(ctx, 2);
     free(tmppath);
    }
   }
  }
 }
}

static int loadscript(lua_State *L)
{
 const char *modpath = lua_tostring(L, -1);
 char *fullpath = malloc(strlen(modpath) + 10);
 strcpy(fullpath, modpath);
 strcat(fullpath, "/js/scriptmine.js");
 if (duk_peval_file_noresult(ctx, fullpath) != 0) {
  printf("Error: %s\n", duk_safe_to_string(ctx, -1));
  exit(1);
 }
 strcpy(fullpath, modpath);
 strcat(fullpath, "/js");
 loadplugins(fullpath, "plugins");
 free(fullpath);
 return 1;
}

int luaopen_scriptmine(lua_State *L)
{
 ctx = duk_create_heap_default();
 duk_push_global_object(ctx);
 duk_push_c_function(ctx, echo, 1);
 duk_put_prop_string(ctx, -2, "echo");
 duk_push_c_function(ctx, readFile, 1);
 duk_put_prop_string(ctx, -2, "readFile");
 duk_push_c_function(ctx, get_node, 3);
 duk_put_prop_string(ctx, -2, "get_node");
 duk_push_c_function(ctx, place_node, 3);
 duk_put_prop_string(ctx, -2, "place_node");
 duk_push_c_function(ctx, set_node, 4);
 duk_put_prop_string(ctx, -2, "set_node");
 duk_push_c_function(ctx, get_player_location, 0);
 duk_put_prop_string(ctx, -2, "get_player_location");
 if (!ctx)
 {
  printf("Failed to create a Duktape heap.\n");
  exit(1);
 }
 lua_register(L, "javascript", javascript);
 lua_register(L, "loadscript", loadscript);
	return 0;
}
