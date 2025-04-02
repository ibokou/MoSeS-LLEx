#include "os.h"
#include <cstdlib>
#include <fmt/format.h>


bool twincentral::os::shutdown(uint32_t wait) {
  auto cmd = fmt::format("sudo sleep {}; sudo shutdown now", wait);
  return (std::system(cmd.c_str()) == 0);
}

bool twincentral::os::restart(uint32_t wait) {
    auto cmd = fmt::format("sudo sleep {}; sudo shutdown -r now", wait);
    return (std::system(cmd.c_str()) == 0);
}

void twincentral::os::restartBluetoothService() {
    auto cmd = "sudo systemctl restart bluetooth.service";
    std::system(cmd);
}
