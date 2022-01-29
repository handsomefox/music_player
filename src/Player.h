#pragma once

#include "File.h"
#include <QMediaPlayer>
#include <QAudioOutput>

class Player {
public:
  enum class State {
    None,
    Playing,
    Paused,
  };

  Player();
  ~Player() override;
  [[nodiscard]] const QMediaPlayer &getQtPlayer() const;
  void setFiles(const std::vector<File> &vec);

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

  std::size_t m_current_song_id = 0;
  State m_player_state = State::None;
  bool m_shuffle = false;
  bool m_repeat = false;

  std::vector<File> m_files;
};
