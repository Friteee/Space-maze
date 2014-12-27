
#include <vector>
#include "click.h"
#include "../objects/tiletype.h"
#include "../objects/collision.cpp"
#include "../utility/configuration.h"
#include "../video/camera.h"
#include "../video/window.h"
#include "tile_menu.h"
#include <cstdio>

namespace gui
{




Tile_menu::Tile_menu(std::vector<objects::Tile_type>& init_types , utility::Configuration * config):
    types(init_types)
{
    texture.init(config->find_string("tile_menu_image"),SDL_Color{255,0,255});
    arrow_texture.init(config->find_string("arrow"),SDL_Color{255,0,255});

    location     = video::Camera::get_position();
    location.y   = location.h-texture.get_height();
    location.x   = location.w/2-texture.get_width()/2;
    location.w   = texture.get_width();
    location.h   = texture.get_height();
    left_arrow   = location;
    left_arrow.x-= arrow_texture.get_width();
    left_arrow.h = arrow_texture.get_height();
    left_arrow.w = arrow_texture.get_width();
    right_arrow  = left_arrow;
    right_arrow.x= location.x + location.w;

    visible=true;
    current_menu_location = 0;

}

void Tile_menu::show()
{
    if(!visible)
    {
        return;
    }
    video::Video_subsystem::blit(texture.get_texture() , nullptr , &location);
    video::Video_subsystem::blit(arrow_texture.get_texture(),nullptr,&right_arrow);
    video::Video_subsystem::blit(arrow_texture.get_texture(),nullptr,&left_arrow,0,nullptr,SDL_FLIP_HORIZONTAL);

}

void Tile_menu::update()
{
    if(!visible)
    {
        return ;
    }
    Click left_click;

    if(left_click.has_clicked(right_arrow))
    {
        if(current_menu_location<types.size()-10)
            current_menu_location++;
        return;
    }

    else if(left_click.has_clicked(left_arrow))
    {
        if(current_menu_location>0)
            current_menu_location--;
        return;
    }

    int breakdown = (location.w-32*10)/11;
    SDL_Rect current_square;
    current_square.y = location.y + breakdown;
    current_square.w = 32;
    current_square.h = 32;
    for(int a = 0,  b = types.size() ; a < 10 && a < b ; a++)
    {
        current_square.x = location.x + breakdown * (a+1) + 32*a;
        if(left_click.has_clicked(current_square))
        {
            current_type=a+current_menu_location;
        }
    }
}

objects::Tile_type * Tile_menu::get_tile()
{
    return &types[current_type];
}

void Tile_menu::hide()
{
    visible=false;
}

void Tile_menu::unhide()
{
    visible=true;
}

}
