#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "gui_element.h"
#include <functional>
#include "../utility/configuration.h"

namespace gui
{

class Text_button : Gui_element
{
    public:

    Text_button(utility::Configuration * config, std::string init_text , int x , int y);
    void show()override;
    void update()override;
    void init_function(std::function<void()> init_on_click);

    private:

    // texture holding surface and texture for the text
    video::Texture        text;
    // function, which is being called every time user clicks a button
    std::function<void()> on_click;
    // boolean to know if the button is being clicked
    bool                  is_clicked;


};

}// end of object namespace

#endif // BUTTON_H_INCLUDED
