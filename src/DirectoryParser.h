#pragma once

#include "File.h"

class DirectoryParser {
public:
  DirectoryParser() = default;
  void changeDirectory(const std::filesystem::path &directory);
  [[nodiscard]] const std::vector<File> &getFiles() const;
  void rescan();

private:
  void parse();
  static bool isMusicFile(const std::wstring &extension);

private:
  std::filesystem::path m_directory;
  std::vector<File> m_files;
};
