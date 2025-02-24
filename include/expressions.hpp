#pragma once

#include <string>
#include <memory>
#include <unordered_set>

enum ExpType {
    Simple,
    Negated,
    Grouped,
    And,
    Or,
};

/**
 * There can be many types of Expressions, but all
 * of them have these aspects in common.
 */
class Expression {
    private:
        ExpType expressionType;

        std::string property;

        std::shared_ptr<Expression> leftExpression;
        std::shared_ptr<Expression> rightExpression;
    public:
        std::string asString();
        bool evaluate(const std::unordered_set<std::string>&);

        Expression(std::string);
        Expression(ExpType, std::shared_ptr<Expression>);
        Expression(ExpType, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
};
