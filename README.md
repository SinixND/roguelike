# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[0] make filterAccessible() utility function that uses **TIsSolid** to return list of tiles


# Shortterm
[0] make filterInRange() utitliy function that return a list of tiles

[1] make filterReachable() utitliy function that uses isInRange() to return a list of tiles

[2] use filterReachable() to filter considered tiles in findPath()



# Longterm 
[1] Transform (S) relative to **selected** (C) (Cursor if unselected)

[1] Map generation

# Continuous


# Notes
ECS-Analogy:
- Class members become components, boolean members (states) become tags (= component without data)
- Class methods become systems
- Objects become Entities

Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match



# Done
[x] compile for linux & windows !

[x] Render sprites/textures

[x] make ECS

[x] realize viewport system (render relative to window -> update Rendersystem: pos 0,0 & transform 0, 0 == center of screen)

[x] adjust texture to default direction right

[x] Refactor Map as Entity that has (Tile)Entities-Component which holds ar array of entities, created by SMapSystem that holds internally a static level info, creates map if level is not equal Configs entry (game state)