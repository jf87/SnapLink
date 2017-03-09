#pragma once

#include "lib/data/Label.h"
#include <map>
#include <list>
#include <memory>
#include <numeric>

class CameraModel;
class Transform;

class Visibility final {
public:
  explicit Visibility(const std::map<int, std::list<Label>> &labels);

  std::vector<std::string> process(int dbId, const CameraModel &camera,
                                   const Transform &pose) const;

private:
  const std::map<int, std::list<Label>> &_labels;
};

struct CompareMeanDist {
  typedef std::pair<std::string, std::vector<double>> PairType;

  static double meanDist(const std::vector<double> &vec);
  bool operator()(const PairType &left, const PairType &right) const;
};