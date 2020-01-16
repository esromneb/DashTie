# DashTie aka Meteor Dashboard
This is a c++ "library" that gives simple control over a meteor js dashboard

# Basics
* The library takes a type, and read-only pointer to a variable.
* With the previous you also provide a json "path"
** the path is something like `["a", "b", "c"]`.  This path can be any length long, and it describes a path down into a json object
** For instance the previous would create an object like:
```
{"a":{"b":{"c":true}}}
```
