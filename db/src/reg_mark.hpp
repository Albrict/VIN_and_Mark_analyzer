#pragma once
#include <string>

namespace RegMark {
    [[nodiscard]] bool CheckMark(const std::string &mark);
    [[nodiscard]] std::string GetNextMarkAfter(const std::string &mark);
    [[nodiscard]] std::string GetNextMarkAfterRange(const std::string &prevMark, const std::string &rangeMark, const std::string &rangeEnd);
    [[nodiscard]] int GetCombinationCountInRange(const std::string &firstMark, const std::string &secondMark);
}
