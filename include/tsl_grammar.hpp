#pragma once

#include "expressions.hpp"

#include <string>
#include <vector>
#include <optional>

class Choice;

class Category {
    private:
        std::string label;

        std::vector<Choice> choices;
    public:
        Category();
        Category(std::string);

        std::string getLabel() const;
        Choice& getChoice(size_t);
        Choice& getRecentChoice();
        size_t getNumChoices() const;

        void addChoice(Choice);
};

class Property;
enum Marker {
    None,
    Single,
    Error,
};

class Choice {
    private:
        std::string label;

        std::vector<Property> normalProperties;

        std::optional<std::shared_ptr<Expression>> expression;
        std::vector<Property> ifProperties;
        std::vector<Property> elseProperties;
    public:
        Choice();
        Choice(std::string);

        std::string getLabel() const;
        std::string getMarker();
        Property& getProperty(size_t);
        Property& getRecentProperty();
        size_t getNumProperties() const;
        std::optional<std::shared_ptr<Expression>> getExpression();

        void addProperty(Property);
        void setMarker(Marker);
        void setExpression(std::shared_ptr<Expression>);
};

class Property {
    private:
        Marker marker;
        std::string variableDefined;
    public:
        Property();
        Property(Marker);
        Property(std::string);

        std::optional<Marker> asMarker();
        std::string asString();
};
