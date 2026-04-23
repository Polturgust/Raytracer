/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Tile.hpp
*/

#ifndef TILE_HPP
    #define TILE_HPP
    #include <array>

namespace raytracer {

typedef enum {
    NOTCOMPUTE = -1,
    COMPUTING = 0,
    COMPUTED = 1,
} TileState;

class Tile {
    std::array<int, 4> _color;
    TileState _state = NOTCOMPUTE;

public:
    Tile(std::array<int, 4> color): _color(color) {};
    ~Tile() = default;

    void SetRed(int r) {_color[0] = r;};
    void SetGreen(int g) {_color[1] = g;};
    void SetBlue(int b) {_color[2] = b;};
    void SetAlpha(int a) {_color[3] = a;};
    void SetColor(std::array<int, 4> color) {_color = color;};
    void SetState(TileState state) {_state = state;};

    int GetRed(void) {return _color[0];};
    int GetBlue(void) {return _color[1];};
    int GetGreen(void) {return _color[2];};
    int GetAlpha(void) {return _color[3];};
    std::array<int, 4> GetColor(void) {return _color;};
    TileState GetState(void) {return _state;};
};

}

#endif
