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

std::vector<Property>& Choice::getProperties() {
    return normalProperties;
}

Property& Choice::getRecentProperty() {
    auto numProperties = normalProperties.size() - 1;
    return getProperty(numProperties);
}

std::optional<EvaluatedProperties> Choice::getEvaluatedProperties(const std::unordered_set<std::string>& variablesDefined) {
    auto choiceExpression = getExpression();
    if (!choiceExpression) {
        return {};
    }

    std::vector<Property> foundProperties;
    EvaluationType evalType = EvaluationType::If;
    auto expressionSatisfied = choiceExpression.value()->evaluate(variablesDefined);

    if (!expressionSatisfied) {
        if (!elseProperties) {
            return {};
        } else {
            evalType = EvaluationType::Else;
            foundProperties = elseProperties.value();
        }
    } else {
        evalType = EvaluationType::If;
        if (ifProperties) {
            foundProperties = ifProperties.value();
        }
    }

    auto evaluatedProperties = EvaluatedProperties(evalType, foundProperties);
    return evaluatedProperties;
}

size_t Choice::getNumProperties() const {
   return normalProperties.size();
}

std::optional<std::string> Choice::getMarker() {
    if (getNumProperties() == 0) {
        return {};
    }

    auto foundMarker = getRecentProperty().asMarker();
    if (!foundMarker) {
        return {};
    }

    if (foundMarker.value() == Marker::Single) {
        return std::make_optional<std::string>("single");
    }

    return std::make_optional<std::string>("error");
}

bool Choice::hasNormalMarker() {
    auto foundMarker = getMarker();
    return foundMarker.has_value();
}

/// Returns whether a [single] or [error] exists in a if expression for the current
/// choice.
bool Choice::hasIfConditionalMarker() {
    return ifProperties && ifProperties.value().back().asMarker();
}

bool Choice::hasConditionalMarker() {
    bool hasIfMarker = hasIfConditionalMarker();
    bool hasElseMarker = false;
    if (elseProperties) {
        auto lastProperty = elseProperties.value().back().asMarker();
        hasElseMarker = lastProperty.has_value();
    }

    bool hasConditionalMarker = hasIfMarker || hasElseMarker;
    return hasConditionalMarker;
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

void Choice::movePropertiesToIfProperties() {
    std::vector<Property> newIfProperties = normalProperties;
    normalProperties.clear();

    ifProperties = std::make_optional<std::vector<Property>>(newIfProperties);
}

void Choice::movePropertiesToElseProperties() {
    std::vector<Property> newElseProperties = normalProperties;
    normalProperties.clear();

    elseProperties = std::make_optional<std::vector<Property>>(newElseProperties);
}

void Choice::markHavingElse() {
    std::vector<Property> newElseProperties;
    elseProperties = std::make_optional<std::vector<Property>>(newElseProperties);
}

EvaluatedProperties::EvaluatedProperties(EvaluationType evalType, std::vector<Property> evaluatedProperties) {
    evaluationType = evalType;
    properties = evaluatedProperties;
}

EvaluationType EvaluatedProperties::getType() {
    return evaluationType;
}

std::vector<Property>& EvaluatedProperties::getProperties() {
    return properties;
}

bool EvaluatedProperties::containsProperty(const std::string& propertyToFind) {
    bool foundProperty = false;
    for (auto& property : properties) {
        if (property.asString() == propertyToFind) {
            foundProperty = true;
            break;
        }
    }

    return foundProperty;
}

bool EvaluatedProperties::containsMarker(Marker& markerToFind) {
    if (properties.size() != 1) {
        return false;
    }

    auto markerProperty = properties[0].asMarker();
    if (!markerProperty) {
        return false;
    }

    auto markerType = markerProperty.value();
    return markerType == markerToFind;
}

bool EvaluatedProperties::is_empty() const {
    return properties.empty();
}

Property::Property() {
    marker = Marker::None;
}

Property::Property(Marker newMarker) {
    marker = newMarker;
}

Property::Property(std::string variableDefined) {
    marker = Marker::None;
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
