#include "Camera.h"
#include <cmath>

const kmuvcl::math::vec3f Camera::center_position() const
{
  kmuvcl::math::vec3f center_position(position_(0) + front_dir_(0),
                 position_(1) + front_dir_(1),
                 position_(2) + front_dir_(2));
  return center_position;
}

void Camera::move_forward(float delta)
{
    kmuvcl::math::vec3f dir = front_dir_;
    float normal_number = delta / sqrt(dir(0)*dir(0) + dir(1)*dir(1) + dir(2)*dir(2));
    dir(0) *=  normal_number;
    dir(1) *=  normal_number;
    dir(2) *=  normal_number;

    position_ += dir;
}

void Camera::move_backward(float delta)
{

  kmuvcl::math::vec3f dir = front_dir_;
  float normal_number = delta / sqrt(dir(0)*dir(0) + dir(1)*dir(1) + dir(2)*dir(2));
  dir(0) *=  normal_number;
  dir(1) *=  normal_number;
  dir(2) *=  normal_number;

  position_ -= dir;
}

void Camera::move_left(float delta)
{

  kmuvcl::math::vec3f dir = right_dir_;
  float normal_number = delta / sqrt(dir(0)*dir(0) + dir(1)*dir(1) + dir(2)*dir(2));
  dir(0) *=  normal_number;
  dir(1) *=  normal_number;
  dir(2) *=  normal_number;

  position_ -= dir;
}

void Camera::move_right(float delta)
{

  kmuvcl::math::vec3f dir = right_dir_;
  float normal_number = delta / sqrt(dir(0)*dir(0) + dir(1)*dir(1) + dir(2)*dir(2));
  dir(0) *=  normal_number;
  dir(1) *=  normal_number;
  dir(2) *=  normal_number;

  position_ += dir;
}

void Camera::rotate_left(float delta)
{
  float radianAngle = delta * (M_PI / 180.0f);

  front_dir_(0) = front_dir_(0) * cos(radianAngle) + front_dir_(2) * sin(radianAngle);
  front_dir_(2) = -front_dir_(0) * sin(radianAngle) + front_dir_(2) * cos(radianAngle);
  right_dir_(0) = right_dir_(0) * cos(radianAngle) + right_dir_(2) * sin(radianAngle);
  right_dir_(2) = -right_dir_(0) * sin(radianAngle) + right_dir_(2) * cos(radianAngle);
}

void Camera::rotate_right(float delta)
{
  float radianAngle = delta * (M_PI / 180.0f);

  front_dir_(0) = front_dir_(0) * cos(radianAngle) - front_dir_(2) * sin(radianAngle);
  front_dir_(2) = front_dir_(0) * sin(radianAngle) + front_dir_(2) * cos(radianAngle);
  right_dir_(0) = right_dir_(0) * cos(radianAngle) - right_dir_(2) * sin(radianAngle);
  right_dir_(2) = right_dir_(0) * sin(radianAngle) + right_dir_(2) * cos(radianAngle);
}
