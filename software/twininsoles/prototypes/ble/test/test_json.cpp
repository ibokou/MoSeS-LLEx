#include "../main/lib/json.hpp"
#include <iostream>
#include <string>

using namespace nlohmann::literals;
using json = nlohmann::json;

int main() {
  const char *const json_literal = R"({"pi": 3.141,"happy": true})";
  std::cout << json_literal << std::endl;
  std::string json_string(json_literal);
  auto data = json::parse(json_literal);
  std::cout << data["pi"] << std::endl;

  auto mf_name = "Hallo";

  json ex3 = {{"membershipFunction", mf_name},
              {"MaxMinInference",
               {{"MeanMaxima", 100.0f},
                {"CenterofGravity", 100.0f},
                {"WeightedAverage", 100.0f}}},
              {"MaxProductInference",
               {{"MeanMaxima", 100.0f},
                {"CenterofGravity", 100.0f},
                {"WeightedAverage", 100.0f}}}};

  std::cout << ex3.dump(4) << std::endl;
}