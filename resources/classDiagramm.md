```mermaid
classDiagram 
A ..> B : uses
note for A "as parameter" 
C --> D : holds 
note for C "reference"
E --|> F : is-a 
note for E "inherits base"
G ..|> H : implements 
note for G "inherits interface"
I o-- J : has 
note for I "copied instance"
K *-- L : owns 
note for K "created instance"
```

```mermaid
classDiagram 
    class main {
        + ActiveScene activeScene
        + appLoop() void
    }
    main *-- Scene : owns
    main ..> ActiveScene : uses

    class Scene {
        <<Interface>>
        + update()* void
        + initialize()* void
        + deinitialize()* void
        + processInput()* void
        + updateState()* void
        + renderOutput()* void
    }
    Scene ..> ActiveScene : uses

    class ActiveScene {
        - Scene scene
        - ActiveScene instance
        + get() Scene
        + set(Scene)
        + getInstance() ActiveScene
    }
    ActiveScene --> ActiveScene


    class GameScene {
    }
    GameScene ..|> Scene : implements
    GameScene *-- Renderer : owns
    GameScene *-- Controller : owns

    class Renderer {
        + render(Renderable) void
    }
    Renderer --> Renderable : uses

    class Renderable {
        <<Interface>>
        + render()* void
    }

    class Positional {
        <<Interface>>
        - Vector2 position
        + getPosition() Vector2
        + setPosition(Vector2) void
    }

    class Controller {
        + move(Positional, Direction, distance) void
    }

    class Map { 
        + MapEntity[] fields
        + Tile[] tiles
        + Unit[] units
        + Item[] items
    }
    note for Map "key-value-container"
    Map *-- MapEntity : has
    Map *-- Tile : has
    Map *-- Item : has
    Map *-- Unit : has

    class MapEntity {
        Tile tile
        Item item
        Unit unit
    }
    MapEntity ..|> Positional : implements
    MapEntity --> Tile : holds
    MapEntity --> Item : holds
    MapEntity --> Unit : holds

    class Tile {
        <<Interface>>
        - bool passable
        + isPassable() bool
    }

    class Wall
    Wall ..|> Tile : implements
    Wall ..|> Positional : implements
    Wall ..|> Renderable : implements
    
    class Floor 
    Floor ..|> Tile : implements
    Floor ..|> Positional : implements
    Floor ..|> Renderable : implements


    class Unit {
        <<Interface>>
        - int Health
        + getHealth() int
        + setHealth(health) void
    }

    class Player 
    Player ..|> Unit : implements
    Player ..|> Positional : implements
    Player ..|> Renderable : implements

    class Item 
    Item ..|> Positional : implements
    Item ..|> Renderable : implements
```
