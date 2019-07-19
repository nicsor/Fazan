#ifndef GAME_INTERFACES_GAME_INTERFACE_H
#define GAME_INTERFACES_GAME_INTERFACE_H

#include <stdint.h>
#include <string>
#include <map>

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

      virtual std::map<Player_Id, Player_ptr> get_players () const = 0;
      virtual bool start () = 0;
      virtual bool stop () = 0;
      virtual bool is_active () const = 0;
      virtual bool add_player (Player_Id idPlayer, const std::string &name) = 0;
      virtual bool remove_player (Player_Id idPlayer) = 0;
      virtual bool submit_new_word (Player_Id idPlayer, const std::string &word) = 0;
      virtual bool challenge (const std::string &word) = 0;
    };

  }
}
#endif
