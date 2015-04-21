#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <QColorDialog>
#include <QFontDialog>
#include <QProgressDialog>
#include <QInputDialog>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // grayColorTable used in cv::image to QImage, gray scale image condition
    if(grayColorTable.isEmpty()) {
        for(int i = 0; i < 256; ++i) {
            grayColorTable.push_back(qRgb(i, i, i));
        }
    }
    MainWindow::connect(ui->hConcat, SIGNAL(triggered()), this, SLOT(hConcatListener()));
    MainWindow::connect(ui->vConcat, SIGNAL(triggered()), this, SLOT(vConcatListener()));
    MainWindow::connect(ui->triConcat, SIGNAL(triggered()), this, SLOT(triConcatListener()));
    MainWindow::connect(ui->hAnimation, SIGNAL(triggered(QAction*)), this, SLOT(hAnimationListener(QAction*)));
    MainWindow::connect(ui->vAnimation, SIGNAL(triggered(QAction*)), this, SLOT(vAnimationListener(QAction*)));
    MainWindow::connect(ui->transition, SIGNAL(triggered()), this, SLOT(transitionListener()));
    MainWindow::connect(ui->rectTransfer, SIGNAL(triggered()), this, SLOT(rectTransferListener()));
    MainWindow::connect(ui->features_SLIC, SIGNAL(triggered()), this, SLOT(features_SLICListener()));
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    qDebug() << QString("Height ") << e->size().height() << QString("Width ") << e->size().width();
}

void MainWindow::ImageCVToQImage(const cv::Mat &imageCV, QImage & imageQ, bool & isOK) {
   switch (imageCV.type()) {
      // 8-bit, 4 channel
      case CV_8UC4:
       imageQ = QImage(imageCV.data, imageCV.cols, imageCV.rows, imageCV.step, QImage::Format_RGB32);
       isOK = true;
       break;
      // 8-bit, 3 channel
      case CV_8UC3:
       imageQ = QImage(imageCV.data, imageCV.cols, imageCV.rows, imageCV.step, QImage::Format_RGB888 );
       isOK = true;
       break;
      // 8-bit, 1 channel
      case CV_8UC1:
       imageQ = QImage(imageCV.data, imageCV.cols, imageCV.rows, imageCV.step, QImage::Format_Indexed8);
       imageQ.setColorTable(grayColorTable);
       isOK = true;
       break;
   default:
       qWarning() << "MainWindow::cvMatToQImage() - cv::Mat image type not handled in switch:" << imageCV.type();
       isOK = false;
       break;
   }
}

void MainWindow::showMessage(const QString message) {
   alert.setText(message);
   alert.exec();
}

void MainWindow::openFile(QString &fileName) {
    fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
}

void MainWindow::showImage(QGraphicsScene &scene, QGraphicsView *gView, const QString &fileName) {
    scene.clear();
    scene.addPixmap(QPixmap(fileName));
    gView->setScene(&scene);
//    gView->adjustSize();
}

void MainWindow::showImage(QGraphicsScene &scene, QGraphicsView *gView, const QImage &image, bool &isOK) {
    QPixmap pixMap;
    if(pixMap.convertFromImage(image)) {
        isOK = true;
        scene.clear();
        scene.addPixmap(pixMap);
        gView->setScene(&scene);
        gView->show();
    }
    else {
        isOK = false;
    }
}
void MainWindow::showImage(QGraphicsScene &scene, QGraphicsView *gView, const cv::Mat &image, bool &isOK) {
    QImage imageQ;
    ImageCVToQImage(image,imageQ, isOK);
    if(isOK == true) {
        showImage(scene, gView, imageQ, isOK);
    }
}

void MainWindow::hConcatListener() {
    std::vector< cv::Mat > imageVec;
    QString fileNameQ;
    do {
        openFile(fileNameQ);
        if(fileNameQ != "") {
            std::string fileName{""};
            fileName = fileNameQ.toStdString();
            cv::Mat image = cv::imread(fileName);
            cv::cvtColor(image, image, CV_BGR2RGB);
            bool isOK;
            showImage(inputGScene, ui->imageView, image, isOK);
            qDebug() << QString("Show Image") + fileNameQ;
            imageVec.push_back(image);
        }
        else {
            qDebug() << "Didn't choose any file.";
            break;
        }
    } while(fileNameQ != "");
    if(imageVec.size() > 0) {
        ImageShowEffects obj;
        cv::Mat result;
        if(obj.horizontalConcat(imageVec, result)) {
            bool isOK;
            showImage(outputGScene, ui->outputImageView, result, isOK);
            qDebug() << "Horizontal concat successfully.";
        }
        else {
            qDebug() << "Vertical concat abortively.";
        }

    }
}

