#pragma once

#include <exception>
#include <iostream>

namespace pgn {
class PgnException : public std::runtime_error {
  public:
    explicit PgnException(const char *message) : std::runtime_error(message) {}

    explicit PgnException(const std::string &message) : std::runtime_error(message) {}

    explicit PgnException(std::string &&message) : std::runtime_error(std::move(message)) {}
};
} // namespace pgn
