#ifndef CSV_H
#define CSV_H
#include <optional>
#include <string>
#include <svm.h>
#include <vector>
namespace libcsv {
struct CSV_SERDER {
public:
  CSV_SERDER(std::vector<int> index, int typeindex);
  std::vector<std::pair<int, std::vector<double>>>
  get_messages(std::string fname);
  std::pair<std::vector<int>, std::vector<std::vector<double>>>
  svm_information(std::string fname);
  // std::optional<int> predict(std::vector<double>);

private:
  std::vector<int> index;
  int type_index;
};
struct SVM_MODEL {
  using message = std::pair<std::vector<int>, std::vector<std::vector<double>>>;

public:
  SVM_MODEL(message messages);
  ~SVM_MODEL();
  std::optional<int> predict(std::vector<double> input);

private:
  int elements;
  svm_parameter svmpara;
  svm_model *model;
};
} // namespace libcsv
#endif // !CSV_H
