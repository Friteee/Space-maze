#ifndef TILETYPE_H_INCLUDED
#define TILETYPE_H_INCLUDED

#include "../video/texture.h"
#include <string>
#include <SDL.h>
#include <memory>


namespace objects
{


struct Tile_type
{
    video::Texture texture;
    bool           is_touchable;
    unsigned int   type_id;
    uint8_t        tile_set_id ,
                   x ,
                   y ,
                   w ,
                   h ;

};


}// end of namespace object


#endif // TILETYPE_H_INCLUDED
