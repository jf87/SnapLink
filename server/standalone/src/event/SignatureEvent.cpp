#include "event/SignatureEvent.h"

const QEvent::Type SignatureEvent::_type =
    static_cast<QEvent::Type>(QEvent::registerEventType());

SignatureEvent::SignatureEvent(
    std::unique_ptr<std::vector<int>> &&wordIds,
    std::unique_ptr<std::vector<cv::KeyPoint>> &&keyPoints,
    std::unique_ptr<CameraModel> &&camera,
    std::unique_ptr<std::vector<int>> &&signatureIds,
    std::unique_ptr<Session> &&session)
    : QEvent(SignatureEvent::type()), _wordIds(std::move(wordIds)),
      _keyPoints(std::move(keyPoints)), _camera(std::move(camera)),
      _signatureIds(std::move(signatureIds)), _session(std::move(session)) {}

std::unique_ptr<std::vector<int>> SignatureEvent::takeWordIds() {
  return std::move(_wordIds);
}

std::unique_ptr<std::vector<cv::KeyPoint>> SignatureEvent::takeKeyPoints() {
  return std::move(_keyPoints);
}

std::unique_ptr<CameraModel> SignatureEvent::takeCameraModel() {
  return std::move(_camera);
}

std::unique_ptr<std::vector<int>> SignatureEvent::takeSignatureIds() {
  return std::move(_signatureIds);
}

std::unique_ptr<Session> SignatureEvent::takeSession() {
  return std::move(_session);
}

QEvent::Type SignatureEvent::type() { return _type; }