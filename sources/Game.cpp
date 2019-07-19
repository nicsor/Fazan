#include <chrono>
#include <vector>

#include "Game.h"

namespace game
{
  const std::vector<std::string> WORD_TERMINATORS = { "nt" };

  Game::Game (game::interfaces::Status_interface_ptr output, uint8_t maxTimeoutSec) :
      m_max_timeout_sec (maxTimeoutSec),
      m_game_state (GAME_STATE_WAITING_PLAYERS),
      m_out_interface_ptr (output),
      m_current_player_id (0)
  {
    m_out_interface_ptr->start_advertising ();
  }

  Game::~Game ()
  {
  }

  std::map<Player_Id, Player_ptr> Game::get_players () const
  {
    return m_players;
  }

  void Game::reinitialize()
  {
    stop();

    // We will keep the current players and just wait for a new game to start
    m_game_state = GAME_STATE_WAITING_PLAYERS;
    m_out_interface_ptr->start_advertising ();
  }

  bool Game::start ()
  {
    bool status = false;

    // Shall start only if at least 2 players
    if (m_players.size () >= 2)
    {
      if ((m_game_state == GAME_STATE_STOPPED) or (m_game_state == GAME_STATE_WAITING_PLAYERS))
      {
        status = true;
        m_current_player = m_players.begin ()->second;
        m_current_player_id = m_current_player->get_id ();
        m_game_state = GAME_STATE_WAITING_INPUT;

        m_thread = std::thread (&Game::background_task, this);

        m_out_interface_ptr->stop_advertising ();
        m_out_interface_ptr->state_update (*this);
      }
    }

    return status;
  }

  bool Game::stop ()
  {
    if (m_game_state != GAME_STATE_STOPPED)
    {
      m_game_state = GAME_STATE_STOPPED;

      m_thread.join ();
    }

    return true;
  }

  bool Game::is_active () const
  {
    return (m_game_state != GAME_STATE_STOPPED);
  }

  bool Game::add_player (Player_Id idPlayer, const std::string &name)
  {
    bool status = false;

    if (m_game_state == GAME_STATE_WAITING_PLAYERS)
    {
      // Check if the id is already available
      if (m_players.find (idPlayer) == m_players.end ())
      {
        Player_ptr playerPtr = std::make_shared<Player> (Player (idPlayer, name));

        m_players.insert (std::make_pair (idPlayer, playerPtr));
        status = true;
      }
    }

    return status;
  }

  bool Game::remove_player (Player_Id idPlayer)
  {
    m_players.erase (idPlayer);

    return true;
  }

  bool Game::submit_new_word (Player_Id idPlayer, const std::string &word)
  {
    bool status = false;

    if (m_current_player.get () != nullptr)
    {
      if (idPlayer == m_current_player.get ()->get_id ())
      {
        m_game_state = GAME_STATE_WAITING_VALIDATION;
        m_last_word = word;
        status = true;
      }
    }

    return status;
  }

  bool Game::challenge (const std::string &cuvant)
  {
    // TODO
    return true;
  }

  void Game::background_task ()
  {
    uint8_t iteration = 0;

    while (is_active ())
    {
      switch (m_game_state)
      {
        case (GAME_STATE_CHALLENGE):
        {
          // not implemented. wait for input
          m_game_state = GAME_STATE_WAITING_INPUT;
          break;
        }
        case (GAME_STATE_WAITING_PLAYERS):
        {
          // Should not be under this scenario. exit
          m_game_state = GAME_STATE_STOPPED;
          break;
        }
        case (GAME_STATE_WAITING_INPUT):
        {
          if ((iteration / 10) >= m_max_timeout_sec)
          {
            m_current_player.get ()->set_match_lost ();
            m_out_interface_ptr->error_detected ("Timeout while waiting for input");
            switch_next_player ("");
            iteration = 0;
          }

          break;
        }
        case (GAME_STATE_WAITING_VALIDATION):
        {
          if (is_valid_word (m_last_word))
          {
            // Move onto the next player and reset the timeout
            switch_next_player (m_last_word);
            iteration = 0;
          }

          // Switch back to waiting input
          m_game_state = GAME_STATE_WAITING_INPUT;
          continue;
        }
        case (GAME_STATE_STOPPED):
        {
          break;
        }
        default:
          break;
      }

      std::this_thread::sleep_for (std::chrono::milliseconds (100));
      iteration++;
    }
  }

  void Game::switch_next_player (const std::string &wordStart)
  {
    if (m_players.size () > 0)
    {
      bool found = false;
      auto currentPlayer = m_players.find (m_current_player_id);

      if (currentPlayer == m_players.end ())
      {
        m_out_interface_ptr->error_detected ("Active player exited on his turn.");

        // Should take in account previous id. but c'est la vie
        m_current_player = m_players.begin ()->second;
        m_current_player_id = m_current_player->get_id ();
      }

      while (++currentPlayer != m_players.end ())
      {
        if (currentPlayer->second.get ()->get_score () > 0)
        {
          m_current_player_id = currentPlayer->second.get ()->get_id ();
          found = true;
          break;
        }
      }

      if (not found)
      {
        auto currentPlayer = m_players.begin ();

        while (currentPlayer != m_players.end ())
        {
          if (currentPlayer->second.get ()->get_score () > 0)
          {
            m_current_player_id = currentPlayer->second.get ()->get_id ();
            found = true;
            break;
          }
          ++currentPlayer;
        }

        if (not found)
        {
          // abnormal
          m_out_interface_ptr->error_detected ("No more active players");
          m_game_state = GAME_STATE_STOPPED;
          return;
        }
      }

      m_current_player = m_players.at (m_current_player_id);

      //m_current_player->
      m_word_start_match.clear ();

      if (is_final_word (wordStart))
      {
        m_current_player->set_match_lost ();
      }
      else if (wordStart.size () >= 2)
      {
        m_word_start_match = wordStart.substr (wordStart.length () - 2);
      }

      // Check if more than one player still available
      if (get_active_players_count () > 1)
      {
        m_out_interface_ptr->request_turn (m_current_player, m_word_start_match);
      }
      else
      {
        m_game_state = GAME_STATE_STOPPED;
      }
    }
    else
    {
      m_game_state = GAME_STATE_STOPPED;
    }

    m_out_interface_ptr->state_update (*this);
  }

  uint8_t Game::get_active_players_count ()
  {
    uint8_t playerCount = 0;

    for (auto it : m_players)
    {
      playerCount += (it.second->get_score () > 0);
    }

    return playerCount;
  }

  bool Game::is_final_word (const std::string &word)
  {
    bool status = false;

    if (word.size () >= 2)
    {
      std::string availableTerminator = word.substr (word.length () - 2);

      for (auto terminator : WORD_TERMINATORS)
      {
        if (terminator == availableTerminator)
          status = true;
      }
    }

    return status;
  }

  bool Game::is_valid_word (const std::string &word)
  {
    bool status = (word.size () >= 2);

    // Check if the word starts with the expected pattern
    status |= std::equal (m_word_start_match.begin (), m_word_start_match.end (), word.begin ());

    // Check each word contains only characters
    for (auto character : word)
    {
      if (not status)
      {
        break;
      }

      status |= (((character >= 'a') && (character <= 'z'))
          || ((character >= 'A') && (character <= 'Z')));
    }

    if (not status)
    {
      m_out_interface_ptr->error_detected ("Received word not valid");
    }

    return status;
  }
}
