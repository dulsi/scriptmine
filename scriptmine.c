#include "scriptmine.h"

duk_context *ctx = NULL;
lua_State *Lg = NULL;
const char *name = NULL;

const char *air = "air";
const char *cobblestone = "default:cobble";
const char *dirt = "default:dirt";
const char *grass = "default:dirt_with_grass";
const char *gravel = "default:gravel";
const char *oakwood_plank = "default:wood";
const char *oak_wood = "default:tree";
const char *sand = "default:sand";
const char *sprucewood_plank = "default:pine_wood";
const char *spruce_wood = "default:pine_tree";
const char *stone = "default:stone";

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
 else if (strcmp(blockID, "17") == 0)
  return oak_wood;
 else if (strcmp(blockID, "17:1") == 0)
  return spruce_wood;
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
 strcat(fullpath, "/drone.js");
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
 drone_init(ctx);
 duk_push_global_object(ctx);
 duk_push_c_function(ctx, echo, 1 /*nargs*/);
 duk_put_prop_string(ctx, -2, "echo");
 if (!ctx)
 {
  printf("Failed to create a Duktape heap.\n");
  exit(1);
 }
 lua_register(L, "javascript", javascript);
 lua_register(L, "loadscript", loadscript);
	return 0;
}
