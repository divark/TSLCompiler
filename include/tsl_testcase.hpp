#pragma once

#include <string>
#include <vector>
#include <map>

class TSLTestCase {
    private:
        size_t testCaseNumber;
        std::vector<std::string> chosenCategories;
        std::map<std::string, std::string> categoryChoices;
    public:
        size_t getTestCaseNumber();
        void setTestCaseNumber(size_t);

        std::vector<std::string>& getCategories();

        std::string getCategoryChoice(const std::string& category);
        void addCategoryChoice(const std::string&, const std::string&);
};
