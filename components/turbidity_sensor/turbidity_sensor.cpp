#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *const TAG = "turbidity.sensor";

void TurbiditySensor::update() {
    if (this->expander_parent_ != nullptr) {
        this->expander_parent_->select_channel(channel_);
    }
    request_data_();
    if (!wait_for_response_()) {
        ESP_LOGW(TAG, "No response from turbidity sensor");
        return;
    }
    std::vector<uint8_t> response = read_response_();
    float value;

    if (type_ == TurbiditySensorType::TURBIDITY) {
        if (parse_dirty_response_(response, value)) {
            ESP_LOGD(TAG, "Parsed dirty value: %.2f", value);
            if (turbidity_sensor_ != nullptr) {
                turbidity_sensor_->publish_state(value);
            }
        } else {
            ESP_LOGW(TAG, "Failed to parse dirty response");
        }
    } else {
        if (parse_adc_response_(response, value)) {
            ESP_LOGD(TAG, "Parsed ADC value: %.2f", value);
            if (adc_sensor_ != nullptr) {
                adc_sensor_->publish_state(value);
            }
        } else {
            ESP_LOGW(TAG, "Failed to parse ADC response");
        }
    }
}

void TurbiditySensor::request_data_() {
    std::vector<uint8_t> command;
    if (type_ == TurbiditySensorType::TURBIDITY) {
        command = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A}; // Example dirty command
    } else {
        command = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA}; // Example ADC command
    }
    write_command_(command);
}


bool TurbiditySensor::wait_for_response_() {
    // Implement your waiting and timeout logic here
    // Return true if a valid response is received, false otherwise
    delay(100); // Short delay to allow response
    return this->available() > 0; // Check if data is available
}

bool TurbiditySensor::parse_dirty_response_(const std::vector<uint8_t> &response, float &value) {
    if (response.size() < 7 || response[0] != 0x01 || response[1] != 0x03 || response[2] != 0x02) {
        ESP_LOGW(TAG, "Invalid response from sensor: incorrect header or length for dirty");
        return false;
    }

    uint16_t raw_value = (response[3] << 8) | response[4]; // 0-4096 range
    value = static_cast<float>(raw_value);

    uint16_t received_crc = (response[5] << 8) | response[6];
    uint16_t calculated_crc = esphome::crc16(&response[0], response.size() - 2);
    if (received_crc != calculated_crc) {
        ESP_LOGW(TAG, "CRC mismatch (dirty): Expected %04X, Got %04X", calculated_crc, received_crc);
        return false;
    }
    return true;
}

bool TurbiditySensor::parse_adc_response_(const std::vector<uint8_t> &response, float &value) {
    if (response.size() < 7 || response[0] != 0x01 || response[1] != 0x03 || response[2] != 0x02) {
        ESP_LOGW(TAG, "Invalid response from sensor: incorrect header or length for ADC");
        return false;
    }

    uint16_t raw_value = (response[3] << 8) | response[4]; // 0-4096 range
    value = static_cast<float>(raw_value);

    uint16_t received_crc = (response[5] << 8) | response[6];
    uint16_t calculated_crc = esphome::crc16(&response[0], response.size() - 2);
    if (received_crc != calculated_crc) {
        ESP_LOGW(TAG, "CRC mismatch (ADC): Expected %04X, Got %04X", calculated_crc, received_crc);
        return false;
    }
    return true;
}

void TurbiditySensor::write_command_(const std::vector<uint8_t>& command) {
    ESP_LOGD(TAG, "Writing command");
    this->write_array(command);
}

std::vector<uint8_t> TurbiditySensor::read_response_() {
    std::vector<uint8_t> response;
    while (this->available()) {
        response.push_back(this->read());
    }
    return response;
}

}  // namespace turbidity_sensor
}  // namespace esphome

