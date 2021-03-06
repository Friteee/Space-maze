#ifndef GAMING_MODE_H_INCLUDED
#define GAMING_MODE_H_INCLUDED

#include "../objects/grid.h"
#include "program_mode.h"
#include "../objects/background.h"
#include "map.h"


namespace game
{

class Gaming_mode : Program_mode
{
    public :

    Gaming_mode(utility::Configuration * init_config);
    bool run ()override;
    bool handle_input()override;

    private:

    // background
    objects::Background * main_background;
    // event
    SDL_Event event;
    // configuration
    utility::Configuration * main_config;
    // does user wants to change programs mode?
    bool change_mode;
    // old map
    Map game_map;
};

}

#endif // GAMING_MODE_H_INCLUDED
