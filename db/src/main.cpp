#include <iostream>
#include <locale>
#include "vin.hpp"
#include "reg_mark.hpp"

int main (int argc, char *argv[]) 
{
    const std::string vin = "19VDE1F75FE004339";
    std::setlocale(LC_ALL, "");
    if (VIN::checkVIN(vin)) {
        std::cout << "Success\n";
        std::cout << VIN::getVINCountry(vin) << '\n';
        std::cout << "Year of vehicle: " << VIN::getTransportYear(vin) << '\n';
    }
    else
        std::cout << "Failure\n";

    const std::string mark = "A999AA100";
    if (RegMark::CheckMark(mark)) {
        std::cout << RegMark::GetNextMarkAfter(mark) << '\n';
        std::cout << RegMark::GetNextMarkAfterRange(mark, "A998AA100", "B999BB100") << '\n';
        std::cout << "Success\n";
    } else {
        std::cout << "Failure\n";
    }
    return 0;
}
