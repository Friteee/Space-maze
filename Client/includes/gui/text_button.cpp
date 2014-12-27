
#include "text_button.h"
#include <functional>
#include "../video/window.h"
#include "../utility/configuration.h"
#include "../utility/functions.h"
#include <vector>
#include <cstdio>

namespace gui
{

/**
 * Text button constructor
 */

Text_button::Text_button(utility::Configuration * config , std::string init_text , int x , int y)
{
    visible = true;
    SDL_Color buffer_color;

    std::vector<int> colors = utility::get_numbers_from_string(config->find_string("button_color"));
    buffer_color.r = colors[0];
    buffer_color.g = colors[1];
    buffer_color.b = colors[2];
    buffer_color.a = 255;

    TTF_Font * font = TTF_OpenFont(config->find_string("font").c_str(),32);

    text.init(init_text,buffer_color,font);

    location.x = x;
    location.y = y;
    location.w = text.get_width();
    location.h = text.get_height();
}

/**
 * Function to initialize the function for a button to call
 */

void Text_button::init_function(std::function<void()> init_on_click )
{
    on_click=init_on_click;
}

/**
 * Show function. Nothing to see here
 */

void Text_button::show()
{
    if(!visible)
    {
        return;
    }
    video::Video_subsystem::blit(text.get_texture(),nullptr,&location);
}

/**
 * Update function
 * Used for handling mouse input and checking whether the button was pressed
 */

void Text_button::update()
{
    if(!visible)
    {
        return;
    }
    int x,y;
    if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)&&!is_clicked)
    {
        if(x>location.x&&x<location.x+location.w&&y>location.y&&y<location.y+location.h)
        {
            is_clicked = true;
        }
    }
    else if(is_clicked)
    {
        if(x>location.x&&x<location.x+location.w&&y>location.y&&y<location.y+location.h&&!(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            if(on_click)
                on_click();
            else
                printf("Warning : no function specified for a button\n");
            is_clicked=false;
        }
        else if (!(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)))
        {
            is_clicked=false;
        }
    }
}


}
