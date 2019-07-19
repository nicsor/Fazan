#include "Player.h"

namespace game
{

  bool Player::has_lost() const
  {
    return (score == 0);
  }

  void Player::set_match_lost ()
  {
    if (not has_lost())
    {
      --score;
    }
  }

  Player::Player (Player_Id id, const std::string &name) :
      id (id), name (name), score (5)
  {
  }

  Player_Id Player::get_id () const
  {
    return id;
  }

  std::string Player::get_name () const
  {
    return name;
  }

  uint8_t Player::get_score () const
  {
    return score;
  }

  std::ostream& operator<< (std::ostream &stream, const Player &player)
  {
    stream << player.get_name ();

    return stream;
  }

}
