#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED

#include <SDL.h>
#include "object.h"
#include "../video/camera.h"
#include <string.h>
#include <memory>
#include <deque>
#include "tiletype.h"

namespace objects
{

class Tile : public Object
{
    public:

    void show() override;
    Tile(SDL_Rect init_location , std::shared_ptr< SDL_Texture* > init_texture);
    Tile(SDL_Rect init_location , Tile_type * init_tile_type);
    SDL_Rect get_location()const override;
    Tile_type * get_tile_type();
    Tile(const Tile &copied_object) ;
    Tile& operator=(const Tile &copied_object) ;
    Tile(Tile && moved_object);
    Tile& operator=(Tile&& moved_object);
    ~Tile();

    private:
    std::shared_ptr<SDL_Texture*> texture;
    Tile_type * tile_type;


};



}// end of object namespace

#endif // WALL_H_INCLUDED
