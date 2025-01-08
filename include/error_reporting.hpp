#pragma once

#include "location.hh"
#include "expressions.hpp"

#include <vector>
#include <string>

class FileReader {
    private:
        std::vector<std::string> inputLines;
    public:
        FileReader(std::string);

        bool hasLine(int);
        std::string getLine(uint);
};

std::string getPointingMsg(const yy::location&);

void reportError(const std::string&, const yy::location&);
void reportUndefinedPropertyError(std::shared_ptr<Expression>, const yy::location&);
