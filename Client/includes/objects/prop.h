#ifndef PROP_H_INCLUDED
#define PROP_H_INCLUDED

#include "object.h"
#include <memory>


namespace objects
{

class Prop : public Object
{
    public:

    void show()override;
    SDL_Rect get_location() const override;

    Prop(SDL_Rect init_location , std::shared_ptr< SDL_Texture* > init_texture);
    Prop(const Prop &copied_object) ;
    Prop& operator=(const Prop &copied_object) ;
    Prop(Prop && moved_object);
    Prop& operator=(Prop&& moved_object);
    ~Prop();

    private:

    std::shared_ptr<SDL_Texture*> texture;


};



}//end of objects namespace

#endif // PROP_H_INCLUDED
