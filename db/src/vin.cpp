//Library implementation for VIN numbers analysis
//To understand more, check this link:
//https://en.wikipedia.org/wiki/Vehicle_identification_number

#include <array>
#include <iostream>
#include "vin.hpp"

using string = std::string;
using size_t = std::size_t;

namespace VIN {
    struct Country {
        const string code;
        const string name;

        Country(const string &code, const string &name)
            : code(code), name(name) {}
    };
    const size_t countries = 89;
    const std::array<const Country, countries> country_data = {
        {
            Country("AA-AH", "UAR"), 
            Country("AJ-AN", "Cot D'Ivoire"),
            Country("BA-BE", "Angola"),
            Country("BF-BK", "Kenia"),
            Country("BL-BR", "Tanzania"),
            Country("CA-CE", "Benin"),
            Country("CF-CK", "Madagascar"),
            Country("CL-CR", "Tunisia"),
            Country("DA-DE", "Egypt"),
            Country("DF-DK", "Morocco"),
            Country("DL-DR", "Zambia"),
            Country("EA-EE", "Ethiopia"),
            Country("EF-EK", "Mozambique"),
            Country("FA-FE", "Ghana"),
            Country("FF-FK", "Nigeria"),
            Country("JA-JT", "Japan"),
            Country("KA-KE", "Sri-Lanka"),
            Country("KF-KK", "Israel"),
            Country("KL-KR", "South Korea"),
            Country("KS-K0", "Kazakhstan"),
            Country("LA-L0", "China"),
            Country("MA-ME", "India"),
            Country("MF-MK", "Indonesia"),
            Country("ML-MR", "Thailand"),
            Country("NF-NK", "Pakistan"),
            Country("NL-NR", "Turkey"),
            Country("PA-PE", "Philippines"),
            Country("PF-PK", "Singapore"),
            Country("PL-PR", "Malaysia"),
            Country("RA-RE", "UAE"),
            Country("RF-RK", "Taiwan"),
            Country("RL-RR", "Vietnam"),
            Country("RS-R0", "Saudi Arabia"),
            Country("SA-SM", "United Kingdom"),
            Country("SN-ST", "Germany"),
            Country("SU-SZ", "Poland"),
            Country("S1-S4", "Latvia"),
            Country("TA-TH", "Switzerland"),
            Country("TJ-TP", "Czechia"),
            Country("TP-TV", "Hungary"),
            Country("TW-T1", "Portugal"),
            Country("UH-UM", "Denmark"),
            Country("UN-UT", "Ireland"),
            Country("UU-UZ", "Romania"),
            Country("U5-U7", "Slovakia"),
            Country("VA-VE", "Austria"),
            Country("VF-VR", "France"),
            Country("VS-VW", "Spain"),
            Country("VX-V2", "Serbia"),
            Country("V3-V5", "Croatia"),
            Country("V6-V0", "Estonia"),
            Country("WA-W0", "Germany"),
            Country("XA-XE", "Bulgaria"),
            Country("XF-XK", "Greece"),
            Country("XL-XR", "Netherlands"),
            Country("XS-XW", "USSR/CIS"),
            Country("XX-X2", "Luxembourg"),
            Country("X3-X0", "Russia"),
            Country("YA-YE", "Belgium"),
            Country("YF-YK", "Finland"),
            Country("YL-YR", "Malta"),
            Country("YS-YW", "Sweden"),
            Country("YX-Y2", "Norway"),
            Country("Y3-Y5", "Belarus"),
            Country("Y6-Y0", "Ukraine"),
            Country("ZA-ZR", "Italy"),
            Country("ZX-Z2", "Slovenia"),
            Country("Z3-Z5", "Lithuania"),
            Country("Z6-Z0", "Russia"),
            Country("1A-10", "USA"),
            Country("2A-20", "Canada"),
            Country("3A-3W", "Mexico"),
            Country("3X-37", "Costa-Rica"),
            Country("38-30", "Cayman Islands"),
            Country("4A-40", "USA"),
            Country("5A-50", "USA"),
            Country("6A-6W", "Australia"),
            Country("7A-7E", "New Zealand"),
            Country("8A-8E", "Argentina"),
            Country("8F-8K", "Chile"),
            Country("8L-8R", "Ecuador"),
            Country("8S-8W", "Peru"),
            Country("8X-82", "Venezuela"),
            Country("9A-9E", "Brazil"),
            Country("9F-9K", "Colombia"),
            Country("9L-9R", "Paraguay"),
            Country("9S-9W", "Uruguay"),
            Country("9X-92", "Trinidad and Tobago"),
            Country("93-99", "Brazil")
        }
    };

    const size_t vin_size = 17;
    const char *illegal_chars = "IOQ";
    const char *vin_size_error = "Error! VIN number size is invalid!\n";
    const char *illegal_vin_number_ioq_symbols_error = "Error! Illegal VIN argument: VIN have I, O, Q symbols!\n"; 
    const char *illegal_vin_number_symbols_error = "Error! Illegal VIN argument: VIN have illegal symbols!\n";
    const char *illegal_vin_number_checksum_error = "Error! Checksum is not properly set in VIN number!\n";
    const char *checksum_error = "Error! Checksum is invalid!\n";  

    [[nodiscard]] bool checkForIllegalCharacters(const string &vin);
    namespace checkSum {
        [[nodiscard]] inline int getCharId(const char sym);
        [[nodiscard]] inline bool verifyCheckSum(const string &vin); 
        [[nodiscard]] inline int getWeight(const size_t position);
    }
};

[[nodiscard]] bool VIN::checkVIN(const string &vin)
{
    // Firstly check size of string
    if (vin.size() != vin_size)
        return false;
    // Check correctness of VIN
    if (!checkForIllegalCharacters(vin)) 
        return false;
    if (!checkSum::verifyCheckSum(vin))
        return false;
    return true;
}

// This algorithm first checks the region code, 
// if there is a match, the algorithm starts looking for the country code in that region
[[nodiscard]] string VIN::getVINCountry(const string &vin)
{
    const char vin_region_code = vin[0];
    const char vin_country_code = vin[1];
    for (size_t i = 0; i < countries; ++i) {
        const char region_code = country_data[i].code[0];
        // Is there match?
        if (vin_region_code == region_code) {
            const char min_country_code = country_data[i].code[1];
            const char max_country_code = country_data[i].code[4];
            // Handle case when max country code is digit 
            if (std::isdigit(max_country_code)) {
                if (std::isdigit(vin_country_code)) {
                    if (max_country_code - '0' == 0)
                        return country_data[i].name;
                    if (vin_country_code <= max_country_code)
                        return country_data[i].name;
                } else 
                    return country_data[i].name;
            }
            if (vin_country_code >= min_country_code && vin_country_code <= max_country_code)
                return country_data[i].name;
        }
    }
    [[unlikely]] return "Not used";
}
//Returns the year of manufacture of the vehicle. If the code is a letter, 
//the year is calculated based on the position of the letter in the alphabet, 
//otherwise if the code is a number, just add 2001 + the number.
[[nodiscard]] int VIN::getTransportYear(const string &vin)
{
    const char code = vin[9];
    int year = 0;
    if (std::isalpha(code)) {
        if (code >= 'A' && code <= 'Y') {
            const int code_alphabet_position = code - 'A' - 1;
            year += code_alphabet_position + 2010;
        }
    } else if (std::isdigit(code)) {
        const int digit_code = code - '0';
        year += digit_code + 2000; 
    }
    return year;
}

[[nodiscard]] bool VIN::checkForIllegalCharacters(const string &vin)
{
    // Check for checksum integer
    if (vin[8] != 'X') {
        if (!std::isdigit(vin[8])) {
            std::cerr << illegal_vin_number_checksum_error;
            return false;
        }
    }
    // Check for illegal characters
    for (const auto symbol : vin) {
        const bool first_result = std::isalnum(symbol);
        if (!first_result) {
            std::cerr << illegal_vin_number_symbols_error;
            return false;
        }
        if (vin.find_first_of(illegal_chars) != string::npos) {
            std::cerr << illegal_vin_number_ioq_symbols_error;
            return false;
        }

    }
    return true;
}

// Returns char ID based on checksum table for symbols
[[nodiscard]] inline int VIN::checkSum::getCharId(const char sym)
{
    int char_id = 0;
    if (sym <= 'H')
        char_id = sym - 'A' + 1;
    else if (sym >= 'J' && sym <= 'R')
        char_id = sym - 'H' - 1;
    else if (sym >= 'S' && sym <= 'Z')
        char_id = sym - 'R' + 1;
    return char_id;
}
// Algorithm of finding the checksum for VIN number. For reference, see this link
// https://en.wikipedia.org/wiki/Vehicle_identification_number#Check-digit_calculation
[[nodiscard]] bool VIN::checkSum::verifyCheckSum(const string &vin)
{
    int vin_sum = 0;
    for (size_t i = 0; i < vin.size(); ++i) {
        // Checksum = we don't want to count this
        if (i == 8)
            continue;
        int num_representation;
        int weight = getWeight(i + 1);
        if (std::isalpha(vin[i]))
            num_representation = getCharId(vin[i]);
        else
            num_representation = vin[i] - '0';
        vin_sum += num_representation * weight; 
    }
    const int nearest_smallest_number = (vin_sum / 11) * 11;
    const int check_sum = vin_sum - nearest_smallest_number; 
    std::cout << "Calculated checksum: " << check_sum << '\n';
    std::cout << "VIN checksum: " << vin[8] << '\n';
    if (check_sum == vin[8] - '0') {
        return true;
    } else {
        std::cerr << checksum_error;
        return false;
    }
}

// Returns weight of the symbol based on his position
[[nodiscard]] inline int VIN::checkSum::getWeight(const size_t position)
{
    int weight = 0;
    if (position < 8) {
        weight = 8;
        for (size_t i = 1; i < position; ++i)
            --weight;
    } else if (position == 8) {
        weight = 10;
    } else if (position >= 10) {
        weight = 9;
        for (size_t i = 10; i < position; ++i)
            --weight; 
    }
    return weight;
}
