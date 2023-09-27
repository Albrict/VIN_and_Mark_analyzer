#include <iostream>
#include <locale>
#include "vin.hpp"

int main (int argc, char *argv[]) 
{
    const std::string vin = "SAJEA01U13HM54253";
    std::setlocale(LC_ALL, "");
    if (VIN::checkVIN(vin)) {
        std::cout << "Success\n";
        std::cout << VIN::getVINCountry(vin) << '\n';
    }
    else
        std::cout << "Failure\n";
    return 0;
}
