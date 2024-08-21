#include "filetracker.hpp"

FileTracker::FileTracker(const std::filesystem::path &newFilePath) {
    filePath = newFilePath;
    lineNumber = 1;
    lineColumn = 1;
}

/**
 * Returns the new line number count after the shift
 */
size_t FileTracker::shiftToNewline(size_t newlineNum) {
    lineNumber = newlineNum;
    lineColumn = 1;

    return lineNumber;
}

/**
 * Returns the new line column count after shifting
 * tokenLength times.
 */
size_t FileTracker::shiftLineColumn(size_t tokenLength) {
    lineColumn += tokenLength;

    return lineColumn;
}

/**
 * Returns the lineNumber recorded so far.
 */
size_t FileTracker::getLineNumber() {
    return lineNumber;
}

/**
 * Returns the lineColumn recorded so far.
 */
size_t FileTracker::getLineColumn() {
    return lineColumn;
}
