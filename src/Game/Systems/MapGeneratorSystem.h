#ifndef IG20240128151409
#define IG20240128151409

#include "Map.h"

namespace MapGeneratorSystem
{
    Map createTestRoom();
    Map createStartRoom();

    //* Map types
    Map createGridRooms( int level );

    Map createRandomMapType( int level );
}

#endif
