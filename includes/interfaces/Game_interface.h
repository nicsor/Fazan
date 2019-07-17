#ifndef GAME_INTERFACES_GAME_INTERFACE_H
#define GAME_INTERFACES_GAME_INTERFACE_H

#include <string>
#include <vector>

#include "Player.h"

namespace game
{
  namespace interfaces
  {

    class Game_interface
    {
    public:
      virtual ~Game_interface ()
      {
      }

      virtual std::vector<Player> get_players () const = 0;
      virtual bool start () = 0;
      virtual bool stop () = 0;
      virtual bool is_active () const = 0;
      virtual bool add_player (int idPlayer, const std::string &name) = 0;
      virtual bool remove_player (int idPlayer) = 0;
      virtual bool add_new_word (int idPlayer, const std::string &word) = 0;
      virtual bool challenge (const std::string &word) = 0;
    };

  }
}
#endif
