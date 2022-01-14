#include <csv.h>
#include <iostream>
int main(int argc, char *argv[]) {
  std::cout << "Hello " << std::endl;

  auto metacsv = libcsv::CSV_SERDER({0, 1, 2, 3, 4, 5, 6}, 14);
  std::vector<std::pair<int, std::vector<double>>> output;
  try {
    output = metacsv.get_messages("csvs/persion1.csv");
  } catch (char const *e) {
    std::cout << e << std::endl;
  } catch (std::invalid_argument e) {
    std::cout << "invalid_argument" << std::endl;
  }
  for (auto apair : output) {
    std::cout << apair.first << ",";
    for (auto name : apair.second) {
      std::cout << name << ",";
    }
    std::cout << std::endl;
  }
  std::pair<std::vector<int>, std::vector<std::vector<double>>> output2;
  try {
    output2 = metacsv.svm_information("csvs/persion1.csv");
  } catch (char const *e) {
    std::cout << e << std::endl;
  } catch (std::invalid_argument e) {
    std::cout << "invalid_argument" << std::endl;
  }
  int len = output2.first.size();
  for (size_t i = 0; i < len; i++) {
    std::cout << output2.first[i] << ":";
    for (auto m : output2.second[i]) {
      std::cout << m << ",";
    }
    std::cout << std::endl;
  }
  libcsv::SVM_MODEL model = libcsv::SVM_MODEL(output2);
  auto output3 = model.predict(
      {50.8449, -19.0803, 138.693, 139.965, 109.852, 65.1941, -7.0376});
  if (output3.has_value()) {
		std::cout << output3.value() << std::endl;
  } else {
    std::cout << "None" << std::endl;
  }
  return 0;
}
