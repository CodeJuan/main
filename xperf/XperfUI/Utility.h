#ifndef XPERFUI_UTILITY_H
#define XPERFUI_UTILITY_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string& s, char c);
std::vector<std::string> GetFileList(const std::string& pattern);
std::string LoadFileAsText(const std::string& fileName);
void WriteTextAsFile(const std::string& fileName, const std::string& text);

#endif