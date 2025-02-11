#include "turbidity_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace turbidity_sensor {

static const char *const TAG = "turbidity.sensor";

void TurbiditySensor::update() {
    request_data_();
    if (!wait_for_response_()) {
        ESP_LOGW(TAG, "No response from turbidity sensor");
        return;
    }
    process_response_();
}

void TurbiditySensor::request_data_() {
    static const uint8_t dirty_command[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    static const uint8_t adc_command[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
    const uint8_t *command;
    size_t command_size;

    if (type_ == TurbiditySensorType::TURBIDITY) {
        command = dirty_command;
        command_size = sizeof(dirty_command);
    } else {
        command = adc_command;
        command_size = sizeof(adc_command);
    }

    for (size_t i = 0; i < command_size; ++i) {
        write(command[i]);
    }
}

bool TurbiditySensor::wait_for_response_() {
    // Implement your waiting and timeout logic here
    // Return true if a valid response is received, false otherwise
    delay(100); // Short delay to allow response
    return available() > 0; // Check if data is available
}

void TurbiditySensor::process_response_() {
    while (available()) {
        uint8_t byte = read();
        rx_buffer_.push_back(byte);
    }
    float value = extract_value(rx_buffer_);

    if (type_ == TurbiditySensorType::TURBIDITY && turbidity_sensor_ != nullptr) {
        turbidity_sensor_->publish_state(value);
    } else if (type_ == TurbiditySensorType::ADC && adc_sensor_ != nullptr) {
        adc_sensor_->publish_state(value);
    } else {
        ESP_LOGW(TAG, "No sensor to publish state to!");
    }
}

float TurbiditySensor::extract_value(const std::vector<uint8_t> &response) {
    if (response.size() >= 7) {
        uint16_t high_byte = response[3];
        uint16_t low_byte = response[4];
        return (high_byte << 8) | low_byte;
    } else {
        ESP_LOGW(TAG, "Invalid response length: %d", response.size());
        return NAN;
    }
}

}  // namespace turbidity_sensor
}  // namespace esphome
