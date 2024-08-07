#pragma once

#include <vector>
#include <string>
#include <memory>

#include "expressions.hpp"

struct TSLCollector {
    std::vector<std::string> categories;
    std::vector<std::string> choices;
    // All variables below are Constraints for Choices
    std::vector<bool> singleMarkings;
    std::vector<bool> errorMarkings;
    std::vector<std::string> properties;

    /// These mimic an Adjacency List approach to a Graph
    /// representation, instead using a vector. This is intentional since
    /// the graph will never have nodes or edges deleted during runtime.
    ///
    /// In addition, it's a graph of indexes based on the contents
    /// of the nodes, where each node's contents are stored in the
    /// vectors above. This saves memory over a hashtable approach
    /// where a key is based on its string, and has the potential to
    /// utilize temporal locality if dealing with the graph alone.
    std::vector<std::vector<int>> categoryChoicesGraph;
    std::vector<std::vector<int>> choiceProperties;

    /// Properties and Markings can be defined from an if-else statement,
    /// so we have to differentiate between this and when a property or marking
    /// is defined without an if statement.
    std::vector<std::vector<int>> choiceIfProperties;
    std::vector<bool> singleIfMarkings;
    std::vector<bool> errorIfMarkings;

    std::vector<bool> choiceHasElseStatement;

    /// Normally, a Choice should have just one expression, but because
    /// we are processing Expressions one at a time, we could have many
    /// while the Parser is running. The goal is that each Choice consolidates
    /// into just one Expression, and having more than one indicates an error.
    std::vector<std::vector<std::shared_ptr<Expression>>> choiceExpressions;

    int recordSimpleExpression(std::string);
    int recordUnaryExpression(ExpType);
    int recordBinaryExpression(ExpType);
    std::shared_ptr<Expression> getChoiceExpression(unsigned int);

    int recordCategory(std::string);
    int recordChoice(std::string);

    int recordProperty(std::string);
    int convertPropertiesToIfProperties();

    int markChoiceAsSingle();
    int markChoiceAsError();

    int markChoiceHasElse();
    bool hasElseExpression(unsigned int);
};
