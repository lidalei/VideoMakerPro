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
    MainWindow::connect(ui->file_Exit, SIGNAL(triggered()), this, SLOT(close()));
    MainWindow::connect(ui->hConcat, SIGNAL(triggered()), this, SLOT(hConcatListener()));
    MainWindow::connect(ui->vConcat, SIGNAL(triggered()), this, SLOT(vConcatListener()));
    MainWindow::connect(ui->triConcat, SIGNAL(triggered()), this, SLOT(triConcatListener()));
    MainWindow::connect(ui->hAnimation, SIGNAL(triggered(QAction*)), this, SLOT(hAnimationListener(QAction*)));
    MainWindow::connect(ui->vAnimation, SIGNAL(triggered(QAction*)), this, SLOT(vAnimationListener(QAction*)));
    MainWindow::connect(ui->transition, SIGNAL(triggered()), this, SLOT(transitionListener()));
    MainWindow::connect(ui->rectTransfer, SIGNAL(triggered(QAction*)), this, SLOT(rectTransferListener(QAction *)));
    MainWindow::connect(ui->features_SLIC, SIGNAL(triggered()), this, SLOT(features_SLICListener()));
    MainWindow::connect(ui->features_SIFT, SIGNAL(triggered()), this, SLOT(features_SIFTListener()));
    MainWindow::connect(ui->features_ASLIC, SIGNAL(triggered()), this, SLOT(features_ASLICListener()));
    MainWindow::connect(ui->misc_Super_Resolution, SIGNAL(triggered()), this, SLOT(misc_Super_ResolutionListener()));
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
    QString actionObjectNameQ = action->objectName();
    int type = ANIMATION_LEFT_IN;
    if(actionObjectNameQ == "vAnimation_Top_In") {
        type = ANIMATION_TOP_IN;
    }
    else if(actionObjectNameQ == "vAnimation_Bottom_In") {
        type = ANIMATION_BOTTOM_IN;
    }
    else if(actionObjectNameQ == "vAnimation_Top_Out") {
        type = ANIMATION_TOP_OUT;
    }
    else if(actionObjectNameQ == "vAnimation_Bottom_Out") {
        type = ANIMATION_BOTTOM_OUT;
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
    if(obj.verticalAnimation(image, outputImageVec, type, 40)) {
        bool isOK = true;
        showImages(outputGScene, ui->outputImageView, isOK);
        qDebug() << "Horizontal animation successfully.";
    }
    else {
        qDebug() << "Horizontal animation abortively.";
    }
}

