#ifndef EDITOR_MODE_H_INCLUDED
#define EDITOR_MODE_H_INCLUDED

#include "program_mode.h"
#include "tilemap.h"
#include "../objects/grid.h"
#include "../gui/text_button.h"
#include "../gui/tile_menu.h"

namespace game
{

/**
 * Modes of editor modes
 */
enum Modes_of_editor
{
    tile_picker, map_editing
};

/**
 * Editor mode for main game logic
 */

class Editor_mode : Program_mode
{
    public :

    Editor_mode(utility::Configuration * init_config);
    bool       run ()override;
    bool       handle_input()override;

    private:

    // tile map
    Tile_map                 tile_map;
    // main configuration
    utility::Configuration * main_config;
    // event
    SDL_Event                event;
    // grid for easier tile placement
    objects::Grid            grid;
    // timers to handle fps and grid movement
    utility::Timer           fps_timer,
                             movement_timer;
    // button
    gui::Text_button         test_button;
    //
    gui::Tile_menu           tile_menu;
    // speed of the grid
    int                      grid_speed;
    // buffers for speed movement
    double                   grid_x,
                             grid_y;
    // mode of editor mode :o
    // "We know that you like modes, so we've put a mode in your mode"
    Modes_of_editor          mode;
};

} // end of game namespace

#endif // EDITOR_MODE_H_INCLUDED
