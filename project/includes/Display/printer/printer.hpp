/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** printer.hpp
*/

#ifndef PRINTER_HPP
    #define PRINTER_HPP
    #include <iostream>

namespace raytracer {

class Printer {
public:
    Printer() = default;
    ~Printer() = default;

    void moveCursorUp(int n) {std::cout << "\033[" << n << "A";};
    void clearLine() {std::cout << "\033[2K\r";}
};

}

#endif
