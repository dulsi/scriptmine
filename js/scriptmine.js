Duktape.modSearch = function (id)
{
 var res;

 print('loading module:', id);

 res = readFile(id);
 if (typeof res === 'string')
 {
  return res;
 }

 throw new Error('module not found: ' + id);
}

function smBlock(typeId, nodeName)
{
 this.typeId = typeId;
 this.nodeName = nodeName;
 return this;
}

var global = this;
var blocks = require("blocks");
var Drone = require("drone");
