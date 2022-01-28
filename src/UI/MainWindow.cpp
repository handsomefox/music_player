#include "pch.h"

#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "DirectoryParser.h"
#include "Metadata.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_parser(new DirectoryParser) {
  ui->setupUi(this);
  setupUI();

  m_player = new Player();
  connect(m_player->getQtPlayer(), &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
  connect(ui->directory_listview, &QListView::doubleClicked, this, &MainWindow::onDoubleClicked);
}
MainWindow::~MainWindow() {
  delete ui;
  delete m_player;
  delete m_parser;
  delete m_placeholder;
}
void MainWindow::on_actionOpen_directory_triggered() {
  m_current_songs_list.clear();
  QString folder_path = QFileDialog::getExistingDirectory((this), "Open folder with music");

  if (folder_path.isEmpty()) {
    QMessageBox::warning(this, "Warning", "Couldn't open folder");
    return;
  }

  m_current_folder = folder_path.toStdWString();
  m_parser->changeDirectory(m_current_folder);
  m_current_songs_list = m_parser->getFiles();
  useFiles();
}
void MainWindow::onDoubleClicked(const QModelIndex &index) {
  m_player->play(index.row());
  connect(m_player->getQtPlayer(), &QMediaPlayer::mediaStatusChanged, this, &MainWindow::mediaStatusChanged);
}
void MainWindow::onPositionChanged(quint64 position) {
  auto Seconds = (position / 1000) % 60;
  auto Minutes = ((position / 1000) - Seconds) / 60;
  ui->current_progress->setText(getFormattedLength(Minutes, Seconds));
  ui->song_progressbar->setValue(position);
}
void MainWindow::setupUI() {
  m_placeholder = new QPixmap(ui->album_cover_label->maximumWidth(), ui->album_cover_label->maximumHeight());
  m_placeholder->load("G:/Placeholder/compact-disc.png");
  ui->album_cover_label->setPixmap(*m_placeholder);
  ui->directory_listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->directory_listview->setAlternatingRowColors(true);
  ui->song_progressbar->setTextVisible(false);
  ui->actionEnable_dark_mode->setChecked(true);
}
QString MainWindow::getFormattedLength(int minutes, int seconds) {
  std::ostringstream oss;
  std::ostringstream oss_second;

  oss << std::setfill('0') << std::setw(2) << minutes;
  oss_second << std::setfill('0') << std::setw(2) << seconds;

  std::string str = oss.str();
  std::string str2 = oss_second.str();

  return QString::fromStdString(str + ':' + str2);
}
void MainWindow::setImage(const QImage &image) {
  auto pixmap = QPixmap::fromImage(image);

  if (!pixmap.isNull()) {
    pixmap.scaled(ui->album_cover_label->maximumWidth(), ui->album_cover_label->maximumHeight(), Qt::KeepAspectRatio);
    ui->album_cover_label->setPixmap(pixmap);
  } else {
    ui->album_cover_label->setPixmap(*m_placeholder);
  }
}
void MainWindow::updateProgress(int max) {
  ui->song_progressbar->setMinimum(0);
  ui->song_progressbar->setMaximum(max);
}
void MainWindow::useFiles() {
  QList<QString> vector;
  vector.reserve((qsizetype) m_current_songs_list.size());

  for (auto &file : m_current_songs_list) {
    vector.push_back(QString::fromStdWString(file.fs_path.filename()));
  }

  QStringList list = QStringList::fromVector(vector);
  auto model = new QStringListModel();
  model->setStringList(list);
  ui->directory_listview->setModel(model);

  m_player->setFiles(m_current_songs_list);
}
void MainWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
  switch (status) {
  case QMediaPlayer::LoadedMedia: {
    auto metadata = Metadata(m_player->getQtPlayer()->metaData());
    setImage(metadata.Cover);
    updateProgress(metadata.Milliseconds);
    ui->artist_label->setText(QString::fromStdWString(metadata.Artist));
    ui->title_label->setText(QString::fromStdWString(metadata.Title));
    ui->song_length->setText(getFormattedLength(metadata.Minutes, metadata.Seconds));
    break;
  }
  case QMediaPlayer::EndOfMedia: {
    m_player->next();
    break;
  }
  default: break;
  }
}
void MainWindow::on_previous_button_clicked() {
  if (m_player == nullptr)
    return;
  m_player->previous();
}
void MainWindow::on_play_pause_button_clicked() {
  if (m_player == nullptr)
    return;
  m_player->togglePlay();
}
void MainWindow::on_next_button_clicked() {
  if (m_player == nullptr)
    return;
  m_player->next();
}
void MainWindow::on_shuffle_button_clicked() {
  if (m_player == nullptr)
    return;
  m_player->toggleShuffle();
  ui->actionShuffle->setChecked(m_player->getShuffle());
}
void MainWindow::on_repeat_button_clicked() {
  if (m_player == nullptr)
    return;
  m_player->toggleRepeat();
  ui->actionRepeat->setChecked(m_player->getRepeat());

}
void MainWindow::on_actionEnable_dark_mode_triggered() {
  if (ui->actionEnable_dark_mode->isChecked()) {
    QFile file(":/dark-purple/stylesheet.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
    file.close();
    ui->actionEnable_dark_mode->setChecked(true);
  } else {
    QFile file(":/light-purple/stylesheet.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
    file.close();
    ui->actionEnable_dark_mode->setChecked(false);
  }
}

void MainWindow::on_actionExit_triggered() {
  QApplication::exit();
}
void MainWindow::on_actionShuffle_triggered() {
  on_shuffle_button_clicked();
}
void MainWindow::on_actionRepeat_triggered() {
  on_repeat_button_clicked();
}
