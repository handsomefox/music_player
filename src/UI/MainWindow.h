#pragma once

#include <QMainWindow>

#include "pch.h"

#include "Player.h"
#include "DirectoryParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  void onDoubleClicked(const QModelIndex &index);
  void onPositionChanged(quint64 position);

private slots:
  void on_actionOpen_directory_triggered();
  void on_previous_button_clicked();
  void on_play_pause_button_clicked();
  void on_next_button_clicked();
  void on_shuffle_button_clicked();
  void on_repeat_button_clicked();
  void on_actionEnable_dark_mode_triggered();
  void on_actionExit_triggered();
  void on_actionShuffle_triggered();
  void on_actionRepeat_triggered();


private:

  void setupUI();
  static QString getFormattedLength(int minutes, int seconds);
  void setImage(const QImage &image);
  void updateProgress(int max);
  void useFiles();
  void mediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
  QPixmap *m_placeholder = nullptr;
  Ui::MainWindow *ui;

  DirectoryParser *m_parser = nullptr;
  Player *m_player = nullptr;

  std::filesystem::path m_current_folder;
  std::vector<File> m_current_songs_list;
};
