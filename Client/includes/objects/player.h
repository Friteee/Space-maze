#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include <string>
#include <memory>
#include "entity.h"
#include "../utility/timer.h"
#include "../utility/configuration.h"
#include "../video/texture.h"

namespace objects
{

enum Player_state
{
    jumping_right, jumping_left, standing, moving_right, moving_left
};

struct Player_information
{
    int       max_speed,
              move_speed,
              acceleration,
              jump_speed,
              gravity;
    SDL_Color color_key;
    SDL_Rect  location;
    double    friction;
    std::vector<std::string> jumping_textures;
    std::vector<std::string> moving_textures;
};

class Player : public Entity
{
    public:

    SDL_Rect get_location()const override;
    void     update()override;
    void     show()override;
    void     center_screen();
    void     notify_collision(Object * collided_object)override;
    Player   (std::string filename);
    Player   (Player_information & init_info);
    Player   (const Player& copied_object);
    Player&  operator=(const Player &copied_object) ;
    Player   (Player && moved_object);
    ~Player  ();

    private:

    void handle_input();
    void apply_state();
    void apply_speed();

    video::Texture              standing, jumping;
    std::vector<video::Texture> moving;

    Speed          speed;
    SDL_Rect       location ;
    Player_state   state;
    int            max_speed ,
                   move_speed ,
                   acceleration ,
                   gravity ,
                   jump_speed ;
    unsigned int   frame;
    utility::Timer movement_timer ,
                   last_jump ,
                   frame_change;
    double         position_x ,
                   position_y ,
                   friction;
};

}// end of objects namespace

#endif // PLAYER_H_INCLUDED
