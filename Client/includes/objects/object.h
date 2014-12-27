#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <sdl.h>

namespace objects
{

/**
 *  Enumeration to find object type
 */
enum Object_type
{
    wall=0 , player , prop
};

/**
 * Structure to hold x and y to be used for speed
 */
struct Speed
{
    double x;
    double y;
};



/**
 * Interface class for future objects
 */
class Object
{
    public:

    // virtual function to get objects location
    virtual SDL_Rect get_location() const = 0;
    // virtual function to show the object
    virtual void show() = 0 ;
    // getter of the object type
    Object_type get_type();

    protected:

    // type of the object
    Object_type type;
    // object's location
    SDL_Rect location;

};


}// end of namespace

#endif // OBJECT_H_INCLUDED
