#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#include <sdbus-c++/sdbus-c++.h>

namespace sdbus {
  using Dict = std::map<std::string, sdbus::Variant>;
  using ByteArray = std::vector<uint8_t>;
  using DictDict = std::map<std::string, std::map<std::string, sdbus::Variant>>;
}

#endif