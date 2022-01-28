#include "DirectoryParser.h"

#include "pch.h"

const std::vector<File> &DirectoryParser::getFiles() const {
  return m_files;
}
void DirectoryParser::changeDirectory(const std::filesystem::path &directory) {
  m_directory = directory;
  rescan();
}
void DirectoryParser::rescan() {
  m_files.clear();
  parse();
}
void DirectoryParser::parse() {
  using it = std::filesystem::recursive_directory_iterator;

  if (!exists(m_directory))
    return;

  if (m_directory.empty())
    return;

  for (const auto &entry : it(m_directory)) {
    auto &path = entry.path();
    if (isMusicFile(path.extension()))
      m_files.emplace_back(path.wstring());
  }
}
bool DirectoryParser::isMusicFile(const std::wstring &extension) {
  if (extension == L".mp3" || extension == L".ogg" || extension == L".flac" || extension == L".wav"
      || extension == L".aiff" || extension == L".acc" || extension == L".alac" || extension == L".oga")
    return true;
  return false;
}
