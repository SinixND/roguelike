#define CATCH_CONFIG_MAIN
#include "Catch2.h"

//* Only define once
#define RAYGUI_IMPLEMENTATION
//* Required for custom icons set
#define RAYGUI_CUSTOM_ICONS

#include "../assets/iconset/iconset.rgi.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "TransformComponent.h"
#include <raygui.h>

TEST_CASE( "Preparing TransformationComponent" )
{
    TransformComponent transform{};
    MovementComponent movement{ { { 1, 0 }, { 2, 0 } } };

    SECTION( "Constructed transform is all 0" )
    {
        REQUIRE( transform.direction.x == 0 );
        REQUIRE( transform.direction.y == 0 );
        REQUIRE( transform.remainingDistance == 0 );
        REQUIRE( transform.speed == 0 );
    }

    SECTION( "Preparing TransformationComponent sets all values" )
    {
        transform = MovementSystem::prepareByDirection( transform, movement, { 1, 0 } );

        REQUIRE( transform.direction.x );
        REQUIRE( transform.direction.y );
        REQUIRE( transform.remainingDistance );
        REQUIRE( transform.speed );
    }
}
