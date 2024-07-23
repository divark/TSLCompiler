#pragma once

#include <string>
#include <memory>

enum ExpType {
    Simple,
    Negated,
    Grouped,
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
    public:
        std::string asString();

        Expression(std::string);
        Expression(ExpType, std::shared_ptr<Expression>);
};

