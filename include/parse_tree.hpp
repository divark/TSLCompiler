#pragma once

#include <string>
#include <memory>
#include <vector>

class Choice {
    private:
        std::string label;

    public:
        Choice(std::string);
};

class Choices {
    private:
        std::vector<std::unique_ptr<Choice>> choices;
    
    public:
        Choices(std::unique_ptr<Choice>);

        int push(std::unique_ptr<Choice>);
};

class Category {
    private:
        std::string label;
        std::unique_ptr<Choices> choices;

    public:
        Category(std::string, std::unique_ptr<Choices>);
};
