function box()
{
 var d = new Drone();
 if (arguments.length >= 4)
  d.box(arguments[0], arguments[1], arguments[2], arguments[3]);
 else if (arguments.length == 3)
  d.box(arguments[0], arguments[1], arguments[2]);
 else if (arguments.length == 2)
  d.box(arguments[0], arguments[1]);
 else if (arguments.length == 1)
  d.box(arguments[0]);
 return d;
}

function box0()
{
 var d = new Drone();
 if (arguments.length >= 4)
  d.box0(arguments[0], arguments[1], arguments[2], arguments[3]);
 else if (arguments.length == 3)
  d.box0(arguments[0], arguments[1], arguments[2]);
 else if (arguments.length == 2)
  d.box0(arguments[0], arguments[1]);
 else if (arguments.length == 1)
  d.box0(arguments[0]);
 return d;
}
