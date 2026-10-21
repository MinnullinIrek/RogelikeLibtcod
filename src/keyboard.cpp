#include "keyboard.h"

// #include <sddl.h>
// #include <libtcod.hpp>
// #include "SDL_keyboard.h"
// #include "../SDL/include/SDL_keyboard.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>

#include "game_struct.h"
#include "header.h"
#include "units/actor.h"
#include "utils/visualEffect.h"

Keyboard::Keyboard(std::shared_ptr<Actor> actor) : m_actor(actor) {}

Keyboard::~Keyboard() {}

void Keyboard::setActor(std::shared_ptr<Actor> actor) { m_actor = actor; }

void Keyboard::setKey(int key, bool down) {
  // pushKeys(key, down);
  if (!down) {
    return;
  }
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
    case SDLK_I:
      action = EAction::inventory;
      break;
    case SDLK_M:
      action = EAction::map;
      break;
    case SDLK_Z:
      if (m_lastKeys == SDLK_LCTRL) {
        action = EAction::undo;
      }
      break;
    case SDLK_C:
      action = EAction::charAction;
      break;
    case SDLK_E:
      action = EAction::equip;
      break;
    case SDLK_X:
      action = EAction::unequip;
      break;
    case SDLK_B:
      // if (m_lastKeys == SDLK_LCTRL)
      {
        EffectMaker effect;

        effect.m_effect = {
            {CoordSymbol(Coord{6, 13}, Identifier('*'))},
            {CoordSymbol(Coord{5, 13}, Identifier('*')), CoordSymbol(Coord{7, 13}, Identifier('*'))},
            {CoordSymbol(Coord{4, 13}, Identifier('*')), CoordSymbol(Coord{8, 13}, Identifier('*'))},
            {CoordSymbol(Coord{3, 13}, Identifier('*')), CoordSymbol(Coord{9, 13}, Identifier('*'))},

        };
        gameStruct.vEffect->effectStart(effect);
      }
      break;

    default:
      break;
  }
  m_lastKeys = key;
  if (action != EAction::none) {
    m_actor->doKey(action);
  }
}

void Keyboard::pushKeys(int key, bool down) {
  if (down) {
    m_pressedKeys.emplace(key);
  } else {
    m_pressedKeys.erase(key);
  }
}
