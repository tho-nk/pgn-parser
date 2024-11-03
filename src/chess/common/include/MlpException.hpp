#pragma once

#include <exception>
#include <iostream>

namespace mlp_ha {
class MlpException : public std::runtime_error {
  public:
    explicit MlpException(const char *message) : std::runtime_error(message) {}

    explicit MlpException(const std::string &message) : std::runtime_error(message) {}

    explicit MlpException(std::string &&message) : std::runtime_error(std::move(message)) {}

};
} // namespace mlp_ha
