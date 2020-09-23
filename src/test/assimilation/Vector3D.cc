/*
 * (C) Crown copyright 2020, Met Office
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "test/assimilation/Vector3D.h"

namespace test {

  Vector3D::Vector3D(const double& x,
                     const double& y,
                     const double& z)
    : x_(x), y_(y), z_(z)
  {}

  Vector3D& Vector3D::operator=(const Vector3D& rhs)
  {
    this->x_ = rhs.x_;
    this->y_ = rhs.y_;
    this->z_ = rhs.z_;
    return *this;
  }

  Vector3D& Vector3D::operator+=(const Vector3D& rhs)
  {
    this->x_ += rhs.x_;
    this->y_ += rhs.y_;
    this->z_ += rhs.z_;
    return *this;
  }

  Vector3D& Vector3D::operator-=(const Vector3D& rhs)
  {
    this->x_ -= rhs.x_;
    this->y_ -= rhs.y_;
    this->z_ -= rhs.z_;
    return *this;
  }

  Vector3D& Vector3D::operator*=(const double mult)
  {
    this->x_ *= mult;
    this->y_ *= mult;
    this->z_ *= mult;
    return *this;
  }

  Vector3D& Vector3D::operator*=(const Vector3D& rhs)
  {
    this->x_ *= rhs.x_;
    this->y_ *= rhs.y_;
    this->z_ *= rhs.z_;
    return *this;
  }

  Vector3D& Vector3D::operator/=(const Vector3D& rhs)
  {
    this->x_ /= rhs.x_;
    this->y_ /= rhs.y_;
    this->z_ /= rhs.z_;
    return *this;
  }

  void Vector3D::axpy(const double mult, const Vector3D& rhs)
  {
    this->x_ += mult * rhs.x_;
    this->y_ += mult * rhs.y_;
    this->z_ += mult * rhs.z_;
  }

  double Vector3D::dot_product_with(const Vector3D& rhs) const
  {
    return this->x_ * rhs.x_ + this->y_ * rhs.y_ + this->z_ * rhs.z_;
  }
}  // namespace test
