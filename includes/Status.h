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

    void error_detected (const std::string &error);
    void start_advertising ();
    void stop_advertising ();
    void state_update (const game::interfaces::Game_interface &game);
    void request_turn (Player_ptr player, const std::string &wordStart);
    void request_vote_on_challenge (const std::string &word);
  };

}

#endif
