# ROGUELIKE
A roguelike with movement inspired by 'Fire Emblem'.
Made with raylib.
Platforms: Linux, Windows, Browser

# REQUIREMENTS
- raylib

# OPTIONAL REQUIREMENTS
- emscripten (for web build)

# TODOs
# active
- explicit constructor (with single argument?)
- [ ] Named parameter for setters:
  - inline Class& setA(T a){a_ = a; return * this;}
- [ ] Print tile info in panel


# Shortterm
- [ ] Map change
  - [ ] Trigger non-input by event system (pub-sub / observer pattern)
  - [ ] Print action in log panel
- [ ] Add enemy


# Longterm 
- [ ] Indicate select option (different cursor color on hover?)
- [ ] (Hold) KEY to show map overview
- [ ] Add touch support
- [ ] Remove zoom?


# Continuous
- Map generation

- Check for obsolete [[maybe_unused]] and //* comments


# Ideas
- [ ] Observer pattern for event communication
- [ ] Add logger
- [ ] Lua/Jinx settings file?


# Notes

## Controls

### Mouse
| Control     | Function                           |
| ----------- | ---------------------------------- |
| RMB         | Toggle cursor control (Mouse/Keys) |
| LMB         | Click to toggle select             |
| Mouse wheel | Zoom                               |


### Keyboard
| Control                           | Function            |
| --------------------------------- | ------------------- |
| WASD / Arrow keys                 | Cursor movement     |
| L_SHIFT                           | Fast cursor movment |
| CTRL + 0 (both numpad and normal) | Reset Zoom          |
| ESCAPE / CAPS_LOCK                | Deselect            |
| F1                                | Toggle debug mode   |
| F4                                | Exit                |
| F11                               | Maximize window     |
| H                                 | Center on hero      |


### Touch
Not implemented yet

## Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match

## Design guide
### Struct/Class general
- Struct: POD 
- Getter / Setter only when needed
- Only private member variables: name_

### Functions
- namespace: domain/related functions (interclass, non-class)
- Class: member if private member needed (class internal behaviour), free if no private member needed (eg. getter/setter)
- For readability only: no new file, but forward declare


## Refactor Composition instead of Inheritance
Refactor with (Interface) Composition when another version of the same type (:interface) is needed:

1) Old:
```cpp
class ConcreteComponent {
public:
    void reset() {};
    void update(int) {};
};

class Class {
public:
    Class(ConcreteComponent component)
    : component(component) {
        component.reset();
    }

    void process() {
        component.update(21);
    }

private:
    ConcreteComponent component;
};
```
2) New:
```cpp
class IComponentInterface {
public:
    virtual ~IComponent(){};
    virtual void reset() = 0;
    virtual void update(int) = 0;
};

class ConcreteComponent : public IComponentInterface {
public:
    void reset() override {};
    void update(int) override {};
};

class Class {
public:
    Class(IComponentInterface& interface)
    : interface(interface) {
        interface.reset();
    }

    void process() {
        interface.update(21);
    }

private:
    IComponentInterface& interface;
};
```


# Done
- [x] compile for linux & windows
- [x] Render sprites/textures
- [ ] make ECS (removed)
- [x] realize viewport/camera system
- [x] Implement zoom
- [x] Check class design (see notes); make diagram to track classes
- [ ] Command pattern for rendering and layers (not implemented -> need to handle abiguous keys in SparseSet: potential duplicate positions for tiles in different layers)
- [x] Implement movment
- [x] Refactor tilePosition to float position
- [x] Implement render area
- [x] edge pan stop relative to map not player
- [x] Implement vision (fog of war: unseen, seen, visible)
- [x] Refactor EdgePan relative to render area
- [x] Add RectangleEx (has left, top, right, bottom, height, width)
- [x] Add input to center on hero
- [x] Always use Setter/Getter (keep open for possible later changes / flexibility)
- [x] Refactor using spritesheet
