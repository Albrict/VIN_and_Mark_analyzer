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
    const std::array<VIN::Country, 2> country_data { {"ff", "f"}};
    enum GEOGRAPHICAL_ZONES { AFRICA, ASIA, EUROPE, NORTH_AMERIKA, OCEANIA, SOUTH_AMERIKA, NONE }; enum AFRICA_COUNTRIES { UAR, COTE_D_IVOIRE, ANGOLA, KENIA, TANZANIA, BENIN, MADAGASCAR, TUNISIA, EGYPT, MOROCCO, ZAMBI, ETHIOPI, MOZAMBIQU, GHANA, NIGERIA };
    enum ASIA_COUNTRIES { JAPAN, SRI_LANKA, ISRAEL, SOUTH_KOREA, KAZAKHSTAN, CHINA, INDIA, INDONESIA, 
                          THAILAND, PAKISTAN, TURKEY, PHILIPPINES, SINGAPORE, MALAYSIA, UAE, TAIWAN, VIETNAM, SAUDI_ARABIA };
    enum EUROPE_COUNTRIES {UNITED_KINGDOM, GERMANY, POLAND, LATVIA, SWITZERLAND, CZECHIA, HUNGARY, PORTUGAL, 
                           DENMARK, IRELAND, ROMANIA, SLOVAKIA, AUSTRIA, FRANCE, SPAIN, SERBIA, CROATIA, ESTONIA, BULGARIA, GREECE, NETHERLANDS, USSR_CIS, 
                           LUXEMBOURG, RUSSIA, BELGIUM, FINLAND, MALTA, SWEDEN, NORWAY, BELARUS, UKRAINE, ITALY, SLOVENIA, LITHUANIA};
    enum NORTH_AMERIKA_COUNTRIES { USA, CANADA, MEXICO, COSTA_RICA, CAYMAN_ISLANDS};
    enum OCEANIA_COUNTRIES {AUSTRALIA, NEW_ZEALAND};
    enum SOUTH_AMERIKA_COUNTRIES {ARGENTINA, CHILE, ECUADOR, PERU, VENEZUELA, BRAZIL, COLOMBIA, PARAGUAY, URUGUAY, TRINIDAD_AND_TOBAGO};

    const std::array<const string, 7> geographical_zones_names = {"Africa", "Asia", "Europe", "North Amerika", "Oceania", "South Amerika", "None"};
    const std::array<const string, 15> africa_country_names = {"UAR", "Cote d'Ivoire", "Angola", "Kenia", "Tanzania", "Benin", "Madagascar", "Tunisia", "Egypt",
                                                         "Morocco", "Zambia", "Ethiopia", "Mozambique", "Ghana", "Nigeria"};
    const std::array<const string, 18> asia_country_names = {"Japan", "Sri Lanka", "Israel", "South Korea", "Kazakhstan", "China", "India", "Indonesia"
                                                             "Thailand", "Pakistan", "Turkey", "Philippines", "Singapore", "Malaysia", "UAE", "Taiwan", "Vietnam",
                                                             "Saudi Arabia"};
    const std::array<const string, 36> europe_country_names = { "United Kingdom", "Germany", "Poland", "Latvia", "Switzerland", "Czechia", "Hungary", "Portugal", 
                            "Denmark", "Ireland", "Romania", "Slovakia", "Austria", "France", "Spain", "Serbia", "Croatia", "Estonia", "Bulgaria", "Greece", 
                            "Netherlands", "USSS_CIS", "Luxembourg", "Russia", "Belgium", "Finland", "Malta", "Sweden", "Norway" "Belarus" "Ukraine" "Italy"
                            "Slovenia", "Lithuania"};
    const std::array<const string, 5> north_amerika_countries_names = {"USA", "Canada", "Mexico", "Costa Rica", "Cayman Islands"};
    const std::array<const string, 2> oceania_countries_names = {"Australia",  "New Zealand"};
    const std::array<const string, 10> south_amerika_countries_names = {"Argentina", "Chile", "Ecuador", "Peru", "Venezuela", "Brazil", "Colombia", "Paraguay",
                                                                        "Uruguay", "Trinidad and Tobago"};

    const size_t vin_size = 17;
    const char *illegal_chars = "IOQ";
    const char *vin_size_error = "Error! VIN number size is invalid!\n";
    const char *illegal_vin_number_ioq_symbols_error = "Error! Illegal VIN argument: VIN have I, O, Q symbols!\n"; 
    const char *illegal_vin_number_symbols_error = "Error! Illegal VIN argument: VIN have illegal symbols!\n";
    const char *illegal_vin_number_checksum_error = "Error! Checksum is not properly set in VIN number!\n";
    const char *checksum_error = "Error! Checksum is invalid!\n";  

    [[nodiscard]] bool checkForIllegalCharacters(const string &vin);

    [[nodiscard]] inline GEOGRAPHICAL_ZONES defineGeographicalZone(const char zone_id);
    [[nodiscard]] inline string defineAsianCountry(const string &region_code);
    [[nodiscard]] inline string defineAfricaCountry(const string &region_code);
    [[nodiscard]] inline string defineEuropeCountry(const string &region_code);
    [[nodiscard]] inline string defineNorthAmerikaCountry(const string &region_code);
    [[nodiscard]] inline string defineSouthAmerikaCountry(const string &region_code);
    [[nodiscard]] inline string defineOceaniaCountry(const string &region_code);

    [[nodiscard]] string defineCountry(const string &region_code);

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
    const GEOGRAPHICAL_ZONES zone = defineGeographicalZone(vin[0]);
    return geographical_zones_names[zone]; 
}

