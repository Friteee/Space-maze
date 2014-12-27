

#include "editor_mode.h"
#include "../objects/grid.h"
#include "../utility/timer.h"
#include "../gui/click.h"
#include <cstdio>
#include <cmath>

namespace game
{

/**
 * Running mode
 * Returns false if user wants to quit
 */

bool Editor_mode::run()
{
    fps_timer.start();
    movement_timer.start();
    int fps = 0;
    while(1)
    {
        fps++;
        if(handle_input()==false)
        {
            return false;
        }
        video::Video_subsystem::reload();

        test_button.update();

        if(mode == map_editing)
        {
            //update
            tile_menu.update();
            //show
            tile_map.get_background()->show();
            grid.draw();
            tile_menu.show();
        }

        test_button.show();


        video::Video_subsystem::update_screen();


        if(fps_timer.get_ticks()>1000)
        {
            printf("Current ticks = %i\n",fps);
            fps=0;
            fps_timer.reload();
        }
    }
}

/**
 * Input handling.
 * Used mostly to move the grid
 * Returns false if user wants to quit
 */

bool Editor_mode::handle_input()
{
    gui::Click click;
    click.set_clicked(false);
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            return false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            click.set_clicked(false);
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                click.set_location(event.button.x,event.button.y);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            click.set_clicked(true);
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                click.set_location(event.button.x,event.button.y);
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_r:
                tile_map.save();
                break;
            case SDLK_ESCAPE:
                break;
            case SDLK_q:
                return false;
            default:
                break;
            }
        default:
            break;
        }
    }
    const Uint8 * key_strokes = SDL_GetKeyboardState(nullptr);
    if((key_strokes[SDL_SCANCODE_RALT]||key_strokes[SDL_SCANCODE_LALT])&&key_strokes[SDL_SCANCODE_TAB])
    {
        video::Video_subsystem::minimize();
    }

    if(key_strokes[SDL_SCANCODE_W])
    {
        grid_y = grid_y - double(grid_speed) * movement_timer.get_ticks() / 1000;
        if(grid_y < -1)
        {
            video::Camera::move(0 , ceil(grid_y));
            grid_y -= ceil(grid_y);
        }
    }
    if(key_strokes[SDL_SCANCODE_A])
    {
        grid_x -= (double)grid_speed * movement_timer.get_ticks() / 1000;
        if(grid_x < -1)
        {
            video::Camera::move( ceil(grid_x) , 0);
            grid_x -= ceil(grid_x);
        }
    }
    if(key_strokes[SDL_SCANCODE_S])
    {
        grid_y += double(grid_speed) * movement_timer.get_ticks() / 1000;
        if(grid_y > 1)
        {
            video::Camera::move(0 , floor(grid_y));
            grid_y -= floor(grid_y);
        }
    }
    if(key_strokes[SDL_SCANCODE_D])
    {
        grid_x += double(grid_speed) * movement_timer.get_ticks() / 1000;
        if(grid_x > 1)
        {
            video::Camera::move( floor(grid_x) , 0);
            grid_x -= floor(grid_x);
        }
    }
    movement_timer.reload();
    return true;
}

/**
 * Editor constructor
 */

Editor_mode::Editor_mode(utility::Configuration * init_config) :
    main_config(init_config),
    grid(60,60,60, atoi(main_config->find_string("grid").c_str())),
    test_button(main_config,"Add tile types",5,5),
    tile_menu(tile_map.get_tile_types() , init_config),
    grid_speed(atoi(main_config->find_string("grid_speed").c_str())),
    grid_x(0),
    grid_y(0),
    mode(map_editing)
{
    auto change_function = [this]()
    {
        this->mode = this->mode==map_editing?tile_picker:map_editing;
    };
    test_button.init_function(change_function);
    tile_map.create_map("./maps/test.stm" , "Test map" , 1 , 1 , main_config);
}

}//end of game namespace

