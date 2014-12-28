#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "SDL.h"
#include <vector>
#include <string>
#include "../gui/text.h"
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/prop.h"
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
    void save_map();
    //delete a wall
    void delete_wall(SDL_Point point);
    //add a coin
    void add_coin(SDL_Point point);

    private:

    // pointer to main player
    objects::Player * main_player;
    // main background for a map
    objects::Background main_background;
    // dequeue of walls
    std::vector<objects::Tile> tiles;
    // pointers to walls surface and texture
    shared_ptr<SDL_Texture*> tiles_texture;
    shared_ptr<SDL_Surface*> tiles_surface;
    // pointers to coin surface and texture
    shared_ptr<SDL_Texture*> coin_texture;
    shared_ptr<SDL_Surface*> coin_surface;
    // map configuration
    utility::Configuration map_config;
    // maps gravity
    int map_gravity;
    // coins
    std::vector<objects::Prop> coins;
    // text for the start
    gui::Text user_start;
    // text for the end
    gui::Text user_won;
    // text for counting those coins
    gui::Text coin_counter;
    // text for counting those coins
    gui::Text time;
    // timer, showing time since the start
    utility::Timer start_timer;

};

}//end of game namespace

#endif // MAP_H_INCLUDED
