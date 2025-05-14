#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class TSLChoice {
    private:
        size_t categoryIdx;
        size_t choiceIdx;

        bool isMarker;
        bool containsConditionalMarker;

        std::string choiceExpression;
    public:
        TSLChoice();
        TSLChoice(size_t, size_t);

        size_t getCategoryIdx() const;
        size_t getChoiceIdx() const;

        bool hasMarker() const;
        bool hasConditionalMarker() const;

        void setChoiceExpression(std::string);
        std::string getChoiceExpression() const;

        void toggleIfMarker(bool);
        void toggleIfConditionalMarkers(bool);
};

class TSLTestCase {
    private:
        size_t testCaseNumber;
        bool isMarkerCase = false;
        std::vector<std::string> chosenCategories;
        std::map<std::string, std::string> categoryChoices;
        std::unordered_map<std::string, std::string> choiceDependencies;
    public:
        size_t getTestCaseNumber();
        void setTestCaseNumber(size_t);

        std::vector<std::string>& getCategories();

        std::string getCategoryChoice(const std::string& category);
        void addCategoryChoice(const std::string&, const std::string&);

        std::string getChoiceDependency(const std::string&);
        void setChoiceDependency(const std::string&, std::string, bool);

        bool hasMarker() const;
        void toggleIsMarker(bool);
};
