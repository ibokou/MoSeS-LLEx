#ifndef BLUEZ_UTIL_H
#define BLUEZ_UTIL_H
#include "type_def.h"
#include <cstdint>
#include <string>
#include <vector>

static sdbus::ByteArray convertStringToByteArray(std::string &mString) {
  sdbus::ByteArray myVector(mString.begin(), mString.end());
  return myVector;
}

static std::string convertByteArrayToString(sdbus::ByteArray value) {
  std::vector<char> charArr(value.begin(), value.end());
  std::string str(charArr.begin(), charArr.end());
  return str;
}

#endif