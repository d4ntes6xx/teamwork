#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "network.hpp"

int main() {
    Game game;
    Network net(false);
    game.myTurn = false;

    sf::RenderWindow window({ Game::SIZE * Game::CELL, Game::SIZE * Game::CELL }, "Tic Tac Toe - Client (O)");

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e))
            if (e.type == sf::Event::Closed)
                window.close();

        int x, y;
        if (net.receive(x, y)) {
            game.makeMove(x, y, Player::X);
            game.myTurn = true;
        }

        if (game.myTurn && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto pos = sf::Mouse::getPosition(window);
            x = pos.x / Game::CELL;
            y = pos.y / Game::CELL;
            if (game.makeMove(x, y, Player::O)) {
                net.send(x, y);
                game.myTurn = false;
            }
        }

        game.draw(window);
        if (game.winner != Player::None) {
            sf::sleep(sf::seconds(2));
            game.reset();
            game.myTurn = false;
        }
    }
}
