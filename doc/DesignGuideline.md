# Design guideline
## Prefer struct over class (**not used in favor of fail save invariant handling of classes**)
Noel's blog: https://gamesfromwithin.com/category/c
- Handle invariants via Module functions (replace member functions)
- Structur (example): 
    - namespace Class::Hero (struct/class)
    - namespace Component::Health (struct/class)
    - namespace Module::certHealth(health) -> health (eg. handle invariants)
    - namespace System::regen(health) -> health (eg. use certHealth())

## Struct/Class general
- Struct: POD (Plain Old Data) -> no functions
- Name private member variables: name_
- OOP/AoS if there is 1; DOD/SoA if there are more
- Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()], if no need for private access / public API (interface) is sufficient

## Class members (variables) and methods (member functions)
**NOTE**: Encapsulation (:= How much has to change if implementation changes)
- Default: `private` (eg. due to internal dependencies)
    - If read access needed (trivial and non-trivial): `const` getter
    - If non-trivial write access needed: setter
- If non-const access or trivial write access needed: `public`

#### **NOTE**: 
Although trivial getters + setter provide more flexibility for later change, as they do not tie to a specific implementation. they hide the (pontentially high) cost of calling the function. 

*Better refactor `public` to getter/setter later if actually needed.*


# Concepts
## SoA
- := Struct of Arrays (of (entity-)members)
<!-- - If access other than by index needed: Add an additional mapping-member Id(entifier)->Index 
 - Decide if duplication for mapping, or support O(n) lookup of values -->
- Add utility methods to modify containers. Also eg. for accessing individual members and/or returning a single entity struct
- Transient (= Intermediate/Temporary/TransMoveation) data `struct`:
    - Forward declared `unique_ptr` member in `SoA.h`
    - Define it (and its member structs) in `SoA.cpp`
- Unsorted/unsynced (public): Querying/prefetching/culling/preparation before (parallel) processing needed due to holes?
    - Use for optional components
- Sorted/Synced (private): Direct parallel processing possible
    - Use if matching components always exist

## SparseSet
- Maps _integer_ `id` to `idx` and `idx` to `data` (array to array)

## DenseMap 
- Maps _arbitrary_ `id` to `idx` and `idx` to `data` (hashmap to array)

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

