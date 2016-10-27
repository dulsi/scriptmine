#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "duktape.h"

extern lua_State *Lg;
extern const char *name;

extern const char *blockID_to_node_name(int blockID);
extern void drone_init(duk_context *ctx);
