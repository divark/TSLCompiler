#include "parse_tree.hpp"

Category::Category(std::string categoryContents, std::unique_ptr<Choices> foundChoices) {
    label = categoryContents;
    choices = std::move(foundChoices);
}

Choices::Choices(std::unique_ptr<Choice> initialChoice) {
    choices.push_back(std::move(initialChoice));
}

/**
 * Returns the position assigned to the newly recorded Choice.
 */
int Choices::push(std::unique_ptr<Choice> newChoice) {
    choices.push_back(std::move(newChoice));

    return choices.size() - 1;
}

Choice::Choice(std::string choiceContents) {
    label = choiceContents;
}
