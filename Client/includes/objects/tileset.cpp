
#include "tileset.h"
#include <cstdio>
#include <memory>

namespace objects
{

/**
 * Crop the tile set and return the cropped surface
 */

SDL_Surface* Tile_set::crop(SDL_Rect * src)
{
    // rgba masks for surface using example from official sdl wiki
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
    #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
    #endif

    // create a surface and copy contents of tile_set to it
    SDL_Surface * buffer = SDL_CreateRGBSurface(0,src->w,src->h,32,rmask,gmask,bmask,amask);
    SDL_BlitSurface(texture.get_surface(), src, buffer, nullptr);

    return buffer;
}

/**
 * Initialize tile set with unique id and filename
 */

bool Tile_set::initialize(std::string init_filename, uint8_t init_id)
{
    texture.init(init_filename);
    id=init_id;
    filename=init_filename;
    return true;
}

/**
 * Tile set name and id getters
 */
std::string Tile_set::get_filename()
{
    return filename;
}
uint8_t Tile_set::get_id()
{
    return id;
}

}
