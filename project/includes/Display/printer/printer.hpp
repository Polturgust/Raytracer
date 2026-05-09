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
    std::size_t LineWrite = 0;
public:
    Printer() = default;
    ~Printer() = default;

    void MoveCursorUp(int n) {std::cout << "\033[" << n << "A";};
    void ClearLine() {std::cout << "\033[2K\r";};
    void PrintLine(std::string line) {std::cout << line << std::endl; LineWrite++;};
    void PrintLine(std::string line, int nb_line) {std::cout << line << std::endl; LineWrite += nb_line;};

    void ClearNbLine(int nb) {for (int i = nb; i > 0; i--) {std::cout << "\033[1A"; std::cout << "\033[2K\r";}};
    void ClearToStart() {this->ClearNbLine(LineWrite); LineWrite = 0;};
    void SetStart() {LineWrite = 0;};
};

}

#endif
