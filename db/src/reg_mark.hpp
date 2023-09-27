#pragma once
#include <string>

namespace RegMark {
    bool CheckMark(const std::string &mark);
    std::string GetNextMarkAfter(const std::string &mark);
    std::string GetNextMarkAfterRange(const std::string &prevMark, const std::string &rangeMark, const std::string &rangeEnd);
    int GetCombinationCountInRange(const std::string &firstMark, const std::string &secondMark);
}
