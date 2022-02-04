#pragma once

#include "DirectoryParser.h"
#include "File.h"

#include <QAudioOutput>
#include <QMediaPlayer>

class Player {
public:
  enum class State {
    None,
    Playing,
    Paused,
  };

  Player();
  ~Player();
  [[nodiscard]] const QMediaPlayer &getQtPlayer() const;

  void setDirectory(const std::wstring &path);
  const std::vector<File> &getFiles();

  // Controls
  void togglePlay();
  void play(size_t index);
  void next();
  void previous();

  void toggleShuffle();
  void toggleRepeat();
  [[nodiscard]] bool getShuffle() const;
  [[nodiscard]] bool getRepeat() const;

private:
  void shuffle();
  void nextSong();
  void previousSong();

private:
  QAudioOutput *m_output;
  QMediaPlayer *m_player;

  std::vector<File> m_files;

  std::size_t m_current_song_id = 0;
  State m_player_state = State::None;
  bool m_shuffle = false;
  bool m_repeat = false;
};
