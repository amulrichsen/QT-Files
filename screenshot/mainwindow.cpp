#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QWindow>
#include <QScreen>
#include <QLayout>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    screenshotLabel(new QLabel(this))
{
    //ui->setupUi(this);

    screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    screenshotLabel->setMinimumSize(screenGeometry.width() / 2, screenGeometry.height() / 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(screenshotLabel);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    newScreenshotButton = new QPushButton(tr("New Screenshot"), this);
    connect(newScreenshotButton, &QPushButton::clicked, this, &MainWindow::shootScreen);
    buttonsLayout->addWidget(newScreenshotButton);
    //QPushButton *saveScreenshotButton = new QPushButton(tr("Save Screenshot"), this);
    //connect(saveScreenshotButton, &QPushButton::clicked, this, &MainWindow::saveScreenshot);
    //buttonsLayout->addWidget(saveScreenshotButton);
    QPushButton *quitScreenshotButton = new QPushButton(tr("Quit"), this);
    quitScreenshotButton->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(quitScreenshotButton, &QPushButton::clicked, this, &QWidget::close);
    buttonsLayout->addWidget(quitScreenshotButton);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    QWidget *placeholderWidget = new QWidget;
    placeholderWidget->setLayout(mainLayout);
    setCentralWidget(placeholderWidget);

    setWindowTitle(tr("Screenshot"));
    resize(600, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = originalPixmap.size();
    scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    if (!screenshotLabel->pixmap() || scaledSize != screenshotLabel->pixmap()->size())
        updateScreenshotLabel();
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
    updateScreenshotLabel();
    saveScreenshot();

    newScreenshotButton->setDisabled(false);
}

void MainWindow::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}
