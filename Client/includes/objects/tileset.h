#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include"tiletype.h"
#include<vector>
#include "../video/texture.h"

namespace objects
{

class Tile_set
{
    public:

    SDL_Surface* crop(SDL_Rect* src );
    bool         initialize(std::string init_filename, uint8_t init_id);
    uint8_t      get_id();
    std::string  get_filename();

    private:
    video::Texture                texture;
    uint8_t                       id;
    std::string                   filename;
};

}//end of object namespace
#endif // TILESET_H_INCLUDED
