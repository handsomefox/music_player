#include "Player.h"
#include "DirectoryParser.h"

Player::Player()
    : m_player(new QMediaPlayer), m_output(new QAudioOutput) {
  m_player->setAudioOutput(m_output);
  m_output->setVolume(25);
}

Player::~Player() {
  delete m_player;
}
void Player::setFiles(const std::vector<File> &vec) {
  m_files = vec;
}
void Player::togglePlay() {
  if (m_player == nullptr)
    return;

  if (m_player_state == State::Playing) {
    m_player->pause();
    m_player_state = State::Paused;
    return;
  }

  if (m_player_state != State::Paused) {
    m_player->setSource(QUrl::fromLocalFile(QString::fromStdWString(m_files[m_current_song_id].path)));
  }
  m_player->play();
  m_player_state = State::Playing;
}
void Player::play(size_t index) {
  if (m_player == nullptr)
    return;

  m_player->setSource(QString::fromStdWString(m_files[index].path));
  m_player->play();
  std::cout << m_player->duration() << '\n';
  m_player_state = State::Playing;
  m_current_song_id = index;
}
void Player::toggleShuffle() {
  m_shuffle = !m_shuffle;
}
void Player::toggleRepeat() {
  m_repeat = !m_repeat;
}
bool Player::getShuffle() const {
  return m_shuffle;
}
bool Player::getRepeat() const {
  return m_repeat;
}
QMediaPlayer *Player::getQtPlayer() {
  return m_player;
}
void Player::shuffle() {
  std::random_device device;
  std::mt19937 rng(device());
  std::uniform_int_distribution<std::mt19937::result_type> distribution(0, m_files.size() - 1);

  auto random_value = distribution(rng);

  while (m_current_song_id == random_value) {
    random_value = distribution(rng);
  }

  m_current_song_id = random_value;
}
void Player::nextSong() {
  if (m_repeat) {
    return;
  }
  if (m_shuffle) {
    shuffle();
    return;
  }

  ++m_current_song_id;
  if (m_current_song_id >= m_files.size()) {
    m_current_song_id = 0;
  }
}
void Player::previousSong() {
  if (m_repeat) {
    return;
  }
  if (m_shuffle) {
    shuffle();
    return;
  }

  --m_current_song_id;
  if (m_current_song_id <= 0) {
    m_current_song_id = m_files.size() - 1;
  }
}
void Player::next() {
  if (m_player == nullptr)
    return;
  nextSong();
  m_player->setSource(QUrl::fromLocalFile(QString::fromStdWString(m_files[m_current_song_id].path)));
  m_player->play();
  m_player_state = State::Playing;
}
void Player::previous() {
  if (m_player == nullptr)
    return;
  previousSong();
  m_player->setSource(QUrl::fromLocalFile(QString::fromStdWString(m_files[m_current_song_id].path)));
  m_player->play();
  m_player_state = State::Playing;
}
