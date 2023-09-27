#include <iostream>
#include <locale>
#include "vin.hpp"

int main (int argc, char *argv[]) 
{
    const std::string vin = "JH4DC4460SS000830";
    std::setlocale(LC_ALL, "");
    if (VIN::checkVIN(vin)) {
        std::cout << "Success\n";
        std::cout << VIN::getVINCountry(vin) << '\n';
    }
    else
        std::cout << "Failure\n";
    return 0;
}
