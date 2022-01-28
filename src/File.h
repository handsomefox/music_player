#pragma once

#include "pch.h"

struct File {
  File() = default;
  explicit File(const std::filesystem::path &p) {
    path = p.wstring();
    fs_path = p;
    std::replace(begin(path), end(path), '\\', '/');
  }
  std::wstring path;
  std::filesystem::path fs_path;
};
