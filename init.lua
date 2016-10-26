require("scriptmine")

minetest.register_chatcommand("js", {
	params = "<command>",
	description = "Run javascript command",
	func = function(name, param)
  javascript(name, param);
	end,
})