[[nodiscard]] int VIN::getTransportYear(const string &vin)
{
    ;
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
            printf("Char %c number representation: %d\t Position: %zu\n", vin[i], num_representation, i);
        } else {
            num_representation = vin[i] - '0';
            printf("Number representation: %d\t Position: %zu\n", num_representation, i);
        }
        std::printf("Weight of %c is: %d\n", vin[i], weight);
        vin_sum += num_representation * weight; 
    }
    const int nearest_smallest_number = (vin_sum / 11) * 11;
    const int check_sum = vin_sum - nearest_smallest_number; 
    printf("Nearest smallest number: %d\n", nearest_smallest_number);
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

[[nodiscard]] inline VIN::GEOGRAPHICAL_ZONES VIN::defineGeographicalZone(const char symbol)
{
    // First symbol of VIN is geo.zone id
    switch(symbol) {
    case 'A'...'H':
        return VIN::GEOGRAPHICAL_ZONES::AFRICA;
    case 'J'...'R':
        return VIN::GEOGRAPHICAL_ZONES::ASIA;
    case 'S'...'Z':
        return VIN::GEOGRAPHICAL_ZONES::EUROPE;
    case '1'...'5':
        return VIN::GEOGRAPHICAL_ZONES::NORTH_AMERIKA;
    case '6'...'7':
        return VIN::GEOGRAPHICAL_ZONES::OCEANIA;
    case '8'...'9':
        return VIN::GEOGRAPHICAL_ZONES::SOUTH_AMERIKA;
    }
    [[unlikely]] return VIN::GEOGRAPHICAL_ZONES::NONE;
}

[[nodiscard]] string VIN::defineCountry(const string &region_code)
{
    const GEOGRAPHICAL_ZONES zone = defineGeographicalZone(region_code[0]);
    string country_name;
    switch(zone) {
    case AFRICA:
        return defineAfricaCountry(region_code); 
    case ASIA:
        return defineAsianCountry(region_code);
    case EUROPE:
        return defineEuropeCountry(region_code);
    case NORTH_AMERIKA:
        return defineEuropeCountry(region_code);
    case OCEANIA:
        return defineOceaniaCountry(region_code);
    case SOUTH_AMERIKA:
        return defineSouthAmerikaCountry(region_code);
    [[unlikely]] case NONE:
        return "None";
    }
}

[[nodiscard]] inline string VIN::defineAsianCountry(const string &region_code)
{
}

[[nodiscard]] inline string VIN::defineAfricaCountry(const string &region_code)
{
    
    const char zone_code = region_code[0];
    const char country_code = region_code[1];

    switch(zone_code) {
    case 'A':
        switch(country_code) {
        case 'A'...'H':
            return africa_country_names[UAR];
        case 'J'...'N':
            return africa_country_names[COTE_D_IVOIRE];
        default:
            return "Not used";  
        }
    case 'B':
        switch(country_code) {
        case 'A'...'E':
            return africa_country_names[ANGOLA];
        case 'F'...'K':
            return africa_country_names[KENIA];
        case 'L'...'R':
            return africa_country_names[TANZANIA];
        default:
            return "Not used";
        }
    case 'C':
        switch(country_code) {
        case 'A'...'E':
            return africa_country_names[BENIN];
        case 'F'...'K':
            return africa_country_names[MADAGASCAR];
        case 'L'...'R':
            return africa_country_names[TUNISIA];
        default:
            return "Not used";
        }
    case 'D':
        switch(country_code) {
        case 'A'...'E':
            return africa_country_names[EGYPT];
        case 'F'...'K':
            return africa_country_names[MOROCCO];
        case 'L'...'R':
            return africa_country_names[ZAMBI];
        default:
            return "Not used";
        }
    case 'E':
//        return VIN::GEOGRAPHICAL_ZONES::OCEANIA;
    case '8'...'9':
//        return VIN::GEOGRAPHICAL_ZONES::SOUTH_AMERIKA;
    }
}

[[nodiscard]] inline string VIN::defineEuropeCountry(const string &region_code)
{

}

[[nodiscard]] inline string VIN::defineNorthAmerikaCountry(const string &region_code)
{

}

[[nodiscard]] inline string VIN::defineSouthAmerikaCountry(const string &region_code)
{

}

[[nodiscard]] inline string VIN::defineOceaniaCountry(const string &region_code)
{

}
