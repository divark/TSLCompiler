#include "expressions.hpp"

/**
 * Constructs a Simple Expression, only holding
 * the value of some property.
 */
Expression::Expression(std::string foundProperty) {
    expressionType = ExpType::Simple;
    property = foundProperty;
}

/**
 * Constructs a Negated Expression holding an existing Expression.
 */
Expression::Expression(ExpType expType, std::shared_ptr<Expression> expression) {
    expressionType = expType;
    leftExpression = expression;
}

/**
 * Returns the Expression as a String from the included
 * Property.
 */
std::string Expression::asString() {
    switch (expressionType) {
        case ExpType::Negated:
            return "!" + leftExpression->asString();
        default:
            return property;
    }
}
