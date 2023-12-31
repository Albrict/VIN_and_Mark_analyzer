//Library implementation for russian vehicle marks analysis
//To understand more, check this link:
// https://ru.wikipedia.org/wiki/%D0%A0%D0%B5%D0%B3%D0%B8%D1%81%D1%82%D1%80%D0%B0%D1%86%D0%B8%D0%BE%D0%BD%D0%BD%D1%8B%D0%B5_%D0%B7%D0%BD%D0%B0%D0%BA%D0%B8_%D1%82%D1%80%D0%B0%D0%BD%D1%81%D0%BF%D0%BE%D1%80%D1%82%D0%BD%D1%8B%D1%85_%D1%81%D1%80%D0%B5%D0%B4%D1%81%D1%82%D0%B2_%D0%B2_%D0%A0%D0%BE%D1%81%D1%81%D0%B8%D0%B8
#include "reg_mark.hpp"
#include <array>
#include <iostream>
#include <vector>

using string = std::string;
using size_t = std::size_t;

namespace RegMark {
    enum class MarkCompareResult {
        MARK_IS_LESSER,
        MARK_IS_BIGGER,
        NONE
    };
    const size_t max_unique_codes = 30;
    const std::array<const unsigned int, max_unique_codes> unique_region_codes = {102, 111, 113, 116, 121, 123, 124, 159, 125, 126,
                                                                             134, 136, 138, 142, 150, 190, 750, 152, 161, 163,
                                                                             164, 196, 173, 174, 177, 197, 199, 777, 178, 186};
    const string illegal_symbols = "DFGIJLNQRSUVWYZ";
    const char *mark_size_error = "Error! Invalid size of mark\n";
    const char *illegal_symbols_error = "Error! Mark contains illegal symbols!\n";
    const char *illegal_latin_symbols_error = "Error! Mark contains illegal latin symbols!\n";
    const char *invalid_mark_digits = "Error! Invalid mark: registration number/regiod code is not properly set\n";
    const char *invalid_mark_chars = "Error! Invalid mark: series not properly set\n";
    
    const size_t mark_size = 9;
    
    [[nodiscard]] MarkCompareResult compareMarks(const string left_mark, const string right_mark);
    [[nodiscard]] inline bool isSymbolLegal(const char symbol);
    [[nodiscard]] inline unsigned int getRegionCode(const string &mark);
    [[nodiscard]] inline unsigned int getPossibleCombinationsOfSymbols(const char range_start, const char range_end); 
    [[nodiscard]] inline unsigned int getPossibleCombinationsOfDigits(const unsigned int range_start, const unsigned int range_end);
    [[nodiscard]] inline char getNextSymbolInSeries(char symbol);
    [[nodiscard]] string getNextSeries(const string &mark);
    [[nodiscard]] bool checkForIllegalCharacters(const string &mark);
    [[nodiscard]] inline bool checkRegionCode(const string &mark);
};

[[nodiscard]] bool RegMark::CheckMark(const string &mark)
{
    // Check size of mark
    if (mark.size() != mark_size) {
        std::cerr << mark_size_error;
        return false;
    }
    // Check for correctness
    if (!checkForIllegalCharacters(mark))
        return false;
    if (!checkRegionCode(mark))
        return false;
    return true;
}

// First the algorithm checks if the series has the maximum number, 
// if so, it creates the next one, otherwise it simply increments by one
[[nodiscard]] string RegMark::GetNextMarkAfter(const string &mark)
{
    const unsigned int reg_number = std::stoi(mark.substr(1, 3)); 
    string mark_after = mark;
    if (reg_number + 1 == 1000) {
        // Create a new series
        const char *reg_number_str = "001";
        const string next_series = getNextSeries(mark);   
        mark_after.replace(1, 3, reg_number_str);
        mark_after[0] = next_series[0];
        mark_after.replace(4, 2, next_series.substr(1, 2));
        return mark_after;
    } else {
        string reg_number_str;
        if (reg_number <= 98) {
            reg_number_str = std::to_string(reg_number + 1); 
            reg_number_str.insert(0, "0");
        } else {
            reg_number_str = std::to_string(reg_number + 1);
        }
        mark_after.replace(1, 3, reg_number_str);
        return mark_after;
    }
}
// The algorithm checks first checks the bounds and then increments the mark in the given range
[[nodiscard]] string RegMark::GetNextMarkAfterRange(const string &prevMark, const string &rangeStart, const string &rangeEnd)
{
    const string out_of_stock = "Out of stock";
    if (prevMark == rangeStart || prevMark == rangeEnd)
        return prevMark;
    else if (compareMarks(prevMark, rangeEnd) == MarkCompareResult::MARK_IS_BIGGER)
        return out_of_stock; 
    else if (compareMarks(prevMark, rangeStart) == MarkCompareResult::MARK_IS_LESSER)
        return out_of_stock;
    else {
        return GetNextMarkAfter(prevMark);   
    }
}

