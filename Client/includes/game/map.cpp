
#include "map.h"
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>
#include "../utility/functions.h"
#include "../objects/collision.cpp"
#include <cstdlib>

namespace game {

Map::Map(std::string config_filename):
    map_config(config_filename)
{
    // enable tile texture
    std::string tile_texture_location = map_config.find_string("wall_texture");
    tiles_surface=std::make_shared<SDL_Surface*>(SDL_LoadBMP(tile_texture_location.c_str()));
    tiles_texture=std::make_shared<SDL_Texture*>(SDL_CreateTextureFromSurface(video::Video_subsystem::get_instance().get_renderer(),*tiles_surface.get()));
    std::vector<std::string> enable_tiles =  map_config.find_strings("wall");
    // create tiles
    for(auto &x : enable_tiles)
    {
        vector<int> position = utility::get_numbers_from_string(x);
        SDL_Rect location;
        location.x=position[0];
        location.y=position[1];
        location.h=(*tiles_surface)->h;
        location.w=(*tiles_surface)->w;
        tiles.push_back(objects::Tile(location,tiles_texture));
    }
    map_gravity = atoi(map_config.find_string("gravity").c_str());
    // create the player
    main_player=new objects::Player(map_config.find_string("players_config"));

    printf("Initialized map with %i tiles\n",tiles.size());
}

void Map::update()
{
    main_player->update();
    this->handle_collisions();
}

void Map::show()
{
    //main_background.show();
    for (auto & a : tiles)
    {
        a.show();
    }
    main_player->center_screen();
    main_player->show();

}

void Map::handle_collisions()
{
    for( auto & a : tiles)
    {
        if(is_overlapping(&a , main_player))
        {
            main_player->notify_collision(&a);
        }
    }
}

void Map::add_wall(SDL_Point point)
{
    SDL_Point old_point = point;
    if(point.x<0)
        point.x-=10;
    if(point.y<0)
        point.y-=10;
    point.x -= point.x % 10 ;
    point.y -= point.y % 10 ;

    for(auto & a : tiles)
    {
        SDL_Rect tile_location = a.get_location();
        if(objects::is_overlapping(&old_point , &tile_location))
        {
            return;
        }
    }
    SDL_Rect location;
    location.x=point.x;
    location.y=point.y;
    location.h=(*tiles_surface)->h;
    location.w=(*tiles_surface)->w;
    tiles.push_back(objects::Tile(location,tiles_texture));
}

void Map::delete_wall(SDL_Point point)
{
    for(int a = 0 , b = tiles.size() ; a<b ;a++)
    {
        SDL_Rect tile_location = tiles[a].get_location();
        if(objects::is_overlapping(&point , &tile_location))
        {
            auto vector_iterator = tiles.begin();
            tiles.erase(vector_iterator+a);
        }
    }
}

void Map::save_walls()
{
    std::ofstream file( "config/walls.txt" , std::ios_base::out ) ;
    for(auto &a : tiles)
    {
        SDL_Rect location = a.get_location();
        file<<"wall = "<<location.x<<","<<location.y<<std::endl;
    }
    file.close();
}

}// end of game namespace