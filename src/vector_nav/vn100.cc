/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems
*/

#include "vector_nav/vn100.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include "core/core.h"
#include "vector_nav/vector_nav.h"
#include "vector_nav/registers.h"
#include "units/units.h"

namespace sensors {

bool Vn100::Begin() {
  vector_nav_.Init();
  error_code_ = vector_nav_.ReadRegister(&serial_num_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::EnableDrdyInt(const DrdyMode mode, const uint16_t srd) {
  error_code_ = vector_nav_.ReadRegister(&sync_cntrl_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  enum SyncOutPolarity : uint8_t {
    NEG_PULSE = 0,
    POS_PULSE = 1
  };
  sync_cntrl_.payload.sync_out_mode = static_cast<uint8_t>(mode);
  sync_cntrl_.payload.sync_out_polarity = POS_PULSE;
  sync_cntrl_.payload.sync_out_pulse_width = 500000;
  sync_cntrl_.payload.sync_out_skip_factor = srd;
  error_code_ = vector_nav_.WriteRegister(sync_cntrl_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::DisableDrdyInt() {
  error_code_ = vector_nav_.ReadRegister(&sync_cntrl_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  sync_cntrl_.payload.sync_out_mode = 0;
  error_code_ = vector_nav_.WriteRegister(sync_cntrl_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::ApplyRotation(const Eigen::Matrix3f &c) {
  for (std::size_t m = 0; m < 3; m++) {
    for (std::size_t n = 0; n < 3; n++) {
      rotation_.payload.c[m][n] = c(m, n);
    }
  }
  error_code_ = vector_nav_.WriteRegister(rotation_);
  vector_nav_.WriteSettings();
  vector_nav_.Reset();
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetRotation(Eigen::Matrix3f *c) {
  if (!c) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&rotation_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  for (std::size_t m = 0; m < 3; m++) {
    for (std::size_t n = 0; n < 3; n++) {
      (*c)(m, n) = rotation_.payload.c[m][n];
    }
  }
  return true;
}

bool Vn100::SetMagFilter(const FilterMode mode, const uint16_t window) {
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  filter_.payload.mag_filter_mode = static_cast<uint8_t>(mode);
  filter_.payload.mag_window_size = window;
  error_code_ = vector_nav_.WriteRegister(filter_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetMagFilter(FilterMode *mode, uint16_t *window) {
  if ((!mode) || (!window)) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  *mode = static_cast<FilterMode>(filter_.payload.mag_filter_mode);
  *window = filter_.payload.mag_window_size;
  return true;
}

bool Vn100::SetAccelFilter(const FilterMode mode, const uint16_t window) {
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  filter_.payload.accel_filter_mode = static_cast<uint8_t>(mode);
  filter_.payload.accel_window_size = window;
  error_code_ = vector_nav_.WriteRegister(filter_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetAccelFilter(FilterMode *mode, uint16_t *window) {
  if ((!mode) || (!window)) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  *mode = static_cast<FilterMode>(filter_.payload.accel_filter_mode);
  *window = filter_.payload.accel_window_size;
  return true;
}

bool Vn100::SetGyroFilter(const FilterMode mode, const uint16_t window) {
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  filter_.payload.gyro_filter_mode = static_cast<uint8_t>(mode);
  filter_.payload.gyro_window_size = window;
  error_code_ = vector_nav_.WriteRegister(filter_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetGyroFilter(FilterMode *mode, uint16_t *window) {
  if ((!mode) || (!window)) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  *mode = static_cast<FilterMode>(filter_.payload.gyro_filter_mode);
  *window = filter_.payload.gyro_window_size;
  return true;
}

bool Vn100::SetTemperatureFilter(const FilterMode mode, const uint16_t window) {
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  filter_.payload.temp_filter_mode = static_cast<uint8_t>(mode);
  filter_.payload.temp_window_size = window;
  error_code_ = vector_nav_.WriteRegister(filter_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetTemperatureFilter(FilterMode *mode, uint16_t *window) {
  if ((!mode) || (!window)) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  *mode = static_cast<FilterMode>(filter_.payload.temp_filter_mode);
  *window = filter_.payload.temp_window_size;
  return true;
}

bool Vn100::SetPressureFilter(const FilterMode mode, const uint16_t window) {
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  filter_.payload.pres_filter_mode = static_cast<uint8_t>(mode);
  filter_.payload.pres_window_size = window;
  error_code_ = vector_nav_.WriteRegister(filter_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::GetPressureFilter(FilterMode *mode, uint16_t *window) {
  if ((!mode) || (!window)) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  error_code_ = vector_nav_.ReadRegister(&filter_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  *mode = static_cast<FilterMode>(filter_.payload.pres_filter_mode);
  *window = filter_.payload.pres_window_size;
  return true;
}


bool Vn100::DrdyCallback(const uint8_t int_pin, void (*function)()) {
  if (!function) {
    error_code_ = VectorNav::ERROR_NULL_PTR;
    return false;
  }
  pinMode(int_pin, INPUT);
  attachInterrupt(int_pin, function, RISING);
  error_code_ = VectorNav::ERROR_SUCCESS;
  return true;
}

bool Vn100::VelocityCompensation(float speed_mps) {
  vel_comp_.payload.velocity_x = speed_mps;
  vel_comp_.payload.velocity_y = 0.0f;
  vel_comp_.payload.velocity_z = 0.0f;
  error_code_ = vector_nav_.WriteRegister(vel_comp_);
  return (error_code_ == VectorNav::ERROR_SUCCESS);
}

bool Vn100::Read() {
  error_code_ = vector_nav_.ReadRegister(&attitude_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  error_code_ = vector_nav_.ReadRegister(&imu_);
  if (error_code_ != VectorNav::ERROR_SUCCESS) {return false;}
  return true;
}

Eigen::Vector3f Vn100::accel_mps2() {
  Eigen::Vector3f accel;
  accel(0) = attitude_.payload.accel_x;
  accel(1) = attitude_.payload.accel_y;
  accel(2) = attitude_.payload.accel_z;
  return accel;
}

Eigen::Vector3f Vn100::gyro_radps() {
  Eigen::Vector3f gyro;
  gyro(0) = attitude_.payload.gyro_x;
  gyro(1) = attitude_.payload.gyro_y;
  gyro(2) = attitude_.payload.gyro_z;
  return gyro;
}

Eigen::Vector3f Vn100::mag_ut() {
  Eigen::Vector3f mag;
  mag(0) = conversions::Gauss_to_uT(attitude_.payload.mag_x);
  mag(1) = conversions::Gauss_to_uT(attitude_.payload.mag_y);
  mag(2) = conversions::Gauss_to_uT(attitude_.payload.mag_z);
  return mag;
}

Eigen::Vector3f Vn100::uncomp_accel_mps2() {
  Eigen::Vector3f accel;
  accel(0) = imu_.payload.accel_x;
  accel(1) = imu_.payload.accel_y;
  accel(2) = imu_.payload.accel_z;
  return accel;
}

Eigen::Vector3f Vn100::uncomp_gyro_radps() {
  Eigen::Vector3f gyro;
  gyro(0) = imu_.payload.gyro_x;
  gyro(1) = imu_.payload.gyro_y;
  gyro(2) = imu_.payload.gyro_z;
  return gyro;
}

Eigen::Vector3f Vn100::uncomp_mag_ut() {
  Eigen::Vector3f mag;
  mag(0) = conversions::Gauss_to_uT(imu_.payload.mag_x);
  mag(1) = conversions::Gauss_to_uT(imu_.payload.mag_y);
  mag(2) = conversions::Gauss_to_uT(imu_.payload.mag_z);
  return mag;
}

}  // namespace sensors
