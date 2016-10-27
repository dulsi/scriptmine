#include "scriptmine.h"

duk_ret_t drone_box(duk_context *ctx)
{
 duk_push_this(ctx); // [b w h d this]
 duk_get_prop_string(ctx, -1, "x");
 int x = duk_to_int(ctx, -1);
 duk_remove(ctx, -1);
 duk_get_prop_string(ctx, -1, "y");
 int y = duk_to_int(ctx, -1);
 duk_remove(ctx, -1);
 duk_get_prop_string(ctx, -1, "z");
 int z = duk_to_int(ctx, -1);
 duk_remove(ctx, -1);
 int blockID = duk_to_int(ctx, -5);
 const char *nodeName = blockID_to_node_name(blockID);
 lua_getfield(Lg, LUA_GLOBALSINDEX, "minetest");  // [minetest]
 lua_getfield(Lg, -1, "set_node");  // [minetest get_player_by_name]
 lua_remove(Lg, -2);  // [get_player_by_name]
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
 return 0;
}

duk_ret_t drone_constructor(duk_context *ctx)
{
 if (!duk_is_constructor_call(ctx))
 {
  return DUK_RET_TYPE_ERROR;
 }

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
   facing = 2;
   x += 2;
  }
  else
   x -= 2;
 }
 else
 {
  if (zv > 0)
  {
   facing = 0;
   z += 2;
  }
  else
  {
  facing = 3;
   z -= 2;
  }
 }
 y += 1;
 duk_push_this(ctx);
 duk_push_number(ctx, x);
 duk_put_prop_string(ctx, -2, "x");
 duk_push_number(ctx, y);
 duk_put_prop_string(ctx, -2, "y");
 duk_push_number(ctx, z);
 duk_put_prop_string(ctx, -2, "z");
 duk_push_number(ctx, facing);
 duk_put_prop_string(ctx, -2, "facing");

 return 0;  /* use default instance */
}

void drone_init(duk_context *ctx)
{
 duk_push_c_function(ctx, drone_constructor, 0);
 duk_push_object(ctx);
 duk_push_c_function(ctx, drone_box, 4);
 duk_put_prop_string(ctx, -2, "box");
 duk_put_prop_string(ctx, -2, "prototype");
 duk_put_global_string(ctx, "Drone");
}