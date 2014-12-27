#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <stdint.h>

namespace objects
{

/**
 * Grid class
 * Use it if you want to draw precise grid
 */

class Grid
{

    public:

    Grid (uint8_t init_r , uint8_t init_g , uint8_t init_b  , int init_width);
    void draw ();

    private:

    uint8_t r,
            g,
            b;
    int     width;


};

}// end of object namespace

#endif // GRID_H_INCLUDED
