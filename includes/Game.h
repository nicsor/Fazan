#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <thread>

#include "interfaces/Game_interface.h"
#include "interfaces/Status_interface.h"

namespace game
{
  class Game : public game::interfaces::Game_interface
  {

  protected:
    bool active;
    std::thread m_thread;
    std::vector<Player> players;
    game::interfaces::Status_interface_ptr m_out_interface_ptr;

  public:
    Game (game::interfaces::Status_interface_ptr output);
    ~Game ();

  public:
    std::vector<Player> get_players () const;
    virtual bool start ();
    virtual bool stop ();
    virtual bool is_active () const;
    virtual bool add_player (int idJucator, const std::string &nume);
    virtual bool remove_player (int idJucator);
    virtual bool add_new_word (int idJucator, const std::string &cuvant);
    virtual bool challenge (const std::string &cuvant);

  protected:
    void background_task();
  };
}
#endif
