#pragma once

#include <QMediaMetaData>
#include <QImage>
#include <string>

struct Metadata {
  Metadata() = default;
  explicit Metadata(const QMediaMetaData &metadata) {
    Artist = metadata.value(QMediaMetaData::AlbumArtist).toString();
    Title = metadata.value(QMediaMetaData::Title).toString();
    Milliseconds = metadata.value(QMediaMetaData::Duration).toInt();
    Seconds = (Milliseconds / 1000) % 60;
    Minutes = ((Milliseconds / 1000) - Seconds) / 60;
    Cover = metadata.value(QMediaMetaData::ThumbnailImage).value<QImage>();
  }
  QString Artist;
  QString Title;
  int Milliseconds{};
  int Seconds{};
  int Minutes{};
  QImage Cover;
};
