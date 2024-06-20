#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;
namespace fs = std::filesystem;

// функция копирования файла SRC в файл DEST в бинарном режиме
bool copyFile(const std::string& SRC, const std::string& DEST) {
  std::ifstream src(SRC, std::ios::binary);
  std::ofstream dest(DEST, std::ios::binary);
  dest << src.rdbuf();
  return src && dest;
}

// функция проверки корректности даты формата dd-mm-yyyy
bool isDate(const std::string& name) {
  if (name.size() != 10) return false;
  if (name[2] != '-' || name[5] != '-') return false;
  if (!isdigit(name[0]) || !isdigit(name[1]) || !isdigit(name[3]) ||
      !isdigit(name[4]) || !isdigit(name[6]) || !isdigit(name[7]) ||
      !isdigit(name[8]) || !isdigit(name[9]))
    return false;
  if (!(name[0] == '0' || name[0] == '1' || name[0] == '2' || name[0] == '3'))
    return false;
  if (!(name[3] == '0' || name[3] == '1')) return false;
  return true;
}

int main() {
  std::unordered_map<int, char>
      months{{1, 'F'},  {2, 'G'}, {3, 'H'}, {4, 'J'}, {5, 'K'},
             {6, 'M'},  {7, 'N'}, {8, 'Q'}, {9, 'U'}, {10, 'V'},
             {11, 'X'}, {0, 'Z'}};  // словарь букв, принадлежащие месяцам

  std::ifstream json_file("..\\config.json");  // считываем config.json
  json json_data = json::parse(json_file);

  std::string path;
  std::cout << "Enter path:";
  std::cin >> path;

  // проходимся по папкам в 'NAME'
  for (const auto& date : fs::directory_iterator(path)) {
    // получаем название каталога
    std::string datename = date.path().filename().string();
    if (!isDate(datename)) continue;

    // получаем букву следующего месяца
    char month =
        months[((datename[3] - '0') * 10 + datename[4] - '0' + 1) % 12];

    // проходимся по файлам в папке 'date'
    for (const auto& files : fs::directory_iterator(date.path())) {
      std::string filename = files.path().filename().string();

      // ищем ключ, под который подходит название файла
      for (auto& [conf_name, conf_value] : json_data.items()) {
        if (filename.size() - conf_name.size() >= 2) {
          // условие копирования файла
          if (filename.substr(0, conf_name.size()) == conf_name &&
              filename[conf_name.size() + 1] == month) {
            if (!copyFile(files.path().string(),
                          date.path().string() + "\\" + conf_name + '!')) {
              std::cerr << "Error copying the file: " + filename << std::endl;
            }
          }
        }
      }
    }
  }
  return 0;
}
