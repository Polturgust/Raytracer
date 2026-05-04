/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** SfmlDisplay.hpp
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP
    #include <vector>
    #include <SFML/Graphics.hpp>

    #include "Tile.hpp"

namespace raytracer {

class Sfmldisplay {
    sf::RenderWindow window;
public:
    Sfmldisplay(std::size_t x, std::size_t y): window(sf::VideoMode(x, y), "Raytracer") {};
    ~Sfmldisplay() {
        window.close();
    }

    void render(const std::vector<std::vector<Tile>>& map) {
        window.clear();
        int height = map.size();
        int width  = map[0].size();

        sf::Texture texture;
        texture.create(width, height);
        std::vector<sf::Uint8> data;
        data.reserve(width * height * 4);
        for (auto& row : map)
            for (auto& pixel : row) {
                data.push_back(pixel.GetRed());
                data.push_back(pixel.GetGreen());
                data.push_back(pixel.GetBlue());
                data.push_back(255);
            }
        texture.update(data.data());
        sf::Sprite sprite(texture);
        window.draw(sprite);
        window.display();
    }
};

}

#endif