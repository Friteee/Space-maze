#ifndef GUI_ELEMENT_H_INCLUDED
#define GUI_ELEMENT_H_INCLUDED

#include <SDL.h>
#include "../video/texture.h"

namespace gui
{

/**
 * Gui element base class
 * Almost the same thing as object, but no collision with this one
 * And objects are used only with camera, while gui elements are drawn straight to screen
 */

class Gui_element
{
    public:

    virtual void show() = 0;
    virtual void update() = 0;
    inline void hide() {visible=false;}
    inline void unhide() {visible=true;}
    inline bool get_visibility(){ return visible;}

    protected :

    SDL_Rect location;
    video::Texture texture;
    bool visible;

};

}//end of object namespace

#endif // GUI_ELEMENT_H_INCLUDED
