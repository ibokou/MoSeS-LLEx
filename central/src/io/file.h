#ifndef TWINCENTRAL_IO_FILE_INCLUDED
#define TWINCENTRAL_IO_FILE_INCLUDED

#include "csv/rapidcsv.h"
#include "error/error.h"
#include "json/json.hpp"
#include <chrono>
#include <filesystem>
#include <mutex>
#include <string>
#include <vector>

namespace {
  using json = nlohmann::json;
}

namespace twincentral::io::file {

  namespace exception {
    class BioMetricsFileFailedCreationException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };
  }

  class SynchronizedBioMetricsFile {
  public:
    SynchronizedBioMetricsFile(const std::string &path = "./",
                               const std::string &fileName = "example.csv");
    ~SynchronizedBioMetricsFile();
    void write(const json &data,
               std::chrono::time_point<std::chrono::steady_clock> startTime);
    std::string getLatest();

  private:
    std::vector<std::string> transformData(const json &data);
    std::string filePath;
    std::mutex mtx;
    rapidcsv::Document doc;
  };
}

#endif