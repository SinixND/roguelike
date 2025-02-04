//* Only define once
#define RAYGUI_IMPLEMENTATION
//* Required for custom icons set
#define RAYGUI_CUSTOM_ICONS

#include "../assets/iconset/iconset.rgi.h"
#include <raygui.h>

// #define CATCH
#define GTEST

#if defined( GTEST )
#include "gtests.h"
#endif

#define CATCH_CONFIG_MAIN

#if defined( CATCH )
#include "Catch2.h"
#include "catch_tests.h"
#endif
