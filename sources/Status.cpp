#include <iostream>

#include "Status.h"

namespace game
{

  Status::Status ()
  {
  }

  Status::~Status ()
  {
  }

  void Status::start_advertising ()
  {
    std::cout << "start_advertising";
  }

  void Status::stop_advertising ()
  {
    std::cout << "stop_advertising";
  }

  void Status::state_update (const game::interfaces::Game_interface &game1)
  {
    std::cout << "Status: " << (game1.is_active() ? "started" : "stopped");

    for (auto player : game1.get_players())
    {
      std::cout << "Player " << player.get_name() << ": " << player.get_score() << std::endl;
    }
  }

  void Status::request_turn (const Player &player)
  {
    std::cout << "Request turn: " << player.get_name();
  }

  void Status::request_vote_on_challenge (const std::string &word)
  {
    std::cout << "Challenge word: " << word;
  }

}
