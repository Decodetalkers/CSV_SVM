#include "csv.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace libcsv;
// create a new struct to handle csvs
CSV_SERDER::CSV_SERDER(std::vector<int> index, int typeindex)
    : index(index), type_index(typeindex) {}
std::vector<std::pair<int, std::vector<double>>>
CSV_SERDER::get_messages(std::string fname) {
  std::vector<std::pair<int, std::vector<double>>> messages;
  std::sort(index.begin(), index.end());
  const int len = index.size();
  std::fstream file(fname, std::ios::in);
  std::string line, word;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::stringstream str(line);
      int index_now = 0;
      int float_index = 0;
      int localtype = -1;
      std::vector<double> context;
      while (std::getline(str, word, ',')) {
        if (index_now == type_index) {
          try {
            localtype = std::stoi(word);
          } catch (std::invalid_argument e) {
            throw;
          }
        } else if (index_now == float_index) {
          float message;
          try {
            message = std::stod(word);
          } catch (std::invalid_argument e) {
            throw;
          }
          context.push_back(message);
          float_index += 1;
        }
        index_now += 1;
      }
      if (localtype == -1) {
        throw "No index, search for it";
      } else {
        messages.push_back(std::make_pair(localtype, context));
      }
    }
  } else {
    throw "Error Open";
  }
  file.close();
  return messages;
}
