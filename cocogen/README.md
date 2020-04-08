This is the cocogen module and generated a the cocogen executable.

It is separated in multiple modules.
Every module determines its own include path, every path can be included from this directory.
So, including a file from filegen can be done via  "filegen/file".
Some modules export other includes,  like the ast module, which exports the ast directory,
so including can be done via "ast/file". Keep these include paths clear.

TODO: maybe move the commandline stuff in its own module.
TODO: Maybe have a clearer distinction between modules, so a separate directory for all backends but I do not want to create too much depth.
Just want to keep the structure clear.