#ifndef GAME_STATUS_INTERFACE_H
#define GAME_STATUS_INTERFACE_H

#include "interfaces/Status_interface.h"

namespace game
{

  class Status : public game::interfaces::Status_interface
  {
  public:
    Status ();
    virtual ~Status ();

    void start_advertising ();
    void stop_advertising ();
    void state_update (const game::interfaces::Game_interface &game);
    void request_turn (const Player &player);
    void request_vote_on_challenge (const std::string &word);
  };

}

#endif