void MainWindow::vConcatListener() {
    std::vector< cv::Mat > imageVec;
    QString fileNameQ;
    do {
        openFile(fileNameQ);
        if(fileNameQ != "") {
            std::string fileName{""};
            fileName = fileNameQ.toStdString();
            cv::Mat image = cv::imread(fileName);
            cv::cvtColor(image, image, CV_BGR2RGB);
            bool isOK;
            showImage(inputGScene, ui->imageView, image, isOK);
            qDebug() << QString("Show Image") + fileNameQ;
            imageVec.push_back(image);
        }
        else {
            qDebug() << "Didn't choose any file.";
            break;
        }
    } while(fileNameQ != "");
    if(imageVec.size() > 0) {
        ImageShowEffects obj;
        cv::Mat result;
        if(obj.verticalConcat(imageVec, result)) {
            bool isOK;
            showImage(outputGScene, ui->outputImageView, result, isOK);
            qDebug() << "Vertical concat successfully.";
        }
        else {
            qDebug() << "Vertical concat abortively.";
        }

    }
}

void MainWindow::triConcatListener() {
    std::vector< cv::Mat > imageVec;
    QString fileNameQ;
    do {
        openFile(fileNameQ);
        if(fileNameQ != "") {
            std::string fileName{""};
            fileName = fileNameQ.toStdString();
            cv::Mat image = cv::imread(fileName);
            cv::cvtColor(image, image, CV_BGR2RGB);
            bool isOK;
            showImage(inputGScene, ui->imageView, image, isOK);
            qDebug() << QString("Show Image") + fileNameQ;
            imageVec.push_back(image);
        }
        else {
            qDebug() << "Didn't choose any file.";
        }
    } while(imageVec.size() < 2);
    if(imageVec.size() == 2) {
        ImageShowEffects obj;
        cv::Mat result;
        if(obj.triangleConcat(imageVec[0], imageVec[1], result)) {
            bool isOK;
            showImage(outputGScene, ui->outputImageView, result, isOK);
            qDebug() << "Triangle concat successfully.";
        }
        else {
            qDebug() << "Triangle concat abortively.";
        }
    }
}

