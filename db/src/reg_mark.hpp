#pragma once
#include <string>

namespace RegMark {
    //  This function checks the passed license plate number in the format a999aa999 (in Latin caps letters) and 
    //  returns true or false depending on the correctness of the license plate number.
    [[nodiscard]] bool CheckMark(const std::string &mark);
    //  This function accepts a license plate number in the format a999aa999 (in Latin caps letters) and 
    //  outputs the next number in the given series or creates the next series.
    [[nodiscard]] std::string GetNextMarkAfter(const std::string &mark);
    //  This function accepts a license plate number in the format a999aa999 (in Latin caps letters) and 
    //  outputs the next number in the given number rangeStart to rangeEnd (including both boundaries). 
    //  If there is no possibility to output the next number, the "out of stock" message returned.
    [[nodiscard]] std::string GetNextMarkAfterRange(const std::string &prevMark, const std::string &rangeMark, const std::string &rangeEnd);
    //  Not implemented:
    //    [[nodiscard]] int GetCombinationCountInRange(const std::string &firstMark, const std::string &secondMark);
}
