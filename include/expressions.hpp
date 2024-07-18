#pragma once

#include <string>
#include <memory>

/**
 * There can be many types of Expressions, but all
 * of them have these aspects in common.
 */
class Expression {
    private:
        std::string property;
    public:
        std::string asString();

        Expression(std::string);
};

