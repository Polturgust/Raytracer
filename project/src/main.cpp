/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main.cpp
*/

#include "main.hpp"

int main(int argc, char **argv) {
    try {
        raytracer::Argument(argc, argv);
    } catch (const IError& e) {
        if (e.code() == 84) {
            std::cerr << "Raytracer : " << e.what();
            return 84;
        } else
            std::cout << e.what();
        return 0;
    }
    return 0;
}