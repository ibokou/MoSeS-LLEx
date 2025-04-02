#ifndef TWINCENTRAL_ERR_INCLUDED
#define TWINCENTRAL_ERR_INCLUDED

#include <exception>
#include <stdexcept>

namespace twincentral::exception {
  class TwincentralBaseException : public std::exception {
  public:
    TwincentralBaseException(const std::string &message = "")
        : message(message){};
    const char *what() { return (this->message).c_str(); };

  private:
    std::string message;
  };
}

#endif