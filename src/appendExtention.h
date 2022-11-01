#pragma once

#include <QString>

// Appends extention to filename it it doesn't have it
//
// Examples:
// appendExtentionIfNot("file", ".txt") = "file.txt"
// appendExtentionIfNot("file.txt", ".txt") = "file.txt"
void appendExtentionIfNot(QString& filename, const QString& extention);
