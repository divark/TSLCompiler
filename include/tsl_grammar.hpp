#pragma once

#include "expressions.hpp"

#include <string>
#include <unordered_set>
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

enum EvaluationType {
    If,
    Else,
};

class EvaluatedProperties {
    private:
        EvaluationType evaluationType;
        std::vector<Property> properties;
    public:
        EvaluatedProperties(EvaluationType, std::vector<Property>);

        EvaluationType getType();
        bool containsProperty(const std::string&);
        bool containsMarker(Marker&);

        bool is_empty() const;
};

class Choice {
    private:
        std::string label;

        std::vector<Property> normalProperties;

        std::optional<std::shared_ptr<Expression>> expression;
        std::optional<std::vector<Property>> ifProperties;
        std::optional<std::vector<Property>> elseProperties;
    public:
        Choice();
        Choice(std::string);

        std::string getLabel() const;
        std::optional<std::string> getMarker();
        bool hasNormalMarker();
        bool hasConditionalMarker();
        Property& getProperty(size_t);
        Property& getRecentProperty();
        std::optional<EvaluatedProperties> getEvaluatedProperties(const std::unordered_set<std::string>&);
        size_t getNumProperties() const;
        std::optional<std::shared_ptr<Expression>> getExpression();

        void addProperty(Property);
        void setMarker(Marker);
        void setExpression(std::shared_ptr<Expression>);
        void movePropertiesToIfProperties();
        void movePropertiesToElseProperties();
        void markHavingElse();
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
