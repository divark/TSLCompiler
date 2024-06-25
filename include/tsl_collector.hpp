#pragma once

#include <vector>
#include <string>

struct TSLCollector {
    std::vector<std::string> categories;

    int recordCategory(std::string);
};
