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
 * Constructs an Unary Expression (Either Negated or Grouped) holding an existing Expression.
 * Preconditions: expType is either Negated or Grouped.
 */
Expression::Expression(ExpType expType, std::shared_ptr<Expression> expression) {
    expressionType = expType;
    leftExpression = expression;
}

/**
 * Constructs a Binary Expression (Either AND or OR) holding two Expressions representing
 * the left and right sides.
 * Preconditions: expType is either AND or OR.
 */
Expression::Expression(ExpType expType, std::shared_ptr<Expression> lExpression, std::shared_ptr<Expression> rExpression) {
    expressionType = expType;

    leftExpression = lExpression;
    rightExpression = rExpression;
}

/**
 * Returns the Expression as a String from the included
 * Property.
 */
std::string Expression::asString() {
    switch (expressionType) {
        case ExpType::Negated:
            return "!" + leftExpression->asString();
        case ExpType::Grouped:
            return "(" + leftExpression->asString() + ")";
        case ExpType::And:
            return leftExpression->asString() + " && " + rightExpression->asString();
        default:
            return property;
    }
}
