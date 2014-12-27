#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "SDL.h"
#include <deque>
#include <string>
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/background.h"
#include "../objects/tile.h"
#include "../utility/configuration.h"

namespace game
{

class Map
{
    public:

    // maps initialization function
    Map(std::string config_filename);
    // maps update function
    void update();
    // maps function to show it's objects
    void show();
    // function to handle collisions
    void handle_collisions();
    //function to add a wall
    void add_wall(SDL_Point point);
    // save a wall
    void save_walls();
    //delete a wall
    void delete_wall(SDL_Point point);

    private:

    // pointer to main player
    objects::Player * main_player;
    // main background for a map
    objects::Background main_background;
    // dequeue of walls
    std::deque<objects::Tile> tiles;
    // pointers to walls surface and texture
    shared_ptr<SDL_Texture*> tiles_texture;
    shared_ptr<SDL_Surface*> tiles_surface;
    // map configuration
    utility::Configuration map_config;
    // maps gravity
    int map_gravity;

};

}//end of game namespace

#endif // MAP_H_INCLUDED
