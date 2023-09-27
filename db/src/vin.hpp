#pragma once
#include <string>

namespace VIN {
    struct Region {
        const std::string geographical_zone;
        const std::string country;

        Region(const std::string &zone, const std::string &country)
            : geographical_zone(zone), country(country) {}
    };

    [[nodiscard]] bool checkVIN(const std::string &vin);
    [[nodiscard]] std::string getVINCountry(const std::string &vin);
    [[nodiscard]] std::string getRegion(const std::string &vin);
    [[nodiscard]] int getTransportYear(const std::string &vin);
}
