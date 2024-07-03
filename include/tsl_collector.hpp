#pragma once

#include <vector>
#include <string>

struct TSLCollector {
    std::vector<std::string> categories;
    std::vector<std::string> choices;
    // All variables below are Constraints for Choices
    std::vector<bool> singleMarkings;

    /// This mimics an Adjacency List approach to a Graph
    /// representation, instead using a vector. This is intentional since
    /// the graph will never have nodes or edges deleted during runtime.
    ///
    /// In addition, it's a graph of indexes based on the contents
    /// of the nodes, where each node's contents are stored in the
    /// vectors above. This saves memory over a hashtable approach
    /// where a key is based on its string, and has the potential to
    /// utilize temporal locality if dealing with the graph alone.
    std::vector<std::vector<int>> categoryChoicesGraph;

    int recordCategory(std::string);
    int recordChoice(std::string);

    int markChoiceAsSingle();
};
