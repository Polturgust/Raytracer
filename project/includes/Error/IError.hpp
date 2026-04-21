/*
** EPITECH PROJECT, 2025
** Nano
** File description:
** IError.hpp
*/

#ifndef IERROR_HPP
    #define IERROR_HPP
    #include <exception>

class IError : public std::exception {
public:
    virtual int code() const noexcept = 0;
    virtual const char *what() const noexcept = 0;
};

#endif
