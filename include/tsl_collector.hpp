#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <optional>

#include "expressions.hpp"
#include "tsl_grammar.hpp"

class TSLCollector {
    private:
        std::vector<Category> categories;
        std::unordered_map<std::string, size_t> propertyDefinedInCategory;

        /// Normally, a Choice should have just one expression, but because
        /// we are processing Expressions one at a time, we could have many
        /// while the Parser is running. The goal is that each Choice consolidates
        /// into just one Expression, and having more than one indicates an error.
        std::vector<std::shared_ptr<Expression>> choiceExpressions;

    public:
        Category& getCategory(size_t);
        size_t getNumCategories() const;

        int recordExpression(std::shared_ptr<Expression>);

        int recordSimpleExpression(std::string);
        int recordUnaryExpression(ExpType);
        int recordBinaryExpression(ExpType);
        bool isExpressionUndefined(std::shared_ptr<Expression>) const;

        int recordCategory(std::string);
        int recordChoice(std::string);

        int recordProperty(std::string);
        int convertPropertiesToIfProperties();
        int convertPropertiesToElseProperties();
        bool hasPropertyDefined(std::string) const;
        std::optional<size_t> getPropertyCategoryIndex(std::string) const;

        int markChoiceAsSingle();
        int markChoiceAsError();

        int markChoiceHasElse();
};

std::string getPropertyWithoutComma(const std::string&);
