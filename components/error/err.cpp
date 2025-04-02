#include "error/err.hpp"

twinperipheral::exception::BaseException::BaseException(
    const std::string &message)
    : message(message){};
const char *twinperipheral::exception::BaseException::what() {
  return (this->message).c_str();
};