void MainWindow::transitionListener() {
    // read src image
    cv::Mat srcImage;
    QString srcFileNameQ;
    openFile(srcFileNameQ);
    if(srcFileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string srcFileName{""};
    srcFileName = srcFileNameQ.toStdString();
    srcImage = cv::imread(srcFileName);
    cv::cvtColor(srcImage, srcImage, CV_BGR2RGB);
    bool isOK;
    showImage(inputGScene, ui->imageView, srcImage, isOK);
    qDebug() << QString("Show Image ") + srcFileNameQ;
    // read dst image
    cv::Mat dstImage;
    QString dstFileNameQ;
    openFile(dstFileNameQ);
    if(dstFileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string dstFileName{""};
    dstFileName = dstFileNameQ.toStdString();
    dstImage = cv::imread(dstFileName);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    showImage(inputGScene, ui->imageView, dstImage, isOK);
    qDebug() << QString("Show Image ") + dstFileNameQ;
    if(obj.transition(srcImage, dstImage, outputImageVec, 40)) {
        showImages(outputGScene, ui->outputImageView, isOK);
        qDebug() << "Transition successfully.";
    }
    else {
        qDebug() << "Transition abortively.";
    }
}

void MainWindow::rectTransferListener(QAction *action) {
    QString actionObjectNameQ = action->objectName();
    int type = RECT_TRANSFER_BORDER_TO_CENTER;
    if(actionObjectNameQ == "rectTransfer_Top_Left_To") {
        type = RECT_TRANSFER_TOP_LEFT_TO;
    }
    else if(actionObjectNameQ == "rectTransfer_Top_Right_To") {
        type = RECT_TRANSFER_TOP_RIGHT_TO;
    }
    else if(actionObjectNameQ == "rectTransfer_Bottom_Left_To") {
        type = RECT_TRANSFER_BOTTOM_LEFT_TO;
    }
    else if(actionObjectNameQ == "rectTransfer_Bottom_Right_To") {
        type = RECT_TRANSFER_BOTTOM_RIGHT_TO;
    }
    else if(actionObjectNameQ == "rectTransfer_Center_To_Border") {
        type = RECT_TRANSFER_CENTER_TO_BOEDER;
    }
    else if(actionObjectNameQ == "rectTransfer_Border_To_Center") {
        type = RECT_TRANSFER_BORDER_TO_CENTER;
    }
    else if(actionObjectNameQ == "rectTransfer_Left_To_Right") {
        type = RECT_TRANSFER_LEFT_TO_RIGHT;
    }
    else if(actionObjectNameQ == "rectTransfer_Right_To_Left") {
        type = RECT_TRANSFER_RIGHT_TO_LEFT;
    }
    else if(actionObjectNameQ == "rectTransfer_Top_To_Bottom") {
        type = RECT_TRANSFER_TOP_TO_BOTTOM;
    }
    else if(actionObjectNameQ == "rectTransfer_Bottom_To_Top") {
        type = RECT_TRANSFER_BOTTOM_TO_TOP;
    }
    // read src image
    cv::Mat srcImage;
    QString srcFileNameQ;
    openFile(srcFileNameQ);
    if(srcFileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string srcFileName{""};
    srcFileName = srcFileNameQ.toStdString();
    srcImage = cv::imread(srcFileName);
    cv::cvtColor(srcImage, srcImage, CV_BGR2RGB);
    bool isOK;
    showImage(inputGScene, ui->imageView, srcImage, isOK);
    qDebug() << QString("Show Image ") + srcFileNameQ;
    // read dst image
    cv::Mat dstImage;
    QString dstFileNameQ;
    openFile(dstFileNameQ);
    if(dstFileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string dstFileName{""};
    dstFileName = dstFileNameQ.toStdString();
    dstImage = cv::imread(dstFileName);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    showImage(inputGScene, ui->imageView, dstImage, isOK);
    qDebug() << QString("Show Image ") + dstFileNameQ;
    if(obj.rectTransfer(srcImage, dstImage, outputImageVec, type, 40)) {
        showImages(outputGScene, ui->outputImageView, isOK);
        qDebug() << "Rect Transfer successfully.";
    }
    else {
        qDebug() << "Rect Transfer abortively.";
    }
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
void MainWindow::features_SIFTListener() {
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
    int imageWidth = image.cols, imageHeight = image.rows;
    qDebug() << "width " << imageWidth << " height " << imageHeight;
    cv::cvtColor(image, image, CV_BGR2RGB);
    qDebug() << "Transfer to RGB from BGR.";
    bool isOK = false;
    showImage(inputGScene, ui->imageView, image, isOK);
    if(!isOK) {
        qDebug() << "Show image error.";
        return;
    }
    cv::Mat gray_image;
    cv::cvtColor(image,gray_image, CV_RGB2GRAY);
    qDebug() << "Transfer to Gray from RGB.";
    /// step 1, Gaussian Blur
    double sigma = QInputDialog::getDouble(this, tr("Input"), tr("Variance sigma"), 1.6, 1.0, 6.0, 1, &isOK);
    if(!isOK) {
        qDebug() << "Get double sigma abortively.";
        return;
    }
    int Gaussian_blur_kernel_length = std::ceil(6 * sigma);
    if((Gaussian_blur_kernel_length & 1) == 0) {
        Gaussian_blur_kernel_length |= 1;
    }
    qDebug() << Gaussian_blur_kernel_length;
    std::vector< double > Gaussian_blur_coeffs(Gaussian_blur_kernel_length, 0.0);
    double sum_Gaussian_blur_coeffs = 0.0;
    for(int i = 0; i < Gaussian_blur_kernel_length; ++i) {
        Gaussian_blur_coeffs.at(i) = std::expl(-std::pow((i - (Gaussian_blur_kernel_length >> 1)), 2) / (2*sigma*sigma));
        sum_Gaussian_blur_coeffs += Gaussian_blur_coeffs.at(i);
    }
    // normalize
    for(int i = 0; i < Gaussian_blur_kernel_length; ++i) {
        Gaussian_blur_coeffs.at(i) /= sum_Gaussian_blur_coeffs;
    }
    // row Gaussian filter
    for(int row = 0; row < imageHeight; ++row) {
        for(int col = 0; col < imageWidth; ++col) {
            int col_start = std::max(0, col - (Gaussian_blur_kernel_length >> 1));
            int col_end = std::min(imageWidth - 1, col_start + Gaussian_blur_kernel_length - 1);
            double weighted_pixel_gray_value = 0.0;
            for(int weighted_col = col_start; weighted_col <= col_end; ++weighted_col) {
                weighted_pixel_gray_value += Gaussian_blur_coeffs.at(weighted_col - col_start) * gray_image.at<uchar>(row, weighted_col);
            }
            gray_image.at<uchar>(row, col) = weighted_pixel_gray_value;
        }
    }
    // column Gaussian filter
    for(int row = 0; row < imageHeight; ++row) {
        for(int col = 0; col < imageWidth; ++col) {
            int row_start = std::max(0, row - (Gaussian_blur_kernel_length >> 1));
            int row_end = std::min(imageHeight - 1, row_start + Gaussian_blur_kernel_length - 1);
            double weighted_pixel_gray_value = 0.0;
            for(int weighted_row = row_start; weighted_row <= row_end; ++weighted_row) {
                weighted_pixel_gray_value += Gaussian_blur_coeffs.at(weighted_row - row_start) * gray_image.at<uchar>(weighted_row, col);
            }
            gray_image.at<uchar>(row, col) = weighted_pixel_gray_value;
        }
    }
    /// step 2


    /// test
    showImage(outputGScene, ui->outputImageView, gray_image, isOK);

}

void MainWindow::features_ASLICListener() {
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
    cv::Mat image_pixels_label(image.rows, image.cols, CV_32SC1, cv::Scalar(-1));
    std::future<bool> fut = std::async(ImageShowEffects::ASLIC, image, image_pixels_label, k, iteration_count);
    if(fut.get() == true) {
        qDebug() << "ASLIC() functions well.";
        /// add contours
        //Extract the contours
        std::vector< std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image_pixels_label, contours, hierarchy, CV_RETR_FLOODFILL, cv::CHAIN_APPROX_NONE);
        qDebug() << contours.size();
        int threshold = image.cols * image.rows / k >> 2;
        for(int i = 0; i < contours.size(); ++i) {
            if(cv::contourArea(contours.at(i)) >= threshold) {
                cv::drawContours(image, contours, i, cv::Scalar(255, 0, 0), 1, CV_AA, hierarchy, 0);
            }
        }
        showImage(outputGScene, ui->outputImageView, image, isOK);
        // save as
        std::string save_as_fileName = std::string("E:/ASLIC_.jpg");
        cv::cvtColor(image, image, CV_RGB2BGR);
        qDebug() << "Save ASLIC image "<< cv::imwrite(save_as_fileName, image);
    }
    else {
        qDebug() << "SLIC() functions badly.";
    }
}

void MainWindow::misc_Super_ResolutionListener() {
    QString fileNameQ = QFileDialog::getOpenFileName(this,tr("Open video"), "", tr("Video Files (*.mp4 *.avi)"));
    if(fileNameQ == "") {
        qDebug() << "Didn't choose any file.";
        return;
    }
    std::string inputVideoName = fileNameQ.toStdString();
    /// get frame count
    cv::VideoCapture inputVideoCapture(inputVideoName);
    if(!inputVideoCapture.isOpened()) {
        inputVideoCapture.open(inputVideoName);
    }
    if(!inputVideoCapture.isOpened()) {
        qDebug() << tr("Open ")  << fileNameQ << tr(" error");
        return;
    }
    int frameNumber =  inputVideoCapture.get(CV_CAP_PROP_FRAME_COUNT);
    if(frameNumber <= 1) {
        qDebug() << "Too few frames";
        return;
    }
    inputVideoCapture.release();
    cv::Ptr<cv::superres::FrameSource> frameSource;
    cv::Ptr<cv::superres::SuperResolution> superRes;
    cv::Ptr<cv::superres::DenseOpticalFlowExt> of;
    if(cv::gpu::getCudaEnabledDeviceCount() > 0) {
        cv::gpu::setDevice(0);
        frameSource = cv::superres::createFrameSource_Video_GPU(inputVideoName);
        superRes = cv::superres::createSuperResolution_BTVL1_GPU();
        of = cv::superres::createOptFlow_Farneback_GPU();
    }
    else {
        frameSource = cv::superres::createFrameSource_Video(inputVideoName);
        superRes = cv::superres::createSuperResolution_BTVL1();
        of = cv::superres::createOptFlow_Farneback();
    }
    qDebug() << "Initialize successfully!";
    if (of.empty()) {
        qDebug() << "Execeute abortively!";
        return;
    }
    superRes->set("opticalFlow", of);
    superRes->set("scale", 2);
    superRes->set("iterations", 40);
    superRes->set("temporalAreaRadius", 4);
    // skip first frame, it is usually corrupted
    {
        cv::Mat frame;
        frameSource->nextFrame(frame);
    }
    superRes->setInput(frameSource);
    std::string outputVideoName{ "result.avi" };
    cv::VideoWriter vWriter;
    QProgressDialog progressDialog(tr("Process Dialog"), tr("Cancel"), 1, frameNumber, this, Qt::Dialog);
    for(int i = 1; i <= frameNumber; ++i) {
        cv::Mat result;
        superRes->nextFrame(result);
        if (result.empty()) {
            break;
        }
        if (!vWriter.isOpened()) {
            vWriter.open(outputVideoName, CV_FOURCC('X', 'V', 'I', 'D'), 25.0, result.size());
        }
        vWriter << result;
        progressDialog.setValue(i);
        QCoreApplication::processEvents();
        if(progressDialog.wasCanceled()) {
            progressDialog.cancel();
            break;
        }
    }
    superRes->collectGarbage();
    vWriter.release();
    qDebug() << "Execute successfully!";
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
