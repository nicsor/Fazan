#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <stdint.h>
#include <string>

namespace game
{

  class Player
  {
  protected:
    uint16_t    id;
    std::string name;
    uint8_t     score;

  protected:
    friend class Game_interface; ///< Permit to set a match as lost
    void set_match_lost ();

  public:
    Player (uint16_t id, const std::string &name);

    bool        has_lost () const;
    uint16_t    get_id () const;
    std::string get_name () const;
    uint8_t     get_score () const;

    friend std::ostream& operator<< (std::ostream &stream, const Player &player);
  };

}

#endif
