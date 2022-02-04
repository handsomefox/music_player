#pragma once

#include "pch.h"
#include <QString>

struct File {
  File() = delete;

  explicit File(std::filesystem::path p) : fs_path(std::move(p)) {
  }

  [[nodiscard]] QString filename() const {
    return QString::fromStdWString(fs_path.filename().wstring());
  }

  [[nodiscard]] QString path() const {
    QString path = QString::fromStdWString(fs_path.wstring());
    std::ranges::replace(path, '\\', '/');// Replace backslash to forward slash because Qt doesn't recognize \ in path
    return path;
  }

private:
  std::filesystem::path fs_path;
};
