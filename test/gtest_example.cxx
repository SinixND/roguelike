#include "Catch2.h"

TEST_CASE( "Example" )
{
    //* Each section copies original int
    int a{ 0 };

    SECTION( "Check a = 0 [Pass]" )
    {
        REQUIRE( a == 0 );
    }

    SECTION( "Modify and Check a = 1 [Pass]" )
    {
        a = 1;
        REQUIRE( a == 1 );
    }

    SECTION( "Check a = 1 [Fail!]" )
    {
        REQUIRE( a == 1 );
    }
}