// All Latin characters that have no representation in Cyrillic are considered illegal 
[[nodiscard]] inline bool RegMark::isSymbolLegal(const char symbol)
{
    for (const auto &illegal_symbol : illegal_symbols) {
        if (symbol == illegal_symbol)
            return false;
    }
    return true;
}

[[nodiscard]] inline char RegMark::getNextSymbolInSeries(char symbol)
{
    ++symbol;
    if (isSymbolLegal(symbol) == true)
        return symbol;
    else
        return getNextSymbolInSeries(symbol);
}

[[nodiscard]] string RegMark::getNextSeries(const string &mark)
{
    char current_char = 0;
    char first_symbol = mark[0];
    char second_symbol = mark[4];
    char third_symbol = mark[5];
     
    if (third_symbol != 'X')
        third_symbol = getNextSymbolInSeries(third_symbol);
    else if (second_symbol != 'X')
        second_symbol = getNextSymbolInSeries(second_symbol);
    else if (first_symbol != 'X') {
        first_symbol = getNextSymbolInSeries(first_symbol);
    } else {
        first_symbol = 'A';
        second_symbol = 'A';
        third_symbol = 'A';
    }
    const string series = {first_symbol, second_symbol, third_symbol};
    return series;
}

[[nodiscard]] inline unsigned int RegMark::getRegionCode(const string &mark)
{
    // Code is one or two digit
    if (mark[8] == '0') {
        if (mark[6] == '0')
            return mark[7] - '0'; // One digit
        else
            return std::stoi(mark.substr(6, 2)); // Two digit
    // Code is three digit
    } else {
        return std::stoi(mark.substr(6));
    }
}

[[nodiscard]] bool RegMark::checkForIllegalCharacters(const string &mark)
{
    // Check for illegal symbols
    for (const auto symbol : mark) {
        if (!std::isalnum(symbol)) {
            std::cerr << illegal_symbols_error;
            return false;
        }
    }
    if (mark.find_first_of(illegal_symbols) != string::npos) {
        std::cerr << illegal_latin_symbols_error;
        return false;
    }

    // Check for number correctness
    for (size_t i = 1; i < mark.size(); ++i) {
        if (!std::isdigit(mark[i])) {
            std::cerr << invalid_mark_digits;
            return false;
        }
        // Skip character sequence
        if (i == 3)
            i += 2;
    }
    // Check for character correctness
    for (size_t i = 0; i < mark.size() - 3; ++i) {
        // Skip digit sequence
        if (i == 1)
            i += 3;
        if (!std::isalpha(mark[i])) {
            std::cerr << invalid_mark_chars;
            return false;
        }
    }
    return true;
}

[[nodiscard]] inline bool RegMark::checkRegionCode(const string &mark)
{
    const unsigned int region_code = getRegionCode(mark);
    if (region_code >= 1 && region_code <= 99) {
        return true;
    } else {
        for (const auto unique_code : unique_region_codes) {
            if (region_code == unique_code)
                return true;
        }
    }
    return false;
}

[[nodiscard]] RegMark::MarkCompareResult RegMark::compareMarks(const string left_mark, const string right_mark)
{
    const unsigned int left_reg_code = std::stoi(left_mark.substr(1, 3));
    const unsigned int right_reg_code = std::stoi(right_mark.substr(1, 3)); 

    if (left_mark[0] < right_mark[0] || left_mark[4] < right_mark[4] || left_mark[5] < right_mark[5]) {
        return MarkCompareResult::MARK_IS_LESSER;
    } else if (left_mark[0] > right_mark[0] || left_mark[4] > right_mark[4] || left_mark[5] > right_mark[5]) {
        return MarkCompareResult::MARK_IS_BIGGER;
    } else if (left_mark[0] == right_mark[0] && left_mark[4] == right_mark[4] && left_mark[5] == right_mark[5]) {
        if (left_reg_code < right_reg_code)
            return MarkCompareResult::MARK_IS_LESSER;
        else 
            return MarkCompareResult::MARK_IS_BIGGER;
    } else [[unlikely]] {
        return MarkCompareResult::NONE;
    }
}
