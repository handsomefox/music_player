#pragma once

#include <QMainWindow>

#include "pch.h"

#include "DirectoryParser.h"
#include "Player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void onDoubleClicked(const QModelIndex &index);
  void onPositionChanged(quint64 position) const;
  void setupUI();
  static QString getFormattedLength(int minutes, int seconds);
  void setImage(const QImage &image);
  void updateProgress(int max);
  void useFiles();
  static void loadThemeFromFile(const QString &path);
  void mediaStatusChanged(QMediaPlayer::MediaStatus status);

private slots:
  void on_actionOpen_directory_triggered();
  void on_previous_button_clicked();
  void on_play_pause_button_clicked();
  void on_next_button_clicked();
  void on_shuffle_button_clicked();
  void on_repeat_button_clicked();
  void on_actionEnable_dark_mode_triggered() const;
  static void on_actionExit_triggered();
  void on_actionShuffle_triggered();
  void on_actionRepeat_triggered();

private:
  QPixmap *m_placeholder = nullptr;// We store the placeholder in memory, so we don't have to keep reloading it if the next song doesn't have an album cover.
  Ui::MainWindow *ui;
  Player m_player{};
};
