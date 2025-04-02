#ifndef TWINCENTRAL_APP_CONFIG_INCLUDED
#define TWINCENTRAL_APP_CONFIG_INCLUDED

#define TASK_DEFAULT_PRIORITY 1
#define TASK_DEFAULT_STACK NULL
#define TASK_DEFAULT_STACK_SIZE 400

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace twincentral::app::config {

  static std::string blePeripheralDeviceName = "TwinCentral";
  static constexpr std::size_t adapter = 0;
  static const std::string bleAddr = "centralAddr";
  static const std::chrono::milliseconds dataCollectionTimeout(13000); // 10s
  static const std::chrono::milliseconds discoverTimeout(30000);      // 30s
  static const std::chrono::milliseconds connectTimeout(30000);       // 30s
  static const std::chrono::milliseconds offsettingDuration(15000);   // 15s
  static const std::chrono::milliseconds
      noraxonTriggerSuccessNotificationDuration(1000);

  static const std::string twinCrutchRightBleName = "TwinCrutch-R";
  static const std::string twinCrutchLeftBleName = "TwinCrutch-L";
  static const std::string twinInsoleRightBleName = "TwinInsole-R";
  static const std::string twinInsoleLeftBleName = "TwinInsole-L";
  static const std::string noraxonTriggerBleName = "Noraxon-Trigger";

  static const std::string eulerAnglesJsonKey = "EA";
  static const std::string linearAccelJsonKey = "LA";
  static const std::string quaternionsJsonKey = "Q";
  static const std::string forceVectorJsonKey = "FV";
  static const std::string loadJsonKey = "LOAD";
  static const std::string fsrJsonKey = "FSR";
  static const std::string fsrHeelJsonKey = "H";
  static const std::string fsr5thMetatarsalJsonKey = "5thM";
  static const std::string fsr1thMetatarsalJsonKey = "1thM";

  static const std::unordered_map<std::string,
                                  std::tuple<std::string, std::string>>
      bleTwinPeripheralsByName{
          {twinCrutchRightBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {twinCrutchLeftBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {twinInsoleRightBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {twinInsoleLeftBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {noraxonTriggerBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")}};

  static const std::vector<std::string> bleCollectableTwinPeripherals{
      twinCrutchRightBleName, twinCrutchLeftBleName, twinInsoleRightBleName,
      twinInsoleLeftBleName};

  /* For testing purposes
   *
   *  static const std::unordered_map<std::string,
                                  std::tuple<std::string, std::string>>
      bleTwinPeripheralsByName{
          {twinInsoleRightBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {twinInsoleLeftBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")},
          {noraxonTriggerBleName,
           std::make_tuple("0000b181-0000-1000-8000-00805f9b34fb",
                           "0000cbf6-0000-1000-8000-00805f9b34fb")}};

  static const std::vector<std::string> bleCollectableTwinPeripherals{
      twinInsoleRightBleName, twinInsoleLeftBleName};
  */

  static const bool acceptOnlyDirectedAdvertisement = true;
  static const uint32_t connTimeout = 30000;
}

#endif