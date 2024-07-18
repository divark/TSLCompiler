#include "expressions.hpp"

/**
 * Constructs a Simple Expression, only holding
 * the value of some property.
 */
Expression::Expression(std::string foundProperty) {
    property = foundProperty;
}

/**
 * Returns the Expression as a String from the included
 * Property.
 */
std::string Expression::asString() {
    return property;
}
