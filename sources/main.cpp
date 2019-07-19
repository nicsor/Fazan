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

void run_test(game::interfaces::Game_interface &game)
{
  game.add_player (1, "Dan");
  game.add_player (2, "Maria");
  game.add_player (3, "George");
  game.start ();

  std::cout << std::endl << "Player 1 sends word" << std::endl;
  game.submit_new_word (1, "crocobaur");
  std::this_thread::sleep_for (std::chrono::milliseconds (200)); // wait for server to process the input
  std::cout << std::endl << "Player 2 sends word" << std::endl;
  game.submit_new_word (2, "urangutan");
  std::this_thread::sleep_for (std::chrono::milliseconds (200)); // wait for server to process the input
  std::cout << std::endl << "Player 3 sends final word" << std::endl;
  game.submit_new_word (3, "ant"); // add closing word
  // Player 1 looses

  // New round
  std::this_thread::sleep_for (std::chrono::milliseconds (200)); // wait for server to process the input
  std::cout << std::endl << "Player 2 sends word (new game)" << std::endl;
  game.submit_new_word (2, "abecedar");
  std::cout << std::endl << "Player 3 sends invalid word (does not start with right value)" << std::endl;
  std::this_thread::sleep_for (std::chrono::milliseconds (200)); // wait for server to process the input
  game.submit_new_word (3, "abecedar");   // invalid word
  std::this_thread::sleep_for (std::chrono::milliseconds (1200)); // wait for timeout
  // Player 3 looses . wrong word

  std::cout << std::endl << "Player 2 exits. Set score to 0" << std::endl;
  game.remove_player(2);
  std::cout << std::endl << "No player input ... expect all to start loosing" << std::endl;

  // Remaining players start loosing one after the other.
  while(1);
}

int main ()
{
  game::interfaces::Status_interface_ptr outputPtr = std::make_shared<game::Status> (
      game::Status ());
  game::Game game(outputPtr, 1);

  // Todo : remove
  run_test(game);

  print_options ();

  while (1)
  {
    char command = 0;
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

        if (game.add_player (id, nume))
        {
          std::cout << "Player added" << std::endl;
        }
        else
        {
          std::cout << "Player already present" << std::endl;
        }
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

        if (game.submit_new_word (id, cuvant))
        {
          std::cout << "Word ok" << std::endl;
        }
        else
        {
          std::cout << "Word not expected" << std::endl;
        }
        break;
      }
      case 'd':
      {
        std::cout << "# Request to delete player" << std::endl;
        int id;
        std::cout << "Player ID: ";
        std::cin >> id;

        if (game.remove_player (id))
        {
          std::cout << "Player removed" << std::endl;
        }
        else
        {
          std::cout << "Player not present" << std::endl;
        }
        break;
      }
      case 'l':
      {
        std::cout << "# Request to list players" << std::endl;

        for (auto player : game.get_players())
        {
          std::cout << player.second.get()->get_name() << std::endl;
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
