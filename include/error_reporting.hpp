#pragma once

#include "location.hh"
#include "tsl_collector.hpp"

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

void checkIfCurrentPropertyRedefined(const std::string, const TSLCollector&, const yy::location&);
void checkIfCurrentPropertyUndefined(const std::string, const TSLCollector&, const yy::location&);
