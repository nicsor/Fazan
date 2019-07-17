#include <iostream>
#include <string>

#include "Game.h"
#include "Status.h"

void print_options ()
{
  std::cout << "\ts - start game" << std::endl;
  std::cout << "\tx - stop game" << std::endl;
  std::cout << "\tc - new word" << std::endl;
  std::cout << "\tj - new player" << std::endl;
  std::cout << "\td - delete player" << std::endl;
  std::cout << "\tl - list players" << std::endl;
}

int main ()
{
  game::interfaces::Status_interface_ptr outputPtr = std::make_shared<game::Status> (
      game::Status ());
  game::Game game (outputPtr);

  print_options ();

  while (1)
  {
    char command;
    std::cin >> command;

    switch (command)
    {
      case 's':
      {
        std::cout << "# Request to start new game" << std::endl;
        game.start ();
        break;
      }
      case 'j':
      {
        std::cout << "# Add new player" << std::endl;
        int id;
        std::string nume;
        std::cout << "Player ID: ";
        std::cin >> id;
        std::cout << "Player name: ";
        std::cin >> nume;
        game.add_player (id, nume);
        break;
      }
      case 'c':
      {
        std::cout << "# Request to input data" << std::endl;
        int id;
        std::string cuvant;
        std::cout << "Player ID: ";
        std::cin >> id;
        std::cout << "Word: ";
        std::cin >> cuvant;

        game.add_new_word (id, cuvant);
        break;
      }
      case 'd':
      {
        std::cout << "# Request to delete player" << std::endl;
        int id;
        std::cout << "Player ID: ";
        std::cin >> id;

        game.remove_player (id);
        break;
      }
      case 'l':
      {
        std::cout << "# Request to list players" << std::endl;

        for (auto player : game.get_players())
        {
          std::cout << player << std::endl;
        }

        break;
      }
      case '?':
      {
        std::cout << "# Request to challenge word" << std::endl;
        std::string cuvant;
        std::cout << "Word: ";
        std::cin >> cuvant;

        game.challenge (cuvant);
        break;
      }
      case 'x':
      {
        std::cout << "# Request to stop game" << std::endl;
        game.stop ();
        return 0;
        break;
      }
      default:
        break;
    }
  }

  return 0;
}
