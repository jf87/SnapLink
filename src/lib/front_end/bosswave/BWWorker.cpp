#include "lib/front_end/bosswave/BWWorker.h"

const std::string BWWorker::none = "None";

BWWorker::BWWorker(PMessage message,
                   std::function<std::vector<std::string>(
                       const cv::Mat &image, const CameraModel &camera)>
                       onQuery,
                   std::atomic<unsigned int> &numClients)
    : _msg(message), _onQuery(onQuery), _numClients(numClients) {}

void BWWorker::process() {
  if (_msg->POs().length() != BW_MSG_LENGTH) {
    std::cerr << "invalid message format\n" << std::endl;
    emit error();
  }

  std::vector<const char *> contents;
  std::vector<int> lens;

  foreach (auto po, _msg->POs()) {
    contents.push_back(po->content());
    lens.push_back(po->length());
  }

  double fx, fy, cx, cy;
  std::stringstream ss;
  // TODO: Add comments here to explain why 5 and 8
  for (int i = 5; i <= 8; i++) {
    ss << std::string(contents[i], lens[i]) << " ";
  }
  ss >> fx >> fy >> cx >> cy;
  cv::Mat image;
  CameraModel camera;
  std::vector<char> rawData;
  rawData.reserve(IMAGE_INIT_SIZE);
  rawData.insert(rawData.end(), contents[2], contents[2] + lens[2]);
  createData(rawData, fx, fy, cx, cy, image, camera);
  if (image.empty()) {
    std::cerr << "Creating image failed" << std::endl;
    emit error();
  }

  // blocking wait
  std::vector<std::string> results = _onQuery(image, camera);

  std::string result = none;
  if (!results.empty()) {
    result = std::move(results.at(0));
  }

  std::cerr << "DEBUG: emmiting done()" << std::endl;
  emit done(QString::fromStdString(result),
            QString::fromStdString(std::string(contents[1], lens[1])));
}

void BWWorker::createData(const std::vector<char> &data, double fx, double fy,
                          double cx, double cy, cv::Mat &image,
                          CameraModel &camera) {
  const bool copyData = false;
  image = imdecode(cv::Mat(data, copyData), cv::IMREAD_GRAYSCALE);

  // imwrite("image.jpg", image);

  int width = image.cols;
  int height = image.rows;
  camera = CameraModel("", fx, fy, cx, cy, cv::Size(width, height));
}