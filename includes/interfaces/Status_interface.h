#ifndef GAME_INTERFACES_STATUS_INTERFACE_H
#define GAME_INTERFACES_STATUS_INTERFACE_H

#include <memory>

#include "interfaces/Game_interface.h"
#include "Player.h"

namespace game
{
  namespace interfaces
  {

    class Game_interface;

    class Status_interface
    {
    public:
      virtual ~Status_interface ()
      {
      }

      virtual void start_advertising () = 0;
      virtual void stop_advertising () = 0;
      virtual void state_update (const Game_interface &joc) = 0;
      virtual void request_turn (const Player &jucator) = 0;
      virtual void request_vote_on_challenge (const std::string &word) = 0;
    };

    typedef std::shared_ptr<Status_interface> Status_interface_ptr;

  }
}
#endif
