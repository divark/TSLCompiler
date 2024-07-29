#pragma once

#include <string>
#include <memory>

enum ExpType {
    Simple,
    Negated,
    Grouped,
    And,
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

        Expression(std::string);
        Expression(ExpType, std::shared_ptr<Expression>);
        Expression(ExpType, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
};
