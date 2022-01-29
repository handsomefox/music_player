#pragma once

#include "pch.h"
#include <QString>

struct File {
  File() = default;
  explicit File(const std::filesystem::path &p) : fs_path(p) {
    path = QString::fromStdWString(p.wstring());
    std::ranges::replace(path, '\\', '/');
  }
  [[nodiscard]] QString filenameAsQString() const {
    return QString::fromStdWString(fs_path.filename().wstring());
  }
  QString path;
  std::filesystem::path fs_path;
};
