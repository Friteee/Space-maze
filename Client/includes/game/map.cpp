
#include "map.h"
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>
#include "../utility/functions.h"
#include "../objects/collision.cpp"
#include <cstdlib>
#include <sstream>

namespace game {

Map::Map(std::string config_filename):
    map_config(config_filename),
    user_start(&map_config,"Find the exit after collecting all the coins!",180,300,SDL_Color{255,255,255}),
    user_won(&map_config,"You have won!",100,300,SDL_Color{255,255,255}),
    coin_counter(&map_config,"Coins left = 0",50,20,SDL_Color{240,240,0}),
    time(&map_config,"Timer = 0",850,20,SDL_Color{255,0,0})
{
    // enable tile texture
    std::string tile_texture_location = map_config.find_string("wall_texture");
    tiles_surface=std::make_shared<SDL_Surface*>(SDL_LoadBMP(tile_texture_location.c_str()));
    tiles_texture=std::make_shared<SDL_Texture*>(SDL_CreateTextureFromSurface(video::Video_subsystem::get_instance().get_renderer(),*tiles_surface.get()));
    std::vector<std::string> enable_tiles =  map_config.find_strings("wall");
    // create tiles
    unsigned int position_one=0;
    for(auto &x : enable_tiles)
    {
        vector<int> position = utility::get_numbers_from_string(x);
        SDL_Rect location;
        location.x=position[0];
        location.y=position[1];
        location.h=(*tiles_surface)->h;
        location.w=(*tiles_surface)->w;
        bool is_repeating=false;
        for(unsigned int a = position_one + 1; a<enable_tiles.size();a++)
        {
            if(x==enable_tiles[a])
            {
                is_repeating = true;
            }
        }
        position_one++;
        if(is_repeating)
        {
            continue;
        }
        tiles.push_back(objects::Tile(location,tiles_texture));
    }
    // create coins
    std::string coin_texture_location = map_config.find_string("coin_texture");
    coin_surface=std::make_shared<SDL_Surface*>(SDL_LoadBMP(coin_texture_location.c_str()));
    Uint32 key = SDL_MapRGB((*coin_surface)->format,255,0,255);
    SDL_SetColorKey( *coin_surface , SDL_TRUE , key );
    coin_texture=std::make_shared<SDL_Texture*>(SDL_CreateTextureFromSurface(video::Video_subsystem::get_instance().get_renderer(),*coin_surface.get()));
    std::vector<std::string> enable_coins =  map_config.find_strings("coin");

    for(auto &x : enable_coins)
    {
        vector<int> position = utility::get_numbers_from_string(x);
        SDL_Rect location;
        location.x=position[0];
        location.y=position[1];
        location.h=(*coin_surface)->h;
        location.w=(*coin_surface)->w;
        coins.push_back(objects::Prop(location,coin_texture));
    }

    map_gravity = atoi(map_config.find_string("gravity").c_str());
    // create the player
    main_player=new objects::Player(map_config.find_string("players_config"));

    start_timer.start();

    user_won.hide();

    std::ostringstream oss;
    oss<<"Coins left = "<<coins.size();
    coin_counter.change_text(oss.str());

    printf("Initialized map with %i tiles\n",tiles.size());
}

void Map::update()
{
    main_player->update();
    this->handle_collisions();
    if(start_timer.get_ticks()>5000)
    {
        user_start.hide();
    }
    std::ostringstream oss;
    oss<<"Timer = "<<start_timer.get_ticks()/1000<<"."<<start_timer.get_ticks()/100%10;
    time.change_text(oss.str());
}

void Map::show()
{
    main_player->center_screen();
    //main_background.show();
    for (auto & a : tiles)
    {
        a.show();
    }
    for( auto & a : coins)
    {
        a.show();
    }
    main_player->show();
    user_start.show();
    coin_counter.show();
    time.show();
    user_won.show();


}

void Map::handle_collisions()
{
    for( auto & a : tiles)
    {
        if(objects::is_overlapping(a.get_location() , main_player->get_location()))
        {
            main_player->notify_collision(&a);
        }
    }
    for(int a = 0 , b = coins.size() ; a<b ; a++)
    {
        if(objects::is_overlapping(coins[a].get_location() , main_player->get_location()))
        {

            coins.erase(coins.begin() + a);
            std::ostringstream oss;
            oss<<"Coins left = "<<coins.size();
            coin_counter.change_text(oss.str());
        }
    }
    SDL_Rect rectangle;
    rectangle.x=1340;
    rectangle.y=-1100;
    rectangle.h=100;
    rectangle.w=100;
    if(objects::is_overlapping(rectangle , main_player->get_location())&&coins.size()==0&&(!user_won.get_visibility()))
    {
        std::ostringstream oss;
        oss<<"Congratulations! Your score = "<<start_timer.get_ticks()/1000;
        user_won.change_text(oss.str());
        user_won.unhide();
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
            return;
        }
    }
}

void Map::save_map()
{
    std::ofstream file( "config/walls.txt" , std::ios_base::out ) ;
    for(auto &a : tiles)
    {
        SDL_Rect location = a.get_location();
        file<<"wall = "<<location.x<<","<<location.y<<std::endl;
    }
    for(auto &a : coins)
    {
        SDL_Rect location = a.get_location();
        file<<"coin = "<<location.x<<","<<location.y<<std::endl;
    }
    file.close();
}

void Map::add_coin(SDL_Point point)
{

    if(point.x<0)
        point.x-=10;
    if(point.y<0)
        point.y-=10;

    SDL_Rect location;
    location.x=point.x;
    location.y=point.y;
    location.h=(*coin_surface)->h;
    location.w=(*coin_surface)->w;
    coins.push_back(objects::Prop(location,coin_texture));
}

}// end of game namespace
