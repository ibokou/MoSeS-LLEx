#ifndef BLE_EVENT_H
#define BLE_EVENT_H
namespace BLE::events {
  enum Event {
    BLE_CONN = 11,
    BLE_DISCONN = 12,
    REQ_BENCHMARK1 = 21,
    REQ_BENCHMARK2 = 22,
    REQ_BENCHMARK3 = 23,
    REQ_STOP_BENCHMARK = 24
  };
}
#endif