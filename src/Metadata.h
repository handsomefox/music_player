#pragma once

#include <QMediaMetaData>
#include <QImage>
#include <string>

struct Metadata {
  Metadata() = default;
  explicit Metadata(const QMediaMetaData &metadata) {
    Artist = metadata.value(QMediaMetaData::AlbumArtist).toString().toStdWString();
    Title = metadata.value(QMediaMetaData::Title).toString().toStdWString();
    Milliseconds = metadata.value(QMediaMetaData::Duration).toInt();
    Seconds = (Milliseconds / 1000) % 60;
    Minutes = ((Milliseconds / 1000) - Seconds) / 60;
    Cover = metadata.value(QMediaMetaData::ThumbnailImage).value<QImage>();
  }
  std::wstring Artist;
  std::wstring Title;
  int Milliseconds{};
  int Seconds{};
  int Minutes{};
  QImage Cover;
};
