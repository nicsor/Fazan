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
    std::cout << " => Start_advertising" << std::endl;
  }

  void Status::stop_advertising ()
  {
    std::cout << " => Stop_advertising" << std::endl;
  }

  void Status::state_update (const game::interfaces::Game_interface &game1)
  {
    std::cout << " => Status: " << (game1.is_active() ? "started" : "stopped") << std::endl;

    for (auto item : game1.get_players())
    {
      auto player = item.second;

      std::cout << "\tPlayer " << player->get_name() << "\t\t| " << (int)player->get_score() << " |" << std::endl;
    }
  }

  void Status::request_turn (Player_ptr player, const std::string &wordStart)
  {
    if (player.get() != nullptr)
    {
      std::cout << " => Request turn: " << player->get_name() << " (" << (int)player->get_id() << ")" << std::endl;
    }
  }

  void Status::request_vote_on_challenge (const std::string &word)
  {
    std::cout << " => Challenge word: " << word << std::endl;
  }

  void Status::error_detected (const std::string &error)
  {
    std::cout << " => Error:  " << error << std::endl;
  }
}
