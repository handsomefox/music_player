#include "pch.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>

#include "./ui_MainWindow.h"
#include "MainWindow.h"

#include "DirectoryParser.h"
#include "Metadata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setupUI();

  const auto &qt_player_ref = m_player.getQtPlayer();

  connect(&qt_player_ref, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::mediaStatusChanged);
  connect(&qt_player_ref, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
  connect(ui->directory_listview, &QListView::doubleClicked, this, &MainWindow::onDoubleClicked);
}

MainWindow::~MainWindow() {
  delete m_placeholder;
  delete ui;
}

void MainWindow::on_actionOpen_directory_triggered() {
  auto folder_path = QFileDialog::getExistingDirectory((this), "Open folder with music");

  if (folder_path.isEmpty()) {
    // QMessageBox::warning(this, "Warning", "Couldn't open folder");
    return;
  }
  m_player.setDirectory(folder_path.toStdWString());
  useFiles();
}

void MainWindow::onDoubleClicked(const QModelIndex &index) {
  m_player.play(index.row());
}

void MainWindow::onPositionChanged(quint64 position) const {
  const auto seconds = (position / 1000) % 60;
  const auto minutes = ((position / 1000) - seconds) / 60;
  ui->current_progress->setText(getFormattedLength((int) minutes, (int) seconds));
  ui->song_progressbar->setValue((int) position);
}

void MainWindow::setupUI() {
  m_placeholder = new QPixmap();
  const auto placeholder_path = QGuiApplication::applicationDirPath() + "/res/cd-drive.png";
  m_placeholder->load(placeholder_path);

  const auto w = ui->album_cover_label->width();
  const auto h = ui->album_cover_label->height();

  ui->album_cover_label->setPixmap(m_placeholder->scaled(w, h, Qt::KeepAspectRatio));
  ui->album_cover_label->setAlignment(Qt::AlignCenter);

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

  const auto str = oss.str();
  const auto str2 = oss_second.str();

  return QString::fromStdString(str + ':' + str2);
}

void MainWindow::setImage(const QImage &image) {
  auto pixmap = QPixmap::fromImage(image);

  if (!pixmap.isNull()) {
    const auto w = ui->album_cover_label->width();
    const auto h = ui->album_cover_label->height();

    ui->album_cover_label->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
  } else {
    const auto w = ui->album_cover_label->width();
    const auto h = ui->album_cover_label->height();

    ui->album_cover_label->setPixmap(m_placeholder->scaled(w, h, Qt::KeepAspectRatio));
  }
}

void MainWindow::updateProgress(int max) {
  ui->song_progressbar->setMinimum(0);
  ui->song_progressbar->setMaximum(max);
}

void MainWindow::useFiles() {
  auto player_files = m_player.getFiles();

  QList<QString> vector;
  vector.reserve((qsizetype) player_files.size());

  for (const auto &file : player_files) {
    vector.push_back(file.filename());
  }

  QStringList list = QStringList::fromVector(vector);
  auto model = new QStringListModel();
  model->setStringList(list);
  ui->directory_listview->setModel(model);
}

void MainWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
  switch (status) {
  case QMediaPlayer::LoadedMedia: {
    auto metadata = Metadata(m_player.getQtPlayer().metaData());
    setImage(metadata.Cover);
    updateProgress(metadata.Milliseconds);
    ui->artist_label->setText(metadata.Artist);
    ui->title_label->setText(metadata.Title);
    ui->song_length->setText(getFormattedLength(metadata.Minutes, metadata.Seconds));
    break;
  }
  case QMediaPlayer::EndOfMedia: {
    m_player.next();
    break;
  }
  default: break;
  }
}

void MainWindow::on_previous_button_clicked() {
  m_player.previous();
}

void MainWindow::on_play_pause_button_clicked() {
  m_player.togglePlay();
}

void MainWindow::on_next_button_clicked() {
  m_player.next();
}

void MainWindow::on_shuffle_button_clicked() {
  m_player.toggleShuffle();
  ui->actionShuffle->setChecked(m_player.getShuffle());
}

void MainWindow::on_repeat_button_clicked() {
  m_player.toggleRepeat();
  ui->actionRepeat->setChecked(m_player.getRepeat());
}

void MainWindow::loadThemeFromFile(const QString &path) {
  QFile file(path);
  if (file.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
    file.close();
  }
}

void MainWindow::on_actionEnable_dark_mode_triggered() const {
  if (ui->actionEnable_dark_mode->isChecked()) {
    loadThemeFromFile(":/dark-purple/stylesheet.qss");
    ui->actionEnable_dark_mode->setChecked(true);
  } else {
    loadThemeFromFile(":/light-purple/stylesheet.qss");
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