void MainWindow::hAnimationListener(QAction* action) {
    QString actionObjectNameQ = action->objectName();
    int type = ANIMATION_LEFT_IN;
    if(actionObjectNameQ == "hAnimation_Left_In") {
        type = ANIMATION_LEFT_IN;
    }
    else if(actionObjectNameQ == "hAnimation_Right_In") {
        type = ANIMATION_RIGHT_IN;
    }
    else if(actionObjectNameQ == "hAnimation_Left_Out") {
        type = ANIMATION_LEFT_OUT;
    }
    else if(actionObjectNameQ == "hAnimation_Right_Out") {
        type = ANIMATION_RIGHT_OUT;
    }
    cv::Mat image;
    QString fileNameQ;
    openFile(fileNameQ);
    if(fileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string fileName{""};
    fileName = fileNameQ.toStdString();
    image = cv::imread(fileName);
    cv::cvtColor(image, image, CV_BGR2RGB);
    bool isOK;
    showImage(inputGScene, ui->imageView, image, isOK);
    qDebug() << QString("Show Image ") + fileNameQ;
    if(obj.horizontalAnimation(image, outputImageVec, type, 40)) {
        bool isOK = true;
        showImages(outputGScene, ui->outputImageView, isOK);
        qDebug() << "Horizontal animation successfully.";
    }
    else {
        qDebug() << "Horizontal animation abortively.";
    }
}

void MainWindow::vAnimationListener(QAction* action) {
    //qDebug() << action->objectName();
    /*
    QString actionObjectNameQ = action->objectName();
    int type = ANIMATION_TOP_IN;
    if(actionObjectNameQ == "hAnimation_Top_In") {
        type = ANIMATION_TOP_IN;
    }
    else if(actionObjectNameQ == "hAnimation_Bottom_In") {
        type = ANIMATION_Bottom_IN;
    }
    else if(actionObjectNameQ == "hAnimation_Top_Out") {
        type = ANIMATION_Top_OUT;
    }
    else if(actionObjectNameQ == "hAnimation_Bottom_Out") {
        type = ANIMATION_Bottom_OUT;
    }
    */
    QString fileNameQ;
    fileNameQ = QFileDialog::getOpenFileName(this,tr("Open Video"), "", tr("Video Files (*.mp4 *.avi *.mkv)"));
    if(fileNameQ != "") {
        // setup video widget
        QVideoWidget *videoWidget = new QVideoWidget(ui->outputImageView);
        videoWidget->resize(ui->outputImageView->width(), ui->outputImageView->height());
        videoWidget->show();
        QMediaPlayer *player = new QMediaPlayer();
        // volume
        player->setVolume(25);
        // start to play
        player->setVideoOutput(videoWidget);
        QMediaPlaylist *playlist = new QMediaPlaylist(player);
        playlist->addMedia(QUrl::fromLocalFile(fileNameQ));
        playlist->setCurrentIndex(0);
        player->setPlaylist(playlist);
        player->play();
        ui->outputImageViewStop->setEnabled(true);
        connect(ui->outputImageViewStop, SIGNAL(clicked()), player, SLOT(stop()));
        connect(ui->outputImageViewStop, SIGNAL(clicked()), player, SLOT(togglePlay()));
        qDebug() << "Play " << fileNameQ;
    }
    else {
        qDebug() << "Didn't choose any video file.";
    }

//    showMessage("V Animation!");
}

void MainWindow::transitionListener() {
//    LoginDialog *loginDialog = new LoginDialog(this);
//    loginDialog->show();
    showMessage("Transition!");
}

void MainWindow::rectTransferListener() {
    QColor color = QColorDialog::getColor(Qt::red, this, tr("Color map"));
    qDebug() << color;
    bool isOK;
    QFont font = QFontDialog::getFont(&isOK, this);
    qDebug() << font;
    QProgressDialog progressDialog(tr("Process Dialog"), tr("Cancel"), 0, 50000, this);
    for(int i =0; i <= 50000; ++i) {
        progressDialog.setValue(i);
        QCoreApplication::processEvents();
        if(progressDialog.wasCanceled()) {
            progressDialog.cancel();
            break;
        }
    }
    qDebug() << "Progressing completely.";
//    showMessage("Rect Transfer!");
}

void MainWindow::features_SLICListener() {
    //qDebug() << action->objectName();
    // !!! TODO
    QString fileNameQ = QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string fileName = fileNameQ.toStdString();
    cv::Mat image = cv::imread(fileName);
    if(image.empty()) {
        qDebug() << "OpenCV read file " << fileNameQ << " error.";
        return;
    }
    qDebug() << "Open file " << fileNameQ << " successfully.";
    qDebug() << "width " << image.cols << " height " << image.rows;
    cv::cvtColor(image, image, CV_BGR2RGB);
    bool isOK = false;
    showImage(inputGScene, ui->imageView, image, isOK);
    if(!isOK) {
        qDebug() << "Show image error.";
        return;
    }
    int k = QInputDialog::getInt(this, tr("Input"), tr("Super pixel k"), 50, 1, 2000, 1, &isOK);
    if(!isOK) {
        qDebug() << "Get integer k abortively.";
        return;
    }
    int iteration_count = QInputDialog::getInt(this, tr("Input"), tr("Iteration count"), 10, 1, 40, 1, &isOK);
    if(!isOK) {
        qDebug() << "Input iteration count cancelled.";
        return;
    }
    int spatial_distance_weight = QInputDialog::getInt(this, tr("Input"), tr("Spatial distance weight"), 10, 1, 40, 1, &isOK);
    if(!isOK) {
        qDebug() << "Input spatial distance weight cancelled.";
        return;
    }
    cv::Mat image_pixels_label(image.rows, image.cols, CV_32SC1, cv::Scalar(-1));
    std::future<bool> fut = std::async(ImageShowEffects::SLIC, image, image_pixels_label, k, iteration_count, spatial_distance_weight);
    if(fut.get() == true) {
        qDebug() << "SLIC() functions well.";
        /// add contours
        //Extract the contours
        std::vector< std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image_pixels_label, contours, hierarchy, CV_RETR_FLOODFILL, cv::CHAIN_APPROX_NONE);
        qDebug() << contours.size();
        for(int i = 0; i < contours.size(); ++i) {
            if(cv::contourArea(contours.at(i)) >= 20) {
                cv::drawContours(image, contours, i, cv::Scalar(128, 128, 128), 1, CV_AA, hierarchy, 0);
            }
        }
        showImage(outputGScene, ui->outputImageView, image, isOK);
//        image_pixels_label = ((image_pixels_label * 256) - image_pixels_label) / k;
//        cv::Mat image_to_show;
//        image_pixels_label.convertTo(image_to_show, CV_8UC1);

//        cv::Mat color_mapped_image;
//        cv::applyColorMap(image_to_show, color_mapped_image, cv::COLORMAP_AUTUMN);
//        cv::cvtColor(color_mapped_image, color_mapped_image, CV_BGR2RGB);
//        showImage(outputGScene, ui->outputImageView, color_mapped_image, isOK);
    }
    else {
        qDebug() << "SLIC() functions badly.";
    }

}

void MainWindow::showImages(QGraphicsScene &scene, QGraphicsView *gView, bool &isOK) {
    outputGview = gView;
    currentDisplayImageIndex = 0;
    connect(&timer, SIGNAL(timeout()), SLOT(showNextFrame()));
    connect(ui->outputImageViewPlay, SIGNAL(clicked()), &timer, SLOT(start()));
    connect(ui->outputImageViewStop, SIGNAL(clicked()), &timer, SLOT(stop()));
    timer.start(100);
    showNextFrame();
    isOK = true;
}

void MainWindow::showNextFrame() {
    if(currentDisplayImageIndex < outputImageVec.size()) {
        bool isOK = false;
        showImage(outputGScene, outputGview, outputImageVec.at(currentDisplayImageIndex), isOK);
    }
    ++currentDisplayImageIndex;
    if(currentDisplayImageIndex == outputImageVec.size()) {
        currentDisplayImageIndex = 0;
    }
}
