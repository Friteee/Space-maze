
#include "player.h"
#include <string>
#include <memory>
#include "../video/camera.h"
#include "../video/window.h"
#include "../utility/functions.h"
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <math.h>

namespace objects
{

/**
 * Object's get location function
 */
SDL_Rect Player::get_location() const
{
    return location;
}

/**
 * Players update function
 * Used every tick to update his speed and location
 */
void Player::update()
{
    // apply location
    old_location = location;

    this->apply_speed();
    // reload the timer for the next tick
    movement_timer.reload();

    this->handle_input();

    this->apply_state();
}

/**
 * Players show function
 * Gets players state and draws player on screen accordingly
 */
void Player::show()
{
    SDL_Rect blit_rectangle = location;
    if(state==Player_state::standing)
    {
        blit_rectangle.w = standing.get_width();
        blit_rectangle.h = standing.get_height();
        video::Camera::blit(standing.get_texture(), nullptr , &blit_rectangle);
    }
    else if(state==Player_state::moving_right)
    {
        blit_rectangle.w = moving[frame].get_width();
        blit_rectangle.h = moving[frame].get_height();
        video::Camera::blit(moving[frame].get_texture(), nullptr , &blit_rectangle);
    }
    else if(state==Player_state::moving_left)
    {
        blit_rectangle.w = moving[frame].get_width();
        blit_rectangle.h = moving[frame].get_height();
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        video::Camera::blit(moving[frame].get_texture(), nullptr , &blit_rectangle , 0 , nullptr , flip);
    }
    else if (state==Player_state::jumping_right)
    {
        blit_rectangle.w = jumping.get_width();
        blit_rectangle.h = jumping.get_height();
        video::Camera::blit(jumping.get_texture(), nullptr , &blit_rectangle);
    }
    else if (state==Player_state::jumping_left)
    {
        blit_rectangle.w = jumping.get_width();
        blit_rectangle.h = jumping.get_height();
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        video::Camera::blit(jumping.get_texture(), nullptr , &blit_rectangle, 0 ,nullptr , flip);
    }

}

/**
 * Players constructor
 */
Player::Player (std::string filename)
{
    // init frame
    frame = 0;
    // init ocnfig
    utility::Configuration player_config(filename);
    // so, the type of this object is player
    type=player;
    // load gravity
    gravity=atoi(player_config.find_string("gravity").c_str());;
    // load surfaces and textures
    std::string players_texture_standing = player_config.find_string("players_standing_texture") ;
    std::string players_texture_jumping = player_config.find_string("players_jumping_texture") ;
    // enable main texture
    if(players_texture_standing=="")
    {
        players_texture_standing="./images/main_player.bmp";
    }
    std::vector <std::string> players_texture_moving = player_config.find_strings("players_moving_texture") ;
    moving.resize(players_texture_moving.size());

    // get color key
    std::vector<int> color_key_rgb = utility::get_numbers_from_string(player_config.find_string("color_key"));

    SDL_Color color_key;
    color_key.r = color_key_rgb[0];
    color_key.g = color_key_rgb[1];
    color_key.b = color_key_rgb[2];
    color_key.a = 255;
    // create textures
    standing.init(players_texture_standing,color_key);
    jumping.init(players_texture_jumping,color_key);
    int count = 0;
    for(auto& a : moving)
    {
        a.init(players_texture_moving[count],color_key);
        count++;
    }


    // initialize the player
    std::vector<int> players_location_int = utility::get_numbers_from_string(player_config.find_string("players_location")) ;
    // if location is bad, set rectangle to default
    if(players_location_int.size()!=2)
    {
        location.x=0;
        location.y=0;
        location.w = standing.get_width();
        location.h = standing.get_height();
    }
    else
    {
        location.x=players_location_int[0];
        location.y=players_location_int[1];
        location.w = standing.get_width();
        location.h = standing.get_height();
    }

    friction = double(atoi(player_config.find_string("friction").c_str()))/1000;

    // initialize speed
    speed.x = 0;
    speed.y = 0;
    //enable state
    state = Player_state::standing;
    // initialize players acceleration
    // acceleration variable is used to express CURRENT acceleration of the player
    acceleration = 0;
    // start the timers
    movement_timer.start();
    last_jump.start();
    frame_change.start();
    // initialize location and position
    old_location = location;
    position_y=location.y;
    position_x=location.x;
    // initialize speeds
    max_speed = atoi(player_config.find_string("players_max_speed").c_str());
    move_speed = atoi(player_config.find_string("players_move_speed").c_str());
    jump_speed = atoi(player_config.find_string("jump_speed").c_str());

}

/**
 * Copy constructor...
 */
Player::Player(const Player& copied_object)
{
    state = copied_object.state;
    max_speed = copied_object.max_speed;
    move_speed = copied_object.move_speed;
    gravity = copied_object.gravity;
    jump_speed = copied_object.jump_speed;
    frame=0;
    friction = copied_object.friction;
    standing = copied_object.standing;
    jumping = copied_object.jumping;
    moving = copied_object.moving;
    speed = copied_object.speed;
    acceleration = 0;
    movement_timer.start();
    location = copied_object.location;
    location.w = standing.get_width();
    location.h = standing.get_height();
    position_y=location.y;
    position_x=location.x;
}

/**
 * Copy assignment operator...
 */
Player& Player::operator=(const Player& copied_object)
{
    state = copied_object.state;
    max_speed = copied_object.max_speed;
    move_speed = copied_object.move_speed;
    gravity = copied_object.gravity;
    jump_speed = copied_object.jump_speed;
    frame=0;
    friction = copied_object.friction;
    standing = copied_object.standing;
    jumping = copied_object.jumping;
    moving = copied_object.moving;
    speed = copied_object.speed;
    acceleration = 0;
    movement_timer.start();
    location = copied_object.location;
    location.w = standing.get_width();
    location.h = standing.get_height();
    position_y=location.y;
    position_x=location.x;
    return *this;
}

/**
 * Move constructor...
 */
Player::Player(Player && moved_object)
{
    standing = moved_object.standing;
    jumping = moved_object.jumping;
    moving = moved_object.moving;
    speed = moved_object.speed;
    acceleration = 0;
    movement_timer.start();
    last_jump.start();
    location = moved_object.location;
    location.w = standing.get_width();
    location.h = standing.get_height();
    position_y = location.y;
    position_x = location.x;
}

/**
 * Currently, nothing is here
 */
Player::~Player()
{

}

/**
 * Notify the player that he has collided with something
 */

void Player::notify_collision(Object * collided_object)
{
    if(collided_object->get_type()==wall)
    {
        SDL_Rect collided = collided_object->get_location();
        if((old_location.x + old_location.w) <= collided.x && (location.x + location.w) > collided.x)// collided from the left
        {
            //revert location
            location = old_location;
            position_x = (double)location.x;
            if(speed.x>0)
                speed.x=0;
        }
        if(old_location.x>=collided.x+collided.w&&location.x<collided.x+collided.w)// collided from the right
        {
            //revert location
            location=old_location;
            position_x = (double)location.x;
            if(speed.x<0)
                speed.x=0;
        }
        if(old_location.y + old_location.h <= collided.y && location.y + location.h > collided.y)// collided from the top
        {
            //revert location
            location   = old_location;
            // I don't know why, but the -0.5 thing fixes the bug with too much collision detection
            // Tried the same thing with -1 and player becomes a little buggy
            if(location.y>0)
                position_y = location.y;
            else
                position_y = location.y - 0.5;
            if(state!=moving_left&&state!=moving_right)
            {
                state = Player_state::standing;
            }
            if(speed.y > 0)
                speed.y = 0;
        }
        if(old_location.y >= collided.y + collided.h && location.y < collided.y + collided.h)// collided from the bottom
        {
            //revert location
            position_y = (double) old_location.y;
            location.y = old_location.y;
            speed.y = 0;
        }



    }
}

/**
 * Handle players input
 */

void Player::handle_input()
{
    // handle the input
    const Uint8 * key_states = SDL_GetKeyboardState(nullptr);
    // ifs to handle input
    if((key_states[SDL_SCANCODE_W]||key_states[SDL_SCANCODE_UP])&&last_jump.get_ticks()>200)
    {
        if(state==Player_state::moving_left || state==Player_state::standing )
            state = jumping_left;
        else if (state==Player_state::moving_right)
            state = jumping_right;
        speed.y = - jump_speed;
        last_jump.reload();
    }
    if(key_states[SDL_SCANCODE_A]||key_states[SDL_SCANCODE_LEFT])
    {
        if(state == Player_state::standing || state == Player_state::moving_right)
            state = Player_state::moving_left;
        if(state == Player_state::jumping_right)
            state = Player_state::jumping_left;
        speed.x = - move_speed;
    }
    else if(key_states[SDL_SCANCODE_D]||key_states[SDL_SCANCODE_RIGHT])
    {
        if(state == Player_state::standing || state == Player_state::moving_left)
            state = Player_state::moving_right;
        if(state == Player_state::jumping_left)
            state = Player_state::jumping_right;
        speed.x = move_speed;
    }
    else
    {
        if((state == Player_state::moving_left || state == Player_state::moving_right)&& state != Player_state::jumping_left && state != Player_state::jumping_right)
            state = Player_state::standing;
        acceleration=0;
    }
}

/**
 * Apply the speed for player according to his input
 */

void Player::apply_speed()
{
    //if last update was half a second ago (which means that either player lags like hell or he suspended the screen)
    if(movement_timer.get_ticks()>350)
    {
        // reload the timer. We don't want the player clipping out of screen
        movement_timer.reload();
    }
    Speed old_speed=speed;
    // check states, change the movement accordingly
    speed.y += (double) movement_timer.get_ticks() / 1000 * gravity ;
    // apply friction to the movement
    // TODO : Make friction time - based
    if( speed.x != 0 )
    {
        if(speed.x > 0)
        {
            speed.x-= friction * movement_timer.get_ticks()  ;
            if(speed.x<0)speed.x=0;
        }
        else
        {
            speed.x+= friction * movement_timer.get_ticks()  ;
            if(speed.x>0)speed.x=0;
        }

    }
    // change the position (double one) and location (rectangle one)
    // use position_x and _y, because they make movement steady and smooth
    position_y +=  (speed.y + old_speed.y)/ 2 * movement_timer.get_ticks() / 1000 ;
    position_x +=  (speed.x + old_speed.x)/ 2 * movement_timer.get_ticks() / 1000 ;
    location.x = (int)position_x;
    location.y = (int)position_y;

    // check if speed is maxed
    if(speed.x > max_speed)
    {
        speed.x = max_speed;
    }
    if( speed.y > max_speed )
    {
        speed.y = max_speed;
    }
    else if( speed.y < - max_speed )
    {
        speed.y = - max_speed;
    }
}

/**
 * Center the camera on player
 */

void Player::center_screen()
{
    // make an rectangle to choose a center of screen as player and update
    SDL_Rect center_of_screen;
    center_of_screen = location;
    SDL_GetWindowSize(video::Video_subsystem::get_instance().get_window() , & center_of_screen.w , & center_of_screen.h);
    center_of_screen.x = center_of_screen.x - center_of_screen.w / 2 + location.w / 2;
    center_of_screen.y = center_of_screen.y - center_of_screen.h / 2 + location.h / 2;
    video::Camera::set_position(center_of_screen);
}


/**
 * Apply state according to his surroundings , position , speed and etc.
 */
void Player::apply_state()
{

    // if player is falling, change state
    if(speed.y>50)  // magic constant...
    {
        if(state==Player_state::standing||state==Player_state::moving_left||state==Player_state::jumping_left)
            state=Player_state::jumping_left;
        else
            state=Player_state::jumping_right;
    }
    if(state!=Player_state::moving_left&&state!=Player_state::moving_right)
    {
        frame = 0;
    }
    else if (frame_change.get_ticks()>200)
    {
        frame++;
        if(frame+1>moving.size())
            frame=0;
        frame_change.reload();
    }

}

}//end of objects namespace
