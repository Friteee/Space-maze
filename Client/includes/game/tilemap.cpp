

#include <cstdio>
#include <fstream>
#include "../video/window.h"
#include "tilemap.h"
#include "../objects/tile.h"

namespace game
{

/**
 * Map initialization function
 * Use it only if you have a legitimate map
 */

bool Tile_map::initialize_map(std::string init_filename, uint8_t major_version_check, uint8_t minor_version_check)
{
    // initialize file
    map_info.filename=init_filename;
    std::ifstream file(map_info.filename , std::ios_base::in | std::ios_base::binary );
    // initialize buffer
    uint8_t * buffer = new uint8_t[8];
    if(file.eof())
    {
        printf("Error, file in %s is empty \n",map_info.filename.c_str());
    }
    // check if begging of file is legitimate
    // beginning should be 0xA9B21804E164F59D
    for(int a = 0; a < 8 ; a++)
    {
        file>>buffer[a];
    }
    if(buffer[0]!=0xA9||buffer[1]!=0xB2||buffer[2]!=0x18||buffer[3]!=0x04||buffer[4]!=0xE1||buffer[5]!=0x64||buffer[6]!=0xF5||buffer[7]!=0x9D)
    {
        printf("Error, file in %s is not legitimate\n",map_info.filename.c_str());
    }
    delete[] buffer;
    buffer = new uint8_t [2];
    file>>buffer[0]>>buffer[1];
    // check major version
    map_info.major_version = buffer[0];
    if(map_info.major_version < major_version_check)
    {
        printf("This map is highly outdated. Use at your own risk.\n");
        printf("Map's major version : %u\n", map_info.major_version);
        printf("Map's parser major version : %u\n" , major_version_check);
    }
    else if(map_info.major_version > major_version_check)
    {
        printf("This map is newer than your client! Either update your client or use map at your own risk.\n");
        printf("Map's major version : %u\n", map_info.major_version);
        printf("Map's parser major version : %u\n" , major_version_check);
    }
    // check minor version
    map_info.minor_version = buffer[1];
    if(map_info.minor_version < minor_version_check)
    {
        printf("This map is slightly outdated. Updating this map is preferable.\n");
        printf("Map's minor version : %u\n", map_info.minor_version);
        printf("Map's parser minor version : %u\n" , minor_version_check);
    }
    else if(map_info.minor_version > minor_version_check)
    {
        printf("This map is newer than your client! Updating your client is preferable.\n");
        printf("Map's minor version : %u\n", map_info.minor_version);
        printf("Map's parser minor version : %u\n" , minor_version_check);
    }
    // delete the versions
    delete [] buffer;
    // input map name
    getline(file,map_info.map_name);
    file>>map_info.width>>map_info.height;
    Chunk_type type_of_chunk;
    uint8_t chunk_buffer;
    while(!file.eof())
    {
        file>>chunk_buffer;
        type_of_chunk = Chunk_type(chunk_buffer);
        switch(type_of_chunk)
        {

        case Chunk_type::player:
            load_player   (file);
            break;
        case Chunk_type::tile_set:
            load_tile_set (file);
            break;
        case Chunk_type::tile_type:
            load_tile_type(file);
            break;
        case Chunk_type::tile :
            load_tile     (file);
            break;
        case Chunk_type::background:
            load_background(file);
            break;
        default:
            printf("Warning : unknown chunk was found. Map won't load properly\n");
            break;
        }
    }

    tiles.shrink_to_fit();

    // TODO - Implement loading of tile sets / tile types / tiles
    return true;
}

/**
 * Map creation function
 */

void Tile_map::create_map(std::string init_filename, std::string init_map_name, uint8_t init_major_version , uint8_t init_minor_version , utility::Configuration * config)
{
    map_info.filename = init_filename;
    map_info.map_name = init_map_name;
    map_info.major_version = init_major_version;
    map_info.minor_version = init_minor_version;
    map_info.width  = atoi(config->find_string("default_map_width").c_str());
    map_info.height = atoi(config->find_string("default_map_height").c_str());

    objects::Tile_set buffer;
    buffer.initialize(config->find_string("default_tile_set"),0);
    sets_of_tiles.push_back(std::move(buffer));

    background_filename = config->find_string("main_background");
    background = new objects::Background (background_filename.c_str());

    return ;
}

/**
 * Function to save the map
 */

bool Tile_map::save()
{
    std::ofstream file(map_info.filename , std::ios_base::out | std::ios_base::binary );
    uint8_t * buffer = new uint8_t (8);
    buffer[0] = 0xA9;
    buffer[1] = 0xB2;
    buffer[2] = 0x18;
    buffer[3] = 0x04;
    buffer[4] = 0xE1;
    buffer[5] = 0x64;
    buffer[6] = 0xF5;
    buffer[7] = 0x9D;
    // initialize the map by checksum
    for(int a=0 ; a < 8 ; a++)
    {
        file<<buffer[a];
    }
    delete [] buffer;
    // save major and minor map version
    file<<map_info.major_version;
    file<<map_info.minor_version;
    // save map name
    file<<map_info.map_name<<'\n';
    file<<map_info.width<<' ';
    file<<map_info.height<<' ';
    if(player)
    {
        file<<Chunk_type::player<<' ';
        save_player(file);
    }
    for(auto & a : sets_of_tiles)
    {
        file<<Chunk_type::tile_set<<' ';
        save_tile_set(file,a);
    }
    for(auto & a : types_of_tile)
    {
        file<<Chunk_type::tile_type<<' ';
        save_tile_type(file,a);
    }
    for(auto & a : tiles)
    {
        file<<Chunk_type::tile<<' ';
        save_tile(file,a);
    }
    if(background)
    {
        file<<Chunk_type::background<<' ';
        save_background(file);
    }

    return true;
}

/**
 * Load functions for
 * a. Player
 * b. Tile sets
 * c. Tile types
 * d. Tiles
 */

void Tile_map::load_player(std::ifstream & file)
{
    std::string filename;
    getline(file,filename);
    player = new objects::Player (filename) ;
}

void Tile_map::load_tile_set(std::ifstream & file)
{
    objects::Tile_set buffer;
    std::string filename;
    getline(file,filename);
    uint8_t id;
    file>>id;
    buffer.initialize(filename,id);
    sets_of_tiles.push_back(std::move(buffer));
}

void Tile_map::load_tile_type(std::ifstream & file)
{
    objects::Tile_type tile_buffer;
    std::string filename;
    file>>tile_buffer.type_id>>tile_buffer.is_touchable>>tile_buffer.tile_set_id;
    file>>tile_buffer.x>>tile_buffer.y>>tile_buffer.w>>tile_buffer.h;
    SDL_Surface * surface_buffer;
    // check if type of tile at the same location as id has the same id as id (oh my...)
    if(sets_of_tiles.size()>tile_buffer.tile_set_id&&sets_of_tiles[tile_buffer.tile_set_id].get_id()==tile_buffer.tile_set_id)
    {
        SDL_Rect location;
        location.x = tile_buffer.x;
        location.y = tile_buffer.y;
        location.w = tile_buffer.w;
        location.h = tile_buffer.h;
        surface_buffer = sets_of_tiles[tile_buffer.tile_set_id].crop(&location);
    }
    else
    for(unsigned int a = 0 ; a < sets_of_tiles.size() ; a++)
    {
        if(sets_of_tiles[a].get_id() == tile_buffer.tile_set_id)
        {
            SDL_Rect location;
            location.x = tile_buffer.x;
            location.y = tile_buffer.y;
            location.w = tile_buffer.w;
            location.h = tile_buffer.h;
            surface_buffer = sets_of_tiles[a].crop(&location);
        }
    }

    tile_buffer.texture.init(surface_buffer);

    for(unsigned int a = 0 ; a < types_of_tile.size() ; a++)
    {
        if(tile_buffer.type_id == types_of_tile[a].type_id)
        {
            printf("Error : Same tile type id declared twice");
        }
    }
    types_of_tile.push_back(std::move(tile_buffer));


}

void Tile_map::load_tile(std::ifstream & file)
{
    SDL_Rect location;
    unsigned int id;
    file>>id;
    file>>location.x>>location.y;
    // check if type of tile at the same location as id has the same id as id (oh my...)
    if(types_of_tile.size()>id&&types_of_tile[id].type_id == id)
    {
       tiles.push_back(objects::Tile(location, &types_of_tile[id]));
    }
    else
    for(unsigned int a = 0 ; a < types_of_tile.size() ; a++)
    {
        if(types_of_tile[a].type_id == id)
        {
            tiles.push_back(objects::Tile(location, &types_of_tile[id]));
        }
    }

}

void Tile_map::load_background(std::ifstream & file)
{
    getline(file,background_filename);
    background = new objects::Background(background_filename.c_str());
}

/**
 * Save functions for :
 * a. Player
 * b. Tile sets
 * c. Tile types
 * d. Tiles
 */

void Tile_map::save_player(std::ofstream & file)
{
    file<<player_filename<<'\n';
}

void Tile_map::save_tile_set(std::ofstream & file , objects::Tile_set & tile_set)
{
    file<<tile_set.get_id()<<tile_set.get_filename()<<'\n';
}

void Tile_map::save_tile_type(std::ofstream & file, objects::Tile_type & tile_type)
{
    file<<tile_type.type_id<<tile_type.is_touchable<<tile_type.x<<tile_type.y<<tile_type.w<<tile_type.h;
}

void Tile_map::save_tile(std::ofstream & file, objects::Tile& tile)
{
    file<<tile.get_location().x<<tile.get_location().y<<tile.get_tile_type()->type_id;
}

void Tile_map::save_background(std::ofstream & file)
{
    file<<background_filename<<'\n';
}

/**
 * Getters
 */

objects::Background * Tile_map::get_background()
{
    return background;
}

objects::Player * Tile_map::get_player()
{
    return player;
}

std::vector<objects::Tile>& Tile_map::get_tiles()
{
    return tiles;
}

std::vector<objects::Tile_type>& Tile_map::get_tile_types()
{
    return types_of_tile;
}

}// end of namespace game
