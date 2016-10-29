#include "scriptmine.h"

duk_context *ctx = NULL;
lua_State *Lg = NULL;
const char *name = NULL;

const char *air = "air";
const char *black_wool = "wool:black";
const char *blue_wool = "wool:blue";
const char *brown_wool = "wool:brown";
const char *cobblestone = "default:cobble";
const char *cyan_wool = "wool:cyan";
const char *diamond_block = "default:diamondblock";
const char *dirt = "default:dirt";
const char *glass_pane = "xpanes:pane";
const char *gold_block = "default:goldblock";
const char *gold_ore = "default:stone_with_gold";
const char *grass = "default:dirt_with_grass";
const char *gravel = "default:gravel";
const char *green_wool = "wool:dark_green";
const char *iron_ore = "default:stone_with_iron";
const char *lime_wool = "wool:green";
const char *magenta_wool = "wool:magenta";
const char *oakwood_plank = "default:wood";
const char *oak_wood = "default:tree";
const char *orange_wool = "wool:orange";
const char *pink_wool = "wool:pink";
const char *purple_wool = "wool:violet";
const char *red_wool = "wool:red";
const char *sand = "default:sand";
const char *spruce_fence = "default:fence_pine_wood";
const char *sprucewood_plank = "default:pine_wood";
const char *spruce_wood = "default:pine_tree";
const char *spruce_wood_stairs = "stairs:stair_pine_wood";
const char *stone = "default:stone";
const char *white_wool = "wool:white";
const char *yellow_wool = "wool:yellow";

const char *blockID_to_node_name(const char *blockID)
{
 if (strcmp(blockID, "1") == 0)
  return stone;
 else if (strcmp(blockID, "2") == 0)
  return grass;
 else if (strcmp(blockID, "3") == 0)
  return dirt;
 else if (strcmp(blockID, "4") == 0)
  return cobblestone;
 else if (strcmp(blockID, "5") == 0)
  return oakwood_plank;
 else if (strcmp(blockID, "5:1") == 0)
  return sprucewood_plank;
 else if (strcmp(blockID, "12") == 0)
  return sand;
 else if (strcmp(blockID, "13") == 0)
  return gravel;
 else if (strcmp(blockID, "14") == 0)
  return gold_ore;
 else if (strcmp(blockID, "15") == 0)
  return iron_ore;
 else if (strcmp(blockID, "17") == 0)
  return oak_wood;
 else if (strcmp(blockID, "17:1") == 0)
  return spruce_wood;
 else if (strcmp(blockID, "35") == 0)
  return white_wool;
 else if (strcmp(blockID, "35:1") == 0)
  return orange_wool;
 else if (strcmp(blockID, "35:2") == 0)
  return magenta_wool;
 else if (strcmp(blockID, "35:4") == 0)
  return yellow_wool;
 else if (strcmp(blockID, "35:5") == 0)
  return lime_wool;
 else if (strcmp(blockID, "35:6") == 0)
  return pink_wool;
 else if (strcmp(blockID, "35:9") == 0)
  return cyan_wool;
 else if (strcmp(blockID, "35:10") == 0)
  return purple_wool;
 else if (strcmp(blockID, "35:11") == 0)
  return blue_wool;
 else if (strcmp(blockID, "35:12") == 0)
  return brown_wool;
 else if (strcmp(blockID, "35:13") == 0)
  return green_wool;
 else if (strcmp(blockID, "35:14") == 0)
  return red_wool;
 else if (strcmp(blockID, "35:15") == 0)
  return black_wool;
 else if (strcmp(blockID, "41") == 0)
  return gold_block;
 else if (strcmp(blockID, "57") == 0)
  return gold_block;
 else if (strcmp(blockID, "102") == 0)
  return glass_pane;
 else if (strcmp(blockID, "134") == 0)
  return spruce_wood_stairs;
 else if (strcmp(blockID, "188") == 0)
  return spruce_fence;
 else
  return air;
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
