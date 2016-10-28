require("scriptmine")

loadscript(minetest.get_modpath(minetest.get_current_modname()))

minetest.register_chatcommand("js", {
	params = "<command>",
	description = "Run javascript command",
	func = function(name, param)
  javascript(name, param);
	end,
})
