#ifndef TWINCENTRAL_OS_INCLUDED
#define TWINCENTRAL_OS_INCLUDED

#include <cstdint>

namespace twincentral::os {
  bool shutdown(uint32_t wait);
  bool restart(uint32_t wait);
  void restartBluetoothService();
}

#endif