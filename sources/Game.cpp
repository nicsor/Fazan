#include <chrono>

#include "Game.h"

namespace game
{

  Game::Game (game::interfaces::Status_interface_ptr output) :
      active (false), m_out_interface_ptr (output)
  {
  }

  Game::~Game ()
  {

  }

  std::vector<Player> Game::get_players () const
  {
    return players;
  }

  bool Game::start ()
  {
    bool status = false;

    if (not active)
    {
      active = true;
      status = true;

      m_thread = std::thread(&Game::background_task, this);

      m_out_interface_ptr->start_advertising ();
    }

    return status;
  }

  bool Game::stop ()
  {
    if (active)
    {
      active = false;

      m_out_interface_ptr->stop_advertising ();

      m_thread.join();
    }

    return true;
  }

  bool Game::is_active () const
  {
    return active;
  }

  bool Game::add_player (int idPlayer, const std::string &name)
  {
    bool status = false;

    if (not is_active ())
    {
      players.push_back (Player (idPlayer, name));
      status = true;
    }

    return status;
  }

  bool Game::remove_player (int idJucator)
  {
    auto player = players.begin();

    while(player != players.end())
    {
      if (player->get_id() == idJucator)
      {
        player = players.erase(player);
      }
      else
      {
        ++player;
      }
    }

    return true;
  }

  bool Game::add_new_word (int idJucator, const std::string &cuvant)
  {
    // TODO
    return true;
  }

  bool Game::challenge (const std::string &cuvant)
  {
    // TODO
    return true;
  }

  void Game::background_task() {
      while (is_active()) {
         std::this_thread::sleep_for(std::chrono::seconds(1));
      }
  }
}
