# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[1] check contiguous memory in ContiguousContainer and the retrieved elements

# Shortterm
[1] map generation

[2] implement viewport

# Longterm 

# Continuous

# Notes
Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match


## Variadic function template:
### Distinct types:
{
    template <typename Type, typename... VariadicTypes>
    void doSomething(Type firstArg, VariadicTypes... variadicArgs) // Recursive variadic function
    {
        // Handle first argument
        cout << firstArg << endl;
    
        // Handle remaining (variadic) arguments
        doSomething(variadicArgs...);
    }
}

### Similar types:
{
    template <class Type>
    void func2( std::initializer_list<Type> arguments )
    {
        for( auto argument : arguments )
        {
            std::cout << argument << std::endl ;
        }
    }
}

# Done
[x] compile for linux & windows !

[x] Render sprites/textures
[x] make ECS
