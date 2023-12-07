# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# Current
[0] compile for linux & windows !

# Shortterm
[0] plan struchture with UML

# Longterm 
[0] implement environment

# Notes

## UML

```mermaid
classDiagram
    class main
    main *-- Scene : consists-of

    class Scene {
        <<abstract>>
        + update() void
        + initialize()* void
        + deinitialize()* void
    }
    Scene <|.. GameREWORKME : is-a

    class GameREWORKME{
        -Character player
    }
    GameREWORKME *-- Character : consists-of

    class Renderer{
        <<abstract>>
        + render(Renderable) void
    }
    Renderer <|-- GameREWORKME : is-a
    Renderer --> Renderable : uses

    class Renderable{
        <<Interface>>
        + render()* void
    }
    Renderable <|-- Character : is-a

    class Character
```

# Done
[x] Render unit
