# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[5] Render sprites/textures
    - check managers for entities, or entities for components, or on component change notify systems

# Shortterm
[0] make ECS
[4] split globals and settings

# Longterm 

# Continuous
[0] plan structure with UML

# Notes
1. init

1.1 init system

1.1 register component(s)

1.2 register system(s)

1.3.1 register component type(s) to system signature

2. create entity
3. add component to entity
4. execute system


## Variadic function template:
### Distinct types:
template <typename Type, typename... VariadicTypes>
void doSomething(Type firstArg, VariadicTypes... variadicArgs) // recursive variadic function
{
    // Handle first argument
    cout << firstArg << endl;
 
    // Handle remaining (variadic) arguments
    doSomething(variadicArgs...);
}

### Similar types:
template <class Type>
void func2( std::initializer_list<Type> arguments )
{
    for( auto argument : arguments )
    {
        std::cout << argument << std::endl ;
    }
}

# Done
[x] compile for linux & windows !
