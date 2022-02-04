#pragma once

#include <QImage>
#include <QMediaMetaData>
#include <string>

struct Metadata {
  Metadata() = delete;

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
