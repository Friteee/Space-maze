#include <SDL.h>

#include <stdio.h>
#include "camera.h"
#include "window.h"

namespace video
{

// memory allocation for an instance of video_subsytem
Camera *Camera::instance = nullptr;


/**
 * Blit function for textures
 */
bool Camera::blit(SDL_Texture* texture , SDL_Rect* source , SDL_Rect* dest)
{
    SDL_Rect* blit_location = nullptr;
    // boolean to store information, about whether to blit the surface
    bool on_screen = true;
    if (dest)
    {
        if(dest->x + dest->w < Camera::instance->location.x || dest->x > Camera::instance->location.x + Camera::instance->location.w)
        {
            on_screen = false;
        }
        else if(dest->y + dest->h < Camera::instance->location.y || dest->y > Camera::instance->location.y + Camera::instance->location.h)
        {
            on_screen = false;
        }
    }
    if(!on_screen)
    {
        // no need to blit the surface
        return true;
    }
    if(dest)
    {
        blit_location=new SDL_Rect;
        blit_location->x=dest->x - Camera::instance->location.x;
        blit_location->y=dest->y - Camera::instance->location.y;
        blit_location->h=dest->h;
        blit_location->w=dest->w;
    }

    bool check = Video_subsystem::blit(texture,source,blit_location);
    if(!check)
    {
        delete blit_location;
        return false;
    }
    delete blit_location;
    return true;
}



/**
 * Blit function for textures with angle and flipping
 */
bool Camera::blit(SDL_Texture* texture , SDL_Rect* source , SDL_Rect* dest, const double angle, const SDL_Point * center , const SDL_RendererFlip flip)
{
    SDL_Rect* blit_location = nullptr;
    // boolean to store information, about whether to blit the surface
    bool on_screen = true;
    if (dest)
    {
        if(dest->x + dest->w < Camera::instance->location.x || dest->x > Camera::instance->location.x + Camera::instance->location.w)
        {
            on_screen = false;
        }
        else if(dest->y + dest->h < Camera::instance->location.y || dest->y > Camera::instance->location.y + Camera::instance->location.h)
        {
            on_screen = false;
        }
    }
    if(!on_screen)
    {
        // no need to blit the surface
        return true;
    }
    if(dest)
    {
        blit_location=new SDL_Rect;
        blit_location->x=dest->x - Camera::instance->location.x;
        blit_location->y=dest->y - Camera::instance->location.y;
        blit_location->h=dest->h;
        blit_location->w=dest->w;
    }

    bool check = Video_subsystem::blit(texture , source , blit_location , angle , center , flip);
    if(!check)
    {
        delete blit_location;
        return false;
    }
    delete blit_location;
    return true;
}

/**
 * camera getter
 */
Camera& Camera::get_instance()
{
    return *Camera::instance;
}

/**
 * Function to set position for camera
 */
void Camera::set_position(int init_width , int init_height , int init_x , int init_y)
{
    Camera::instance->location.x=init_x;
    Camera::instance->location.y=init_y;
    Camera::instance->location.w=init_width;
    Camera::instance->location.h=init_height;
}
/**
 * Function to set position for camera
 */
void Camera::set_position(SDL_Rect init_location)
{
    Camera::instance->location=init_location;
}


/**
 * Function to initialize subsystem with integers
 */
bool Camera::initialize(int init_width , int init_height , int init_x , int init_y)
{
    if(Camera::instance != nullptr)
    {
        return true;
    }
    Camera::instance = new Camera(init_width , init_height , init_x , init_y);
    return Camera::instance != nullptr;
}

/**
 * Function to initialize subsystem with rectangle
 */
bool Camera::initialize(SDL_Rect init_location)
{
    if(Camera::instance != nullptr)
    {
        return true;
    }
    Camera::instance = new Camera(init_location.w , init_location.h , init_location.x , init_location.y);
    return Camera::instance != nullptr;
}

/**
 * private constructor
 */
Camera::Camera(int init_width , int init_height , int init_x , int init_y)
{
    location.w=init_width;
    location.h=init_height;
    location.x=init_x;
    location.y=init_y;
}

/**
 * Camera movement function
 * As you may guess, moves the camera several pixels away
 */
void Camera::move(int x , int y)
{
    Camera::instance->location.x+=x;
    Camera::instance->location.y+=y;
}

SDL_Rect Camera::get_position()
{
    return instance->location;
}

}// end of video namespace
