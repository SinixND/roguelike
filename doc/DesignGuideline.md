# Design guideline
The Zen of Python (selection):
> - Explicit is better than implicit.
> - Simple is better than complex.
> - Complex is better than complicated.
> - Flat is better than nested.
> - Sparse (code) is better than dense.
> - Practicality beats purity.
> - Now is better than never.

## Links
[Noel's blog](https://gamesfromwithin.com/category/c)

## Functions parameters 
- Input last (can have default value, needs to be rightmost in that case)
    `(out >) io > in` 
- According to the above (see below example why to avoid ptr parameters for non-sideeffect like parameters)
    `(&/* >) const& / v `

### Example: Fake-pure, but copyless/performant and clear intention:
```cpp
[[nodiscard]] S const& f(S& s)
{
    ++s.m;

    return s;
}

// Use IO-suffix for non-returned, but modified (side effect) parameters
void f(S& sIO)
{
    ++sIO.m;
}
```

## Struct/Class general
- `class` for "single/few entities", `struct` for "many components"
- `struct`: POD (Plain Old Data) -> no functions, members descending size
    - `struct` can have a non-default ctor, but no private members or a base class
    - NOTE: According to deprectated `is_trivial` and `is_standard_layout` struct can have member funcitons
- Name `private` member variables: name_
- OOP/AoS if there is 1; DOD/SoA if there are more
- Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()], if no need for private access / public API (interface) is sufficient

## Class members (variables) and methods (member functions)
- Use `struct` if members are POD/trivial; 
- Use `class` if members are non-POD 
**NOTE**: Encapsulation (:= How much has to change if implementation changes) -> Low level parameters
- `public/struct` = default (KISS)
- `private/class` + non-trivial getter/setter= to handle invariants or restrict/ have controlled access
**NOTE**: refactor `public` -> getter/setter later if actually needed.*

## Example
```cpp
// Module.h
class C 
{ // has_invariant ? class : struct
private:
    // invariant/internal members
public:
    // independent members
public:
    // interface/functions for invariants
// private: 
    // internal helper functions belong to .cpp 
};

namespace CModule
{
    // functions for C only modification
}

// System.h
namespace SSystem
{
    // functions for multi-class modification
    // -> use multiple modules
}

```

# Concepts
## SoA
- := Struct of Arrays (of (entity-)members)
<!-- - If access other than by index needed: Add an additional mapping-member Id(entifier)->Index 
 - Decide if duplication for mapping, or require O(n) lookup of values -->
- Add utility methods to modify containers. Also eg. for accessing individual members and/or returning a single entity struct
- Transient (= Intermediate/Temporary/TransMoveation) data `struct`:
    - Forward declared `unique_ptr` member in `SoA.h`
    - Define it (and its member structs) in `SoA.cpp`

## SparseSet
- Maps  `id` => `idx` and `idx` => `data` (array to array) (potentially BIG!)

## DenseMap 
- Maps _arbitrary_ `id` => `idx` and `idx` => `data` (hashmap to array)

## Functor for existing function
```cpp
void func(int arg)
{
  std::cout << "Do something with arg " << arg << "\n";
}

struct fFunctor
{
    int arg;
    void operator()(int arg) {
        func(arg);
    }

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
# Git conventional commits
```markdown
// Subject line
type(scope): desc
// type:   change/feature/codestyle/structure/...
// scope:  area of app that is changed
// desc:   short description of solution **ONLY**

// Body
Explain what **and why**...

// Optional footer

```
