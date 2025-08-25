#include "expressions.hpp"
#include <unordered_set>

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
        case ExpType::Or:
            return leftExpression->asString() + " || " + rightExpression->asString();
        default:
            return property;
    }
}

/**
* Returns true if all properties from the Expression were found in the
* symbol table, or false otherwise.
*/
bool Expression::evaluate(const std::unordered_set<std::string>& properties) {
    bool returnedEvaluation = false;
    switch (expressionType) {
        case ExpType::Negated:
            returnedEvaluation = !leftExpression->evaluate(properties);
            break;
        case ExpType::Grouped:
            returnedEvaluation = leftExpression->evaluate(properties);
            break;
        case ExpType::And:
            returnedEvaluation =  leftExpression->evaluate(properties) && rightExpression->evaluate(properties);
            break;
        case ExpType::Or:
            returnedEvaluation = leftExpression->evaluate(properties) || rightExpression->evaluate(properties);
            break;
        default:
            returnedEvaluation = properties.contains(property);
            break;
    }

    return returnedEvaluation;
}
