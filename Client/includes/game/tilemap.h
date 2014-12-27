#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include "../objects/tiletype.h"
#include "../objects/tileset.h"
#include "../objects/player.h"
#include "../objects/tile.h"
#include "../objects/background.h"

namespace game
{

struct Map_information
{
    int width;
    int height;
    int gravity;
    uint8_t major_version;
    uint8_t minor_version;
    std::string map_name;
    std::string filename;
};

enum Chunk_type
{
    player=0,tile_set,tile_type,tile,background
};


/**
 * This tile map is more or less just a data container
 */

class Tile_map
{

    public :

    bool initialize_map(std::string init_filename, uint8_t major_version_check , uint8_t minor_version_check);
    void create_map(std::string init_filename, std::string init_map_name, uint8_t init_major_version , uint8_t init_minor_version , utility::Configuration * config);
    bool save();

    objects::Background *            get_background();
    objects::Player *                get_player();
    std::vector<objects::Tile>&      get_tiles();
    std::vector<objects::Tile_type>& get_tile_types();

    private :

    // private functions, mainly to process chunks of data

    void load_player    (std::ifstream & file);
    void load_tile_set  (std::ifstream & file);
    void load_tile_type (std::ifstream & file);
    void load_tile      (std::ifstream & file);
    void load_background(std::ifstream & file);
    void save_player    (std::ofstream & file);
    void save_tile_set  (std::ofstream & file, objects::Tile_set & set);
    void save_tile_type (std::ofstream & file, objects::Tile_type & type);
    void save_tile      (std::ofstream & file, objects::Tile & tile);
    void save_background(std::ofstream & file);

    // private variables

    std::vector<objects::Tile_type>    types_of_tile;
    std::string                        player_filename ,
                                       background_filename;
    std::vector<objects::Tile_set>     sets_of_tiles;
    std::vector<objects::Tile>         tiles;
    objects::Background *              background;
    objects::Player *                  player;
    Map_information                    map_info;

};

}// end of game namespace

#endif // TILEMAP_H_INCLUDED
