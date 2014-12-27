

#include "entity.h"

namespace objects{

/**
 * Check if two objects (an object and a entity) are overlapping each other
 */

inline bool is_overlapping(const Object* one , const Entity* two)
{
    SDL_Rect first_rect=one->get_location();
    SDL_Rect second_rect=two->get_location();
    if( !((first_rect.x+first_rect.w) > second_rect.x && first_rect.x < (second_rect.x+second_rect.w)))
        return false;

    if( !((first_rect.y+first_rect.h) > second_rect.y && first_rect.y < second_rect.y+second_rect.h))
        return false;

    return true;
}

/**
 * Detect if a point is overlapping a rectangle
 */

inline bool is_overlapping(const SDL_Point* one , const SDL_Rect* two)
{

    if( !(one->x > two->x && one->x < two->x+two->w))
        return false;

    if( !(one->y > two->y && one->y < two->y+two->h))
        return false;

    return true;
}

}
