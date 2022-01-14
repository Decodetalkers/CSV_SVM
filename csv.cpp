#include "csv.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#define Malloc(type, n) (type *)malloc((n) * sizeof(type))
constexpr svm_parameter Initialize_svm_parameter() {
  svm_parameter svmpara; // svm的相关参数
  svmpara.svm_type = C_SVC;
  svmpara.kernel_type = LINEAR;
  svmpara.degree = 3;
  svmpara.gamma = 0;
  svmpara.coef0 = 0;
  svmpara.nu = 0.5;
  svmpara.cache_size = 1; //缓存块大小
  svmpara.C = 1;
  svmpara.eps = 1e-3;
  svmpara.p = 0.1;
  svmpara.shrinking = 1;
  svmpara.probability = 0;
  svmpara.nr_weight = 0;
  svmpara.weight_label = NULL;
  svmpara.weight = NULL;
  return svmpara;
}
namespace libcsv {
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
        } else if (index_now == index[float_index]) {
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
std::pair<std::vector<int>, std::vector<std::vector<double>>>
CSV_SERDER::svm_information(std::string fname) {
  std::vector<int> first;
  std::vector<std::vector<double>> second;
  std::pair<std::vector<int>, std::vector<std::vector<double>>> messages;
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
        } else if (index_now == index[float_index]) {
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
        first.push_back(localtype);
        second.push_back(context);
        // messages.push_back(std::make_pair(localtype, context));
      }
    }
  } else {
    throw "Error Open";
  }
  messages = std::make_pair(first, second);
  file.close();
  return messages;
}
SVM_MODEL::SVM_MODEL(message messages) : svmpara(Initialize_svm_parameter()) {
  int sizeOfProblem = messages.first.size();
  elements = messages.second[0].size();
  svm_problem prob;
  svm_node *x_space;
  prob.l = sizeOfProblem;
  prob.y = Malloc(double, prob.l);
  prob.x = Malloc(struct svm_node *, prob.l);
  x_space = Malloc(struct svm_node, (elements + 1) * prob.l);
  for (size_t i = 0; i < prob.l; i++) {
    prob.y[i] = messages.first[i];
  }
  int j = 0;
  for (size_t i = 0; i < prob.l; i++) {
    prob.x[i] = &x_space[j];
    for (int k = 0; k < messages.second[i].size(); ++k, ++j) {
      x_space[j].index = k + 1;                 // index of value
      x_space[j].value = messages.second[i][k]; // value
    }
    x_space[j].index = -1; // state the end of data vector
    x_space[j].value = 0;
    j++;
  }
  model = svm_train(&prob, &svmpara);
  delete x_space;
}
std::optional<int> SVM_MODEL::predict(std::vector<double> input) {
  if (input.size() != elements) {
    return std::nullopt;
  }
  svm_node *test = new svm_node[elements + 1];
  for (size_t i = 0; i < elements; i++) {
    test[i].index = i + 1;
    test[i].value = input[i];
  }
  test[elements].index = -1;
  int result = svm_predict(model, test);
  delete[] test;
  return result;
}
SVM_MODEL::~SVM_MODEL() { delete model; }
} // namespace libcsv
