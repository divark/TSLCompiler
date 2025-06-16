#pragma once

#include "tsl_grammar.hpp"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class TSLTestCase {
    private:
        size_t testCaseNumber;
        std::optional<Marker> markerCase;
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
        bool hasChoiceDependency(const std::string&);

        bool hasMarker() const;
        void setMarker(Marker);
        Marker getMarker() const;
};
