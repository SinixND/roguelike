#include "Catch2.h"

#include "MovementComponent.h"
#include "TransformComponent.h"

TEST_CASE( "Test case" )
{
    int var{ 0 };

    SECTION( "Section" )
    {
        REQUIRE( var == 0 );
    }
}

