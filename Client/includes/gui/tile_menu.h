#ifndef TILE_MENU_H_INCLUDED
#define TILE_MENU_H_INCLUDED

#include <vector>
#include "../objects/tiletype.h"
#include "../utility/configuration.h"
#include "gui_element.h"

namespace gui
{

class Tile_menu : Gui_element
{
    public:

    Tile_menu(std::vector<objects::Tile_type>& init_types , utility::Configuration * config);
    void show()override;
    void update()override;
    void hide()override;
    void unhide()override;
    objects::Tile_type * get_tile();


    private:

    std::vector<objects::Tile_type>& types;
    video::Texture arrow_texture;
    //video::Texture highlight_texture;
    SDL_Rect       left_arrow;
    SDL_Rect       right_arrow;
    unsigned int   current_type,
                   current_menu_location;
    bool           has_clicked;

};

}

#endif // TILE_MENU_H_INCLUDED
