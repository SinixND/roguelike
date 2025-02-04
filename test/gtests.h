#ifndef IG20250204120911
#define IG20250204120911

#include <gtest/gtest.h>

#include "MovementComponent.h"
#include "MovementSystem.h"
#include "TransformComponent.h"

TEST(
    Sec_Suite_Constructed_transform_is_all_0,
    Test_Preparing_TransformationComponent
)
{
    TransformComponent transform{};
    MovementComponent movement{ { { 1, 0 }, { 2, 0 } } };

    EXPECT_EQ( transform.direction.x, 0 );
    EXPECT_EQ( transform.direction.y, 0 );
    EXPECT_EQ( transform.remainingDistance, 0 );
    EXPECT_EQ( transform.speed, 0 );
}

TEST(
    Sec_Suite_Preparing_TransformationComponent_sets_all_values,
    Test_Preparing_TransformationComponent
)
{
    TransformComponent transform{};
    MovementComponent movement{ { { 1, 0 }, { 2, 0 } } };

    transform = MovementSystem::prepareByDirection( transform, movement, { 1, 0 } );

    EXPECT_EQ( transform.direction.x, true );
    EXPECT_EQ( transform.direction.y, true );
    EXPECT_EQ( transform.remainingDistance, true );
    EXPECT_EQ( transform.speed, true );
}

#endif
