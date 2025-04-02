#include "file.h"
#include "config/ble.h"
#include <string>

twincentral::io::file::SynchronizedBioMetricsFile::SynchronizedBioMetricsFile(
    const std::string &dir, const std::string &fileName) {

  if (!std::filesystem::exists(dir)) {
    if (!std::filesystem::create_directories(dir)) {
      throw twincentral::io::file::exception::
          BioMetricsFileFailedCreationException();
    }
  }
  filePath = dir + fileName;
  const std::string header = std::string("Timestamp,Fuzzy_Gait_Pct,COP,") +
                             "CR_EA,CL_EA,IR_EA,IL_EA," +
                             "CR_LA,CL_LA,IR_LA,IL_LA," +
                             "CR_Q,CL_Q,IR_Q,IL_Q," + "CR_F,CR_FV,CL_F,CL_FV," +
                             "IR_FSR,IL_FSR" + "\n";

  std::stringstream sstream(header);
  doc.Load(sstream);
  doc.Save(filePath);
}

twincentral::io::file::SynchronizedBioMetricsFile::
    ~SynchronizedBioMetricsFile() {
  doc.Save();
}

void twincentral::io::file::SynchronizedBioMetricsFile::write(
    const json &data,
    std::chrono::time_point<std::chrono::steady_clock> startTime) {
  const std::lock_guard<std::mutex> lock(mtx);
  auto current = std::chrono::steady_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(current -
                                                                    startTime);
  auto dataToBeStored = transformData(data);
  dataToBeStored.insert(dataToBeStored.begin(), std::to_string(time.count()));
  doc.Load(filePath);
  doc.SetRow(doc.GetRowCount(), dataToBeStored);
  doc.Save(filePath);
}

std::string twincentral::io::file::SynchronizedBioMetricsFile::getLatest() {
  const std::lock_guard<std::mutex> lock(mtx);
  doc.Load(filePath);
  const std::size_t latestRow = doc.GetRowCount() - 1;
  std::string result;
  for (auto &entry : doc.GetRow<std::string>(latestRow)) {
    if (!result.empty()) {
      result += ", "; // Add a comma and space if not the first element
    }
    result += entry; // Concatenate the element
  }
  return result;
}

std::vector<std::string>
twincentral::io::file::SynchronizedBioMetricsFile::transformData(
    const json &data) {

  std::vector<std::string> transformedData{};

  // General (calculated) Values
  auto cop = to_string(data["COP"]["x"]) + "|" + to_string(data["COP"]["y"]);
  std::vector<std::string> generalValues{to_string(data["GaitPct"]), cop};

  std::vector<std::string> eulerAngles{};

  std::vector<std::string> linearAccels{};

  std::vector<std::string> quaternions{};

  std::vector<std::string> adcValues{};

  for (auto &peripheralName :
       twincentral::app::config::bleCollectableTwinPeripherals) {

    // Euler Angles
    auto euler =
        to_string(data[peripheralName]
                      [twincentral::app::config::eulerAnglesJsonKey]["x"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::eulerAnglesJsonKey]["y"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::eulerAnglesJsonKey]["z"]);
    eulerAngles.push_back(euler);

    // Linear Acceleration

    auto linearAccel =
        to_string(data[peripheralName]
                      [twincentral::app::config::linearAccelJsonKey]["x"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::linearAccelJsonKey]["y"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::linearAccelJsonKey]["z"]);
    linearAccels.push_back(linearAccel);

    // Quaternions

    auto quaternion =
        to_string(data[peripheralName]
                      [twincentral::app::config::quaternionsJsonKey]["w"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::quaternionsJsonKey]["x"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::quaternionsJsonKey]["y"]) +
        "|" +
        to_string(data[peripheralName]
                      [twincentral::app::config::quaternionsJsonKey]["z"]);
    quaternions.push_back(quaternion);

    // Load
    if (data[peripheralName].contains(twincentral::app::config::loadJsonKey)) {
      auto forceVector =
          to_string(data[peripheralName]
                        [twincentral::app::config::forceVectorJsonKey]["x"]) +
          "|" +
          to_string(data[peripheralName]
                        [twincentral::app::config::forceVectorJsonKey]["y"]) +
          "|" +
          to_string(data[peripheralName]
                        [twincentral::app::config::forceVectorJsonKey]["z"]);
      adcValues.push_back(to_string(
          data[peripheralName][twincentral::app::config::loadJsonKey]));
      adcValues.push_back(forceVector);
    }

    if (data[peripheralName].contains(twincentral::app::config::fsrJsonKey)) {
      auto fsr =
          to_string(data[peripheralName][twincentral::app::config::fsrJsonKey]
                        [twincentral::app::config::fsrHeelJsonKey]) +
          "|" +
          to_string(data[peripheralName][twincentral::app::config::fsrJsonKey]
                        [twincentral::app::config::fsr5thMetatarsalJsonKey]) +
          "|" +
          to_string(data[peripheralName][twincentral::app::config::fsrJsonKey]
                        [twincentral::app::config::fsr1thMetatarsalJsonKey]);
      adcValues.push_back(fsr);
    }
  }

  transformedData.insert(transformedData.end(), generalValues.begin(),
                         generalValues.end());
  transformedData.insert(transformedData.end(), eulerAngles.begin(),
                         eulerAngles.end());
  transformedData.insert(transformedData.end(), linearAccels.begin(),
                         linearAccels.end());
  transformedData.insert(transformedData.end(), quaternions.begin(),
                         quaternions.end());
  transformedData.insert(transformedData.end(), adcValues.begin(),
                         adcValues.end());

  return transformedData;
}
