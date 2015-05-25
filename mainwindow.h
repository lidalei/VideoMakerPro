#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QtCore>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QDebug>

#include "ImageShowEffects.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ImageCVToQImage(const cv::Mat &imageCV, QImage & imageQ, bool & isOK);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    Ui::MainWindow *ui;
    QMessageBox alert;
    QGraphicsScene inputGScene;
    QGraphicsScene outputGScene;
    QVector<QRgb>  grayColorTable;
    void showMessage(const QString message);
    void openFile(QString &fileName);
    void showImage(QGraphicsScene &scene, QGraphicsView *gView, const QString &fileName);
    void showImage(QGraphicsScene &scene, QGraphicsView *gView, const QImage &image, bool &isOK);
    void showImage(QGraphicsScene &scene, QGraphicsView *gView, const cv::Mat &image, bool &isOK);
    QTimer timer;
    void showImages(QGraphicsScene &scene, QGraphicsView *gView, bool &isOK);
    ImageShowEffects obj;
    QGraphicsView * outputGview;
    std::vector< cv::Mat > outputImageVec;
    int currentDisplayImageIndex;

private slots:
    void hConcatListener();
    void vConcatListener();
    void triConcatListener();
    void hAnimationListener(QAction*action);
    void vAnimationListener(QAction*action);
    void transitionListener();
    void rectTransferListener(QAction *action);
    void features_SLICListener();
    void features_SIFTListener();
    void features_ASLICListener();
    void misc_Super_ResolutionListener();
    void showNextFrame();
};

#endif // MAINWINDOW_H
