
#include "gaming_mode.h"
#include "game_logic.h"

namespace game
{

/**
 * Gaming mode's input handler
 */

bool Gaming_mode::handle_input()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_q:
                break;
            case SDLK_a:
                break;
            case SDLK_w:
                break;
            case SDLK_s:
                break;
            case SDLK_d:
                break;
            case SDLK_RALT:
                break;
            case SDLK_LALT:
                break;
            case SDLK_TAB:
                break;
            case SDLK_ESCAPE:
                return false;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            {
                SDL_Rect click_location;
                click_location = video::Camera::get_position();
                printf("Click location =  %i , %i\n",click_location.x+event.button.x,click_location.y+event.button.y);
                break;
            }
        case SDL_WINDOWEVENT:
            switch(event.window.event)
            {
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_MOVED:
                break;
            }

        }

    }
    const Uint8 * key_states =SDL_GetKeyboardState(nullptr);

    // if alt + tab was pressed
    if((key_states[SDL_SCANCODE_LALT]||key_states[SDL_SCANCODE_RALT])&&key_states[SDL_SCANCODE_TAB])
    {
        // minimize the window
        video::Video_subsystem::minimize();
    }
    return true;
}

/**
 * Gaming mode's game loop
 */

bool Gaming_mode::run()
{
    unsigned int fps_cap = atoi(main_config->find_string("fps_cap").c_str());
    bool fps_cap_enabled = main_config->find_string("fps_cap_enabled").c_str()==string("true");
    // timers to enable sleeping and count of fps
    utility::Timer sleep_timer;
    utility::Timer fps_timer;
    fps_timer.start();
    sleep_timer.start();
    // integer for fps, overflow is unlikely :)
    unsigned int fps=0;
    while(!change_mode)
    {
        // handle that input
        if(handle_input()==false)
        {
            game_map.save_map();
            return false;
        }
        // reload video subsystem
        video::Video_subsystem::reload();
        // main logic here
        game_map.update();
        main_background->show();
        game_map.show();
        video::Video_subsystem::update_screen();
        // check the fps and sleep the necessary time
        fps++;
        if(fps_timer.get_ticks()>1000)
        {
            printf("Current fps = %i \n",fps);
            fps_timer.reload();
            fps=0;
        }
        if(sleep_timer.get_ticks()<1000/fps_cap&&fps_cap_enabled)
        {
            SDL_Delay(1000/fps_cap-sleep_timer.get_ticks());
        }
        sleep_timer.reload();

    }
    game_map.save_map();
    return true;
}

/**
 * Constructor
 */

Gaming_mode::Gaming_mode(utility::Configuration * init_config):
    main_config(init_config),
    game_map(main_config->find_string("map_config"))
{
    change_mode = false;
    main_background = new objects::Background(main_config->find_string("main_background").c_str());
}

}
