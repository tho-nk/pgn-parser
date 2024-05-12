#pragma once

#include <exception>
#include <iostream>

namespace mlp_ha {
class MlpException : public std::exception {
  public:
    MlpException(const char *message) : m_message(message) {}

    // Override what() method to provide a custom error message
    const char *what() const noexcept override { return m_message.c_str(); }

  private:
    std::string m_message;
};
} // namespace mlp_ha
