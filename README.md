# ROGUELIKE
A roguelike made with raylib.
Platforms: Linux, Windows, Browser

# REQUIREMENTS
- raylib

# OPTIONAL REQUIREMENTS
- emscripten (for web build)

# TODOs
# active
- [ ] Fire event on camera change
- [ ] Check for triggered functions -> pub-sub functor
    - Keep filtered list of map tiles to render
- [ ] Redesign to usual rogue controls (remove fire emblem like controls)
    - [ ] Character triggers edgePan


# Shortterm
- [ ] Changeable key bindings
    - Does GameScene(?) hold Buttons that get funcitons assigned? (https://gameprogrammingpatterns.com/command.html)
    - Or should actions be called via events? (see next point)
- [ ] Refactor classes to DOD (if object count > 1)
    - For DOD: Class/Object owns component-container (SparseSet for individual access)
    - For OOP: Object owns individual component
        - Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()]
        - Choose closest scope parameter that contains all information needed (member > object)


# Longterm 
- [ ] Add rogue theme (ASCII)
    - Floor : '.'
    - Wall  : '#'
    - Hero  : '@'
    - Stairs: '%'
- [ ] Map change
- [ ] Print the action in log panel
- [ ] Add enemy
    - Therefore World holds list of Units
- [ ] (Hold) KEY to show map overview
- [ ] Add touch support
- [ ] Remove zoom?
- [ ] Save/Load Account > Character > CurrentRun
- [ ] Account settings via lua


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
| Control                           | Function                |
| --------------------------------- | ----------------------- |
| WASD / Arrow keys                 | Hero movement           |
| L_SHIFT                           | Continuous hero movment |
| CTRL + 0 (both numpad and normal) | Reset Zoom              |
| F1                                | Toggle developer mode   |
| F4                                | Exit                    |
| F11                               | Maximize window         |
| H                                 | Center on hero          |


### Touch
Not implemented yet

## Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match
- Quad tree ()

## Design guide
### Struct/Class general
- Struct: POD 
- Only private member variables: name_

### Functions
- namespace: domain/related functions (interclass, non-class)
- methods: member if private member needed (class internal behaviour), free function (NMNF) if no private member needed (can work with existing interface)
- Task: Find most concise interface (as small as possible, as big as needed)!!!
    - When creating a member function: can it make use of a more "atomic" member function and therefore be made NMNF?
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

## Functor for existing function
```cpp
void func(int arg)
{
  std::cout << "Do something with arg " << arg << "\n";
}

struct fFunctor
{
  void operator()(int arg) {func(arg);}

  fFunctor() = default;
  fFunctor(int arg, bool ctorTrigger = false)
  {
    if (ctorTrigger)
    {
      std::cout << "Trigger in ctor: ";
      func(arg);
    }
  }
};
```

## Handles for SoA (https://youtu.be/QbffGSgsCcQ?si=5xdlcdhjIdP-QKVQ&t=1080)
```cpp
struct SoA
{
    static inline std::vector<int> members;
};

struct SoAHandle
{
    size_t index;
    int& id() { return SoA::members[index]; }
};

namespace SoAUtility
{
    SoAHandle create(int member)
    {
        SoA::members.push_back(member);
        return SoAHandle{SoA::members.size() - 1};
    };
}
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
- [x] Print tile info in panel
- [x] Refactor classes to emphasize NMNF (nonmember, nonfriend) functions
- [x] Change 'emscripten_set_main_loop()' to 'emscripten_set_main_loop_arg()'
- [x] Check function design (split long ones)
- [ ] Check movement functions in regards to game phases (obsolete after movement system change)
- [x] Modified parameters -> pointer
