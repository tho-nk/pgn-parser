#pragma once

#include <exception>
#include <iostream>

namespace mlp_ha {
class MlpException : public std::runtime_error {
  public:
    // Constructor accepting a const char* message
    explicit MlpException(const char *message) : std::runtime_error(message) {}

    // Constructor accepting a std::string message
    explicit MlpException(const std::string &message) : std::runtime_error(message) {}

    // Move constructor
    explicit MlpException(std::string &&message) : std::runtime_error(std::move(message)) {}

};
} // namespace mlp_ha
