#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool copyFile(const std::string& SRC, const std::string& DEST)
{
  std::ifstream src(SRC, std::ios::binary);
  std::ofstream dest(DEST, std::ios::binary);
  dest << src.rdbuf();
  return src && dest;
}

int main() {
  std::map<int, char> months{{1, 'F'}, {2, 'G'},{3, 'H'},{4, 'J'},{5, 'K'},{6, 'M'},{7, 'N'},{8, 'Q'},{9, 'U'},{10, 'V'},{11, 'X'},{12, 'Z'}};
  std::ifstream json_file("..\\config.json");
  json json_data = json::parse(json_file);

  std::string path;
  std::cout << "Enter path:";
  std::cin >> path;
  for (const auto &date : fs::directory_iterator(path)) {
    std::string datename = date.path().filename().string();
    char month = months[(std::stoi(datename.substr(3,2)) + 1)%12];
    for (const auto &name : fs::directory_iterator(date.path())) {
      std::string filename = name.path().filename().string();
      for (auto& [conf_name,conf_value] : json_data.items()) { // ищем ключ, под который подходит название файла
        if (filename.size() - conf_name.size() >= 2) {
          if (filename.substr(0, conf_name.size()) == conf_name && filename[conf_name.size() + 1] == month) {
            copyFile(name.path().string(), date.path().string() + "\\" + conf_name + '!');
          }
        }
      }
    }
  }
}
