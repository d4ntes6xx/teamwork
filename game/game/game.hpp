#pragma once
#include <SFML/Graphics.hpp>
#include <array>

enum class Player { None, X, O };

class Game {
public:
    static constexpr int SIZE = 3;
    static constexpr int CELL = 150;
    std::array<std::array<Player, SIZE>, SIZE> board{};
    Player current = Player::X;
    Player winner = Player::None;
    bool myTurn = true;
    sf::Font font;

    Game() {
        font.loadFromFile("arial.ttf");
    }

    void reset() {
        for (auto& row : board) row.fill(Player::None);
        winner = Player::None;
        current = Player::X;
        myTurn = true;
    }

    bool makeMove(int x, int y, Player p) {
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return false;
        if (board[y][x] != Player::None) return false;
        board[y][x] = p;
        winner = checkWin();
        return true;
    }

    Player checkWin() {
        for (int i = 0; i < SIZE; ++i)
            if (board[i][0] != Player::None && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return board[i][0];

        for (int i = 0; i < SIZE; ++i)
            if (board[0][i] != Player::None && board[0][i] == board[1][i] && board[1][i] == board[2][i])
                return board[0][i];

        if (board[0][0] != Player::None && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return board[0][0];

        if (board[0][2] != Player::None && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return board[0][2];

        return Player::None;
    }

    void draw(sf::RenderWindow& window) {
        window.clear(sf::Color::White);
        sf::RectangleShape line;
        for (int i = 1; i < SIZE; ++i) {
            line.setSize({ SIZE * CELL, 4 });
            line.setPosition(0, i * CELL);
            line.setFillColor(sf::Color::Black);
            window.draw(line);

            line.setSize({ 4, SIZE * CELL });
            line.setPosition(i * CELL, 0);
            window.draw(line);
        }

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(100);
        t.setFillColor(sf::Color::Black);

        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (board[y][x] != Player::None) {
                    t.setString(board[y][x] == Player::X ? "X" : "O");
                    t.setPosition(x * CELL + 40, y * CELL + 20);
                    window.draw(t);
                }

        window.display();
    }
};
