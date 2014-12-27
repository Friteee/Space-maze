

#include "grid.h"
#include "../video/window.h"
#include "../video/camera.h"

namespace objects
{

/**
 * Grid draw function
 */
void Grid::draw()
{
    SDL_Rect camera;
    camera = video::Camera::get_position();
    //draw vertical grid
    for(int x = 0; x < camera.w; x++)
    {
        if( (x + camera.x) % width != 0)
            continue;
        video::Video_subsystem::draw_line(x, 0 , x , camera.h , r, g, b );
    }
    //draw horizontal grid
    for(int y = 0; y <  camera.h; y++)
    {
        if( (y+camera.y) % width != 0)
            continue;
        video::Video_subsystem::draw_line(0 ,  y , camera.w , y  , r, g, b );
    }
}

/**
 * Grid constructor
 */

Grid::Grid(uint8_t init_r , uint8_t init_g , uint8_t init_b  , int init_width):
    r(init_r),
    g(init_g),
    b(init_b),
    width(init_width)
{

}

} // end of namespace objects
