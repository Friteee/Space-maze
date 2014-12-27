

#include "text.h"
#include "../utility/functions.h"
#include "../video/window.h"

namespace gui
{

Text::Text(utility::Configuration * config, std::string init_text, int x , int y)
{
    visible = true;
    SDL_Color buffer_color;

    std::vector<int> colors = utility::get_numbers_from_string(config->find_string("button_color"));
    buffer_color.r = colors[0];
    buffer_color.g = colors[1];
    buffer_color.b = colors[2];
    buffer_color.a = 255;

    TTF_Font * font = TTF_OpenFont(config->find_string("font").c_str(),32);

    texture.init(init_text,buffer_color,font);

    location.x = x;
    location.y = y;
    location.w = texture.get_width();
    location.h = texture.get_height();
}

void Text::show()
{
    if(!visible)
    {
        return;
    }
    video::Video_subsystem::blit(texture.get_texture(),nullptr,&location);
}

void Text::update()
{

}

void Text::change_text(std::string init_text)
{

}

}// end of namespace gui
