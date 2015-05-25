#ifndef IMAGE_SHOW_EFFECTS_H
#define IMAGE_SHOW_EFFECTS_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/superres/optical_flow.hpp>
#include <opencv2/superres/superres.hpp>
#include <cmath>

const int ANIMATION_LEFT_IN(1);
const int ANIMATION_RIGHT_IN(2);
const int ANIMATION_TOP_IN(3);
const int ANIMATION_BOTTOM_IN(4);
const int ANIMATION_LEFT_OUT(5);
const int ANIMATION_RIGHT_OUT(6);
const int ANIMATION_TOP_OUT(7);
const int ANIMATION_BOTTOM_OUT(8);
const int ANIMATION_TOP_LEFT_IN(9);
const int ANIMATION_TOP_RIGHT_IN(10);
const int ANIMATION_BOTTON_LEFT_IN(11);
const int ANIMATION_BOTTOM_RIGHT_IN(12);
const int ANIMATION_TOP_LEFT_OUT(13);
const int ANIMATION_TOP_RIGHT_OUT(14);
const int ANIMATION_BOTTON_LEFT_OUT(15);
const int ANIMATION_BOTTOM_RIGHT_OUT(16);

const int RECT_TRANSFER_TOP_LEFT_TO(17);
const int RECT_TRANSFER_TOP_RIGHT_TO(18);
const int RECT_TRANSFER_BOTTOM_LEFT_TO(19);
const int RECT_TRANSFER_BOTTOM_RIGHT_TO(20);
const int RECT_TRANSFER_CENTER_TO_BOEDER(21);
const int RECT_TRANSFER_BORDER_TO_CENTER(22);
const int RECT_TRANSFER_LEFT_TO_RIGHT(23);
const int RECT_TRANSFER_RIGHT_TO_LEFT(24);
const int RECT_TRANSFER_TOP_TO_BOTTOM(25);
const int RECT_TRANSFER_BOTTOM_TO_TOP(26);

const int MOSAIC_LOAD_TOP_TO_BOTTOM(27);
const int MOSAIC_LOAD_BOTTOM_TO_TOP(28);
const int MOSAIC_LOAD_LEFT_TO_RIGHT(29);
const int MOSAIC_LOAD_RIGHT_TO_LEFT(30);

class ImageShowEffects {
public:
        /**
        * constructor
        */
        ImageShowEffects();
        /**
        * deconstructor
        */
        ~ImageShowEffects();
        /**
        * horizontal concat images
        * if any one  of images is empty, return false, otherwise, return true
        */
        bool horizontalConcat(const std::vector<cv::Mat> & images, cv::Mat & resultImage);
        /**
        * vertical concat images
        * if any one  of images is empty, return false, otherwise, return true
        */
        bool verticalConcat(const std::vector<cv::Mat> & images, cv::Mat & resultImage);
        /**
        * triangle concat two images
        * if any one  of images is empty, return false, otherwise, return true
        */
        bool triangleConcat(const cv::Mat & image1, const cv::Mat & image2, cv::Mat & resultImage);
        /**
        * @para image, given an image
        * @para images, generate a sequence of image to realize horizontal animation effect
        * @para animationType, animation type
        * @para number,  stands for the size of generated image sequence
        * if image is empty, return false, otherwise return true
        */
        bool horizontalAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType = ANIMATION_LEFT_IN, const int number = 2);
        /**
        * @para image, given an image
        * @para images, generate a sequence of image to realize vertical animation effect
        * @para animationType, animation type
        * @para number,  stands for the size of generated image sequence
        * if image is empty, return false, otherwise return true
        */
        bool verticalAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType = ANIMATION_TOP_IN, const int number = 2);
        /**
        * @para image, given an image
        * @para images, generate a sequence of image to realize load horizontal animation effect
        * @para number,  stands for the size of generated image sequence
        * if image is empty, return false, otherwise return true
        */
        bool horizontalAnimationLoad(const cv::Mat & image, std::vector<cv::Mat> & images, const int number = 2);
        /**
        * @para image, given an image
        * @para images, generate a sequence of image to realize vertical load animation effect
        * @para number,  stands for the size of generated image sequence
        * if image is empty, return false, otherwise return true
        */
        bool verticalAnimationLoad(const cv::Mat & image, std::vector<cv::Mat> & images, const int number = 2);
        /**
        * @para image, given an image
        * @para images, generate a sequence of image to realize gradient animation effect
        * @para animationType, animation type
        * @para number,  stands for the size of generated image sequence
        * if image is empty, return false, otherwise return true
        */
        bool gradientAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType = ANIMATION_TOP_LEFT_IN, const int number = 2);
        /**
        * zoom in local image, image is empty, return true, otherwise return false
        * @para image, given image
        * @para resultImage, output image
        * @para position, given position
        * @para diameter, width & height of the square
        * TODO!!!!!!!!!!
        */
        bool localZoomIn(const cv::Mat & image, cv::Mat & resultImage, const cv::Point & position, const int diameter = 6);
        /**
        * cascade two images
        * @para Mat srcImage1
        * @para Mat srcImage2
        * @para Mat dstImage
        * @para double alpha
        */
        bool cascade(const cv::Mat & srcImage1, const cv::Mat & srcImage2, cv::Mat & dstImage, const double alpha=0.5);
        /**
        * transition two images
        * @para Mat srcImage
        * @para Mat dstImage
        * @para vector< Mat > resultImageVec
        * @para int fnumber
        */
        bool transition(const cv::Mat & srcImage, const cv::Mat & dstImage, std::vector< cv::Mat>  & resultImageVec, const unsigned int number = 30);
        /**
        * rectangular transition two images
        * @para Mat srcImage
        * @para Mat dstImage
        * @para int transferType
        * @para vector< Mat > resultImageVec
        * @para int number
        */
        bool rectTransfer(const cv::Mat & srcImage, const cv::Mat & dstImage, std::vector< cv::Mat>  & resultImageVec, const int transferType = RECT_TRANSFER_CENTER_TO_BOEDER, const unsigned int number = 30);
        /**
        * load image effect
        * @para Mat srcImage
        * @para vector< Nat > resultImagevec
        * @para int loadType orientation
        * @para int blockHeight
        * @para int blockWidth
        */
        bool mosaicLoad(const cv::Mat & srcImage, std::vector< cv::Mat > & resultImageVec, const int type = MOSAIC_LOAD_TOP_TO_BOTTOM, const int blockHeight = 40, const int blockWidth = 40);
        /**
        * calculate image gradient
        * @para Mat srcImage
        * @para Mat gradient
        */
        static bool gradient(const cv::Mat &srcImage, cv::Mat &gradient);
        /**
        * SLIC super pixel
        * @para Mat srcImage
        * @para Mat image_pixels_label, type is CV_32SC1
        * @para int k
        * @para int iteration count
        * @para int spatial distance weight
        */
        static bool SLIC(const cv::Mat & srcImage, cv::Mat &image_pixels_label, const int & k, const int & iteration_count = 20, const int & spatial_distance_weight = 4);
        /**
        * SLIC super pixel
        * @para Mat srcImage
        * @para Mat image_pixels_label, type is CV_32SC1
        * @para int k
        * @para int iteration count
        */
        static bool ASLIC(const cv::Mat & srcImage, cv::Mat &image_pixels_label, const int & k, const int & iteration_count = 20);

private:


};

#endif
