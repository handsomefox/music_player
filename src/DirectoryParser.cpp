#include "DirectoryParser.h"

#include "pch.h"

std::vector<File> DirectoryParser::scanDirectory(const std::filesystem::path &directory) {
  using it = std::filesystem::recursive_directory_iterator;

  if (!exists(directory))
    return {};

  if (directory.empty())
    return {};

  std::vector<File> files;

  for (const auto &entry : it(directory)) {
    auto &path = entry.path();
    if (isMusicFile(path.extension()))
      files.emplace_back(path.wstring());
  }
  return files;
}

bool DirectoryParser::isMusicFile(const std::wstring &extension) {
  if (extension == L".mp3" || extension == L".ogg" || extension == L".flac" || extension == L".wav"
      || extension == L".aiff" || extension == L".acc" || extension == L".alac" || extension == L".oga")
    return true;
  return false;
}
