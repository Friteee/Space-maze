#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "object.h"


namespace objects
{


/**
 * Entity is movable / moving object which NEEDS to be updated every frame
 */
class Entity : public Object
{
    public:

    // virtual update function
    virtual void update() = 0;
    // virtual notify of collisions function
    virtual void notify_collision(Object * collided_object) = 0;


    protected:

    SDL_Rect old_location;

};

}//end of namespace


#endif // ENTITY_H_INCLUDED
