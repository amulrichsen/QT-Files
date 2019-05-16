#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QVideoWidget>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QVBoxLayout>
#include <QTimer>
#include <QDir>
#include <QImageWriter>
#include <QWindow>
#include <QScreen>
#include <QTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set window title
    setWindowTitle("HMI Cameras");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // create 4 video widgets
    QVideoWidget *_vw1 = new QVideoWidget;
    QVideoWidget *_vw2 = new QVideoWidget;
    QVideoWidget *_vw3 = new QVideoWidget;
    QVideoWidget *_vw4 = new QVideoWidget;

    // create 4 media players
    QMediaPlayer *_player1 = new QMediaPlayer;
    QMediaPlayer *_player2 = new QMediaPlayer;
    QMediaPlayer *_player3 = new QMediaPlayer;
    QMediaPlayer *_player4 = new QMediaPlayer;


    // create 4x4 QGridLayout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(_vw1,0,0,1,1);
    layout->addWidget(_vw2,0,1,1,1);
    layout->addWidget(_vw3,1,0,1,1);
    layout->addWidget(_vw4,1,1,1,1);

    mainLayout->addLayout(layout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    newScreenshotButton = new QPushButton(tr("New Screenshot"), this);
    connect(newScreenshotButton, &QPushButton::clicked, this, &MainWindow::shootScreen);
    buttonsLayout->addWidget(newScreenshotButton);

    QPushButton *quitScreenshotButton = new QPushButton(tr("Quit"), this);
    quitScreenshotButton->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(quitScreenshotButton, &QPushButton::clicked, this, &QWidget::close);
    buttonsLayout->addWidget(quitScreenshotButton);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    //create main widget to hold quads
    QWidget *win = new QWidget();
    win->setLayout(mainLayout);
    setCentralWidget(win);
    //resize(600, 1000);

//    QWidget *placeholderWidget = new QWidget;
//    placeholderWidget->setLayout(mainLayout);
//    setCentralWidget(placeholderWidget);


    // QMediaPlayer -> QVideoWidget
    _player1->setVideoOutput(_vw1);
    _player2->setVideoOutput(_vw2);
    _player3->setVideoOutput(_vw3);
    _player4->setVideoOutput(_vw4);


    // link RTSP to Videos
    const QUrl url1 = QUrl("http://192.168.1.2:8081/");
    const QUrl url2 = QUrl("http://192.168.1.2:8082/");
    const QUrl url3 = QUrl("http://192.168.1.2:8081/");
    const QUrl url4 = QUrl("http://192.168.1.2:8082/");

    // network request the URLs
    const QNetworkRequest requestRtsp1(url1);
    const QNetworkRequest requestRtsp2(url2);
    const QNetworkRequest requestRtsp3(url3);
    const QNetworkRequest requestRtsp4(url4);

    // set the request to player
    _player1->setMedia(requestRtsp1);
    _player2->setMedia(requestRtsp2);
    _player3->setMedia(requestRtsp3);
    _player4->setMedia(requestRtsp4);

    // start streaming
    _player1->play();
    _player2->play();
    _player3->play();
    _player4->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveScreenshot()
{
    const QString format = "png";
    QString initialPath = "/home/anette/savedScreenShots";
    QTime ct = QTime::currentTime();
    initialPath += "/" + ct.toString() + "." + format;

    QStringList mimeTypes;
    foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
        mimeTypes.append(QLatin1String(bf));

    QFile file(initialPath);
    file.open(QIODevice::WriteOnly);
    originalPixmap.save(&file, "PNG");
}


void MainWindow::shootScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;


    originalPixmap = screen->grabWindow(0);
    saveScreenshot();

    newScreenshotButton->setDisabled(false);
}


