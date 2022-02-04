#pragma once

#include "File.h"

class DirectoryParser {
public:
  DirectoryParser() = delete;
  static std::vector<File> scanDirectory(const std::filesystem::path &directory);

private:
  static bool isMusicFile(const std::wstring &extension);
};
