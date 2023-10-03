#pragma once
#include <string>

namespace VIN {
    // Checks the VIN number and returns true or false depending on the correctness of the VIN number
    [[nodiscard]] bool checkVIN(const std::string &vin);
    // Returns VIN country, if not found - returns "Not used"
    [[nodiscard]] std::string getVINCountry(const std::string &vin);
    //Returns the year in which the vehicle was manufactured from 2001 to 2029
    [[nodiscard]] int getTransportYear(const std::string &vin);
}
