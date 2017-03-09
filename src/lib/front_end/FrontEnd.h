#pragma once

#include <functional>
#include <vector>
#include <memory>
#include "lib/data/CameraModel.h"

class FrontEnd {
public:
  virtual ~FrontEnd() = default;

  /**
   * start the front end asynchronously
   */
  virtual bool start() = 0;

  /**
   * stop front end synchronously
   */
  virtual void stop() = 0;

  /**
   * register a callback function
   */
  void registerOnQuery(std::function<std::vector<std::string>(const cv::Mat &image, const CameraModel &camera)> onQuery) {_onQuery = onQuery;}

  /**
   * call the callback function
   */
  std::function<std::vector<std::string>(const cv::Mat &image, const CameraModel &camera)> getOnQuery() {return _onQuery;}

private:
  std::function<std::vector<std::string>(const cv::Mat &image, const CameraModel &camera)> _onQuery;
};