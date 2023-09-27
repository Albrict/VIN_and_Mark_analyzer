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
    const Country country_data[countries] = {
        {"AA-AH", "UAR"},
        {"AJ-AN", "Cot D'Ivoire"},
        {"BA-BE", "Angola"},
        {"BF-BK", "Kenia"},
        {"BL-BR", "Tanzania"},
        {"CA-CE", "Benin"},
        {"CF-CK", "Madagascar"},
        {"CL-CR", "Tunisia"},
        {"DA-DE", "Egypt"},
        {"DF-DK", "Morocco"},
        {"DL-DR", "Zambia"},
        {"EA-EE", "Ethiopia"},
        {"EF-EK", "Mozambique"},
        {"FA-FE", "Ghana"},
        {"FF-FK", "Nigeria"},
        {"JA-JT", "Japan"},
        {"KA-KE", "Sri-Lanka"},
        {"KF-KK", "Israel"},
        {"KL-KR", "South Korea"},
        {"KS-K0", "Kazakhstan"},
        {"LA-L0", "China"},
        {"MA-ME", "India"},
        {"MF-MK", "Indonesia"},
        {"ML-MR", "Thailand"},
        {"NF-NK", "Pakistan"},
        {"NL-NR", "Turkey"},
        {"PA-PE", "Philippines"},
        {"PF-PK", "Singapore"},
        {"PL-PR", "Malaysia"},
        {"RA-RE", "UAE"},
        {"RF-RK", "Taiwan"},
        {"RL-RR", "Vietnam"},
        {"RS-R0", "Saudi Arabia"},
        {"SA-SM", "United Kingdom"},
        {"SN-ST", "Germany"},
        {"SU-SZ", "Poland"},
        {"S1-S4", "Latvia"},
        {"TA-TH", "Switzerland"},
        {"TJ-TP", "Czechia"},
        {"TP-TV", "Hungary"},
        {"TW-T1", "Portugal"},
        {"UH-UM", "Denmark"},
        {"UN-UT", "Ireland"},
        {"UU-UZ", "Romania"},
        {"U5-U7", "Slovakia"},
        {"VA-VE", "Austria"},
        {"VF-VR", "France"},
        {"VS-VW", "Spain"},
        {"VX-V2", "Serbia"},
        {"V3-V5", "Croatia"},
        {"V6-V0", "Estonia"},
        {"WA-W0", "Germany"},
        {"XA-XE", "Bulgaria"},
        {"XF-XK", "Greece"},
        {"XL-XR", "Netherlands"},
        {"XS-XW", "USSR/CIS"},
        {"XX-X2", "Luxembourg"},
        {"X3-X0", "Russia"},
        {"YA-YE", "Belgium"},
        {"YF-YK", "Finland"},
        {"YL-YR", "Malta"},
        {"YS-YW", "Sweden"},
        {"YX-Y2", "Norway"},
        {"Y3-Y5", "Belarus"},
        {"Y6-Y0", "Ukraine"},
        {"ZA-ZR", "Italy"},
        {"ZX-Z2", "Slovenia"},
        {"Z3-Z5", "Lithuania"},
        {"Z6-Z0", "Russia"},
        {"1A-10", "USA"},
        {"2A-20", "Canada"},
        {"3A-3W", "Mexico"},
        {"3X-37", "Costa-Rica"},
        {"38-30", "Cayman Islands"},
        {"4A-40", "USA"},
        {"5A-50", "USA"},
        {"6A-6W", "Australia"},
        {"7A-7E", "New Zealand"},
        {"8A-8E", "Argentina"},
        {"8F-8K", "Chile"},
        {"8L-8R", "Ecuador"},
        {"8S-8W", "Peru"},
        {"8X-82", "Venezuela"},
        {"9A-9E", "Brazil"},
        {"9F-9K", "Colombia"},
        {"9L-9R", "Paraguay"},
        {"9S-9W", "Uruguay"},
        {"9X-92", "Trinidad and Tobago"},
        {"93-99", "Brazil"},
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
        [[nodiscard]] int getCharId(const char sym);
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


[[nodiscard]] string VIN::getVINCountry(const string &vin)
{
    const char vin_region_code = vin[0];
    const char vin_country_code = vin[1];
    for (size_t i = 0; i < countries; ++i) {
        const char region_code = country_data[i].code[0];
        if (vin_region_code == region_code) {
            const char min_country_code = country_data[i].code[1];
            const char max_country_code = country_data[i].code[4];
            if (vin_country_code >= min_country_code && vin_country_code <= max_country_code)
                return country_data[i].name;
        }
    }
    [[unlikely]] return "Not used";
}

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

[[nodiscard]] bool VIN::checkSum::verifyCheckSum(const string &vin)
{
    int vin_sum = 0;
    for (size_t i = 0; i < vin.size(); ++i) {
        // Checksum = we don't want to count this
        if (i == 8)
            continue;
        int num_representation;
        int weight = getWeight(i + 1);
        if (std::isalpha(vin[i])) {
            num_representation = getCharId(vin[i]);
//            printf("Char %c number representation: %d\t Position: %zu\n", vin[i], num_representation, i);
        } else {
            num_representation = vin[i] - '0';
//            printf("Number representation: %d\t Position: %zu\n", num_representation, i);
        }
//        std::printf("Weight of %c is: %d\n", vin[i], weight);
        vin_sum += num_representation * weight; 
    }
    const int nearest_smallest_number = (vin_sum / 11) * 11;
    const int check_sum = vin_sum - nearest_smallest_number; 
//    printf("Nearest smallest number: %d\n", nearest_smallest_number);
    std::cout << "Calculated checksum: " << check_sum << '\n';
    std::cout << "VIN checksum: " << vin[8] << '\n';
    if (check_sum == vin[8] - '0') {
        return true;
    } else {
        std::cerr << checksum_error;
        return false;
    }
}

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
