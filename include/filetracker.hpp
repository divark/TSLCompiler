#pragma once

#include <filesystem>
#include <cstddef>

class FileTracker {
    private:
        size_t lineNumber;
        size_t lineColumn;
        std::filesystem::path filePath;
    public:
        FileTracker(const std::filesystem::path&);

        size_t shiftToNewline(size_t);
        size_t shiftLineColumn(size_t);

        size_t getLineNumber();
        size_t getLineColumn();
};

