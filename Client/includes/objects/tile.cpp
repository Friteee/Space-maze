
#include "tile.h"
#include <stdio.h>
#include "../video/window.h"

namespace objects
{

/**
 * Object's get location function
 */
SDL_Rect Tile::get_location() const
{
    return location;
}

Tile_type * Tile::get_tile_type()
{
    return tile_type;
}

/**
 * Object's show function
 */
void Tile::show()
{
    video::Camera::blit(*texture, nullptr , &location);
    return;
}


/**
 * Constructor
 */
Tile::Tile(SDL_Rect init_location , std::shared_ptr<SDL_Texture*> init_texture)
{
    type=wall;
    location=init_location;
    texture=init_texture;
}

/**
 * Constructor
 */
Tile::Tile(SDL_Rect init_location , Tile_type * init_tile_type)
{
    type=wall;
    tile_type = init_tile_type;
    location=init_location;
    //texture=tile_type->texture;
}


/**
 * Copy constructor
 */
Tile::Tile(const Tile &copied_object)
{
    type=copied_object.type;
    texture=copied_object.texture;
    location=copied_object.location;
}

/**
 * Copy assignment operator
 */
Tile& Tile::operator=(const Tile &copied_object)
{
    if(this!=&copied_object)
    {
        this->texture=copied_object.texture;
        this->location=copied_object.location;
        this->type=copied_object.type;
    }
    return *this;
}

/**
 * Move constructor
 */
Tile::Tile(Tile && moved_object)
{
    type=moved_object.type;
    texture=moved_object.texture;
    location=moved_object.location;
    moved_object.texture.reset();
}

/**
 * Move assignment operator
 */
Tile& Tile::operator=(Tile &&moved_object)
{
    if(this!=&moved_object)
    {
        this->texture=moved_object.texture;
        this->location=moved_object.location;
        this->type=moved_object.type;
        moved_object.texture.reset();
    }
    return *this;
}


Tile::~Tile()
{
    texture.reset();
}

}//end of object namespace
