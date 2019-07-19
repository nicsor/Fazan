#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <thread>

#include "interfaces/Game_interface.h"
#include "interfaces/Status_interface.h"

namespace game
{
  class Game : public game::interfaces::Game_interface
  {
    // Internal type definitions
  protected:
    enum Game_state
    {
      GAME_STATE_CHALLENGE,
      GAME_STATE_WAITING_PLAYERS,
      GAME_STATE_WAITING_INPUT,
      GAME_STATE_WAITING_VALIDATION,
      GAME_STATE_STOPPED
    };

  protected:
    uint8_t m_max_timeout_sec;
    Game_state m_game_state;
    std::thread m_thread;
    std::map<Player_Id, Player_ptr> m_players;
    game::interfaces::Status_interface_ptr m_out_interface_ptr;
    std::string m_last_word;
    std::string m_word_start_match;
    Player_ptr m_current_player;
    Player_Id m_current_player_id;

  protected:
    uint8_t get_active_players_count();
    void switch_next_player(const std::string &wordStart);
    bool is_final_word(const std::string &word);
    bool is_valid_word(const std::string &word);

  public:
    Game (game::interfaces::Status_interface_ptr output, uint8_t maxTimeoutSec);
    ~Game ();

  public:
    virtual std::map<Player_Id, Player_ptr> get_players () const;
    virtual bool start ();
    virtual bool stop ();
    virtual bool is_active () const;
    virtual bool add_player (Player_Id idPlayer, const std::string &nume);
    virtual bool remove_player (Player_Id idPlayer);
    virtual bool submit_new_word (Player_Id idPlayer, const std::string &cuvant);
    virtual bool challenge (const std::string &cuvant);

    void reinitialize();

  protected:
    void background_task();
  };
}
#endif
