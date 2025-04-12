# McServerShell

Maybe you have seen [MCDReforged](https://github.com/MCDReforged/MCDReforged), which provides a plugin system
for vanilla servers. My goal is to implement something similar and light-weigt while still allowing the user
to have full control about actual the actual server execution.

So basically, this is a NCurses based TUI for a Minecraft server (or any other program for that matter)

## why is there this random python file?

manycolors.py is basically a tiny test program to check if text coloring is working. This is important since
NCurses uses its own color definitions and just recieving ANSI color codes doesn't actually end up coloring
anything.
