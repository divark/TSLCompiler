#include "tsl_grammar.hpp"

Category::Category() {
    label = "";
}

Category::Category(std::string categoryLabel) {
    label = categoryLabel;
}

std::string Category::getLabel() const {
    return label;
}

Choice& Category::getChoice(size_t choiceNum) {
    return choices[choiceNum];
}

Choice& Category::getRecentChoice() {
    auto choiceNum = choices.size() - 1;
    return getChoice(choiceNum);
}

size_t Category::getNumChoices() const {
    return choices.size();
}

void Category::addChoice(Choice newChoice) {
    choices.push_back(newChoice);
}

Choice::Choice() {
    label = "";
}

Choice::Choice(std::string choiceLabel) {
    label = choiceLabel;
}

std::string Choice::getLabel() const {
    return label;
}

Property& Choice::getProperty(size_t propertyNum) {
    return normalProperties[propertyNum];
}

Property& Choice::getRecentProperty() {
    auto numProperties = normalProperties.size() - 1;
    return getProperty(numProperties - 1);
}

size_t Choice::getNumProperties() const {
   return normalProperties.size();
}

std::string Choice::getMarker() {
    auto foundMarker = getRecentProperty().asMarker();
    if (foundMarker == Marker::Single) {
        return "single";
    }

    return "error";
}

std::optional<std::shared_ptr<Expression>> Choice::getExpression() {
    return expression;
}

void Choice::addProperty(Property newProperty) {
    normalProperties.push_back(newProperty);
}

void Choice::setMarker(Marker newMarker) {
    auto markerProperty = Property(newMarker);
    addProperty(markerProperty);
}

void Choice::setExpression(std::shared_ptr<Expression> newExpression) {
    expression = std::make_optional<std::shared_ptr<Expression>>(newExpression);
}

Property::Property() {
    marker = Marker::None;
}

Property::Property(Marker newMarker) {
    marker = newMarker;
}

Property::Property(std::string variableDefined) {
    this->variableDefined = variableDefined;
}

std::optional<Marker> Property::asMarker() {
    if (marker == Marker::None) {
        return {};
    }

    return std::make_optional<Marker>(marker);
}

std::string Property::asString() {
    return variableDefined;
}
