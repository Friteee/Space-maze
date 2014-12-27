

#include "text.h"
#include "../utility/functions.h"
#include "../video/window.h"

namespace gui
{

Text::Text(utility::Configuration * config, std::string init_text, int x , int y)
{
    visible = true;
    text = init_text;

    std::vector<int> colors = utility::get_numbers_from_string(config->find_string("button_color"));
    color_key.r = colors[0];
    color_key.g = colors[1];
    color_key.b = colors[2];
    color_key.a = 255;

    font = TTF_OpenFont(config->find_string("font").c_str(),32);

    texture.init(init_text,color_key,font);

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
    text = init_text;
    texture.init(init_text,color_key,font);
}

}// end of namespace gui
