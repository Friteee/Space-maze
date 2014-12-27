
#include "prop.h"
#include "../video/camera.h"
#include <memory>


namespace objects
{

SDL_Rect Prop::get_location() const
{
    return location;
}

void Prop::show()
{
    video::Camera::blit( (*texture) , nullptr , &location);
}

Prop::Prop(SDL_Rect init_location , std::shared_ptr< SDL_Texture* > init_texture)
{
    type = prop;
    texture = init_texture;
    location = init_location;
}

Prop::Prop(const Prop &copied_object)
{
    type = prop;
    texture = copied_object.texture;
    location = copied_object.location;
}

Prop& Prop::operator=(const Prop &copied_object)
{
    if(this!=&copied_object)
    {
        this->type = prop;
        this->texture = copied_object.texture;
        this->location = copied_object.location;
    }
    return *this;
}

Prop::Prop(Prop && moved_object)
{
    type = prop;
    texture = moved_object.texture;
    location = moved_object.location;
    moved_object.texture.reset();
}

Prop& Prop::operator=(Prop&& moved_object)
{
    if(this!=&moved_object)
    {
        this->type = prop;
        this->texture = moved_object.texture;
        this->location = moved_object.location;
        moved_object.texture.reset();
    }
    return *this;
}

Prop::~Prop()
{
    if(texture.unique())
    {
        SDL_DestroyTexture(*texture);
    }
    texture.reset();
}


}//end of object namespace
