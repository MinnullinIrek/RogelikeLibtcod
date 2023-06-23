#include "keyboard.h"

// #include <sddl.h>
// #include <libtcod.hpp>
// #include "SDL_keyboard.h"
// #include "../SDL/include/SDL_keyboard.h"
#include "header.h"
#include "units/actor.h"
#include <SDL_keycode.h>

Keyboard::Keyboard(std::shared_ptr<Actor> actor)
    : m_actor(actor){

}

Keyboard::~Keyboard() {}

void Keyboard::setActor(std::shared_ptr<Actor> actor) { m_actor = actor; }

void Keyboard::setKey(int key) {
  EAction action = EAction::none;
   switch (key) {
     case SDLK_RIGHT:
       action = EAction::right;
       break;
     case SDLK_LEFT:
       action = EAction::left;
       break;
     case SDLK_UP:
       action = EAction::up;
       break;
     case SDLK_DOWN:
       action = EAction::down;
       break;

     default:
       break;
   }
   if (action != EAction::none) {
     m_actor->doKey(action);
   }

}
