#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "duktape.h"

duk_context *ctx = NULL;
lua_State *Lg = NULL;
const char *name = NULL;

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
 duk_eval_string(ctx, cmd);
	return 1;
}

int luaopen_scriptmine(lua_State *L)
{
 ctx = duk_create_heap_default();
 duk_push_global_object(ctx);
 duk_push_c_function(ctx, echo, 1 /*nargs*/);
 duk_put_prop_string(ctx, -2, "echo");
 if (!ctx) {
  printf("Failed to create a Duktape heap.\n");
  exit(1);
 }
 lua_register(L, "javascript", javascript);
	return 0;
}
