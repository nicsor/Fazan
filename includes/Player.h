#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <stdint.h>
#include <memory>
#include <string>

namespace game
{
  typedef uint8_t Player_Id;

  class Player
  {
  protected:
    Player_Id    id;
    std::string name;
    uint8_t     score;

  protected:
    friend class Game_interface; ///< Permit to set a match as lost
  public:
    void set_match_lost ();

  public:
    Player (Player_Id id, const std::string &name);

    bool        has_lost () const;
    Player_Id   get_id () const;
    std::string get_name () const;
    uint8_t     get_score () const;

    friend std::ostream& operator<< (std::ostream &stream, const Player &player);
  };

  typedef std::shared_ptr<Player> Player_ptr;
}

#endif
