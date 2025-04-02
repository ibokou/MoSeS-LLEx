#ifndef TWINPERIPHERAL_EXCEPTION_BASE_INCLUDED
#define TWINPERIPHERAL_EXCEPTION_BASE_INCLUDED

#include <exception>
#include <stdexcept>

namespace twinperipheral::exception {
  class BaseException : public std::exception {
  public:
    BaseException(const std::string &message = "");
    const char *what();

  private:
    std::string message;
  };
}

#endif