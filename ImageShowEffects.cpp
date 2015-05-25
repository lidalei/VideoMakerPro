#include "ImageShowEffects.h"

/**
* constructor
*/
ImageShowEffects::ImageShowEffects() {


}
/**
* deconstructor
*/
ImageShowEffects::~ImageShowEffects() {


}

/**
* horizontal concat images
* if any one  of images is empty, return false, otherwise, return true
*/
bool ImageShowEffects::horizontalConcat(const std::vector<cv::Mat> & images, cv::Mat & resultImage) {
        std::vector<cv::Mat> _images;
        for (size_t i = 0; i < images.size(); ++i) {
                if (images[i].rows == 0) {
                        return false;
                }
                else {
                        // resize other images.rows to be the same with images[0]'s
                        _images.push_back(cv::Mat());
                        resize(images[i], _images[i], cv::Size(images[i].cols*images[0].rows / images[i].rows, images[0].rows));
                }
        }

        hconcat(_images, resultImage);

        return true;
}

/**
* vertical concat images
* if any one  of images is empty, return false, otherwise, return true
*/
bool ImageShowEffects::verticalConcat(const std::vector<cv::Mat> & images, cv::Mat & resultImage) {
        std::vector<cv::Mat> _images;
        for (size_t i = 0; i < images.size(); ++i) {
                if (images[i].rows == 0) {
                        return false;
                }
                else {
                        // resize other images.cols to be the same with images[0]'s
                        _images.push_back(cv::Mat());
                        resize(images[i], _images[i], cv::Size(images[0].cols, images[i].rows*images[0].cols / images[i].cols));
                }
        }

        vconcat(_images, resultImage);

        return true;
}



/**
* triangle concat two images
* if any one of images is empty, or size is not the same, return false,
* otherwise, return true
*/
bool ImageShowEffects::triangleConcat(const cv::Mat & image1, const cv::Mat & image2, cv::Mat & resultImage) {
        if (image1.rows == 0 || image2.rows == 0 || image1.size != image2.size) {
                return false;
        }
        int image1Channels = image1.channels();
        resultImage = image1.clone();
        if (image1Channels == 1) { // gray image
                for (int i = 0; i < resultImage.cols; ++i) {
                        for (int j = resultImage.rows - 1; j > resultImage.rows*(resultImage.cols - i) / resultImage.cols; --j) {
                                resultImage.at<uchar>(j, i) = image2.at<uchar>(j, i);
                        }
                }
        }
        else if (image1Channels == 3) { // RGB image
                for (int i = 0; i < resultImage.cols; ++i) {
                        for (int j = resultImage.rows - 1; j > resultImage.rows*(resultImage.cols - i) / resultImage.cols; --j) {
                                resultImage.at<cv::Vec3b>(j, i) = image2.at<cv::Vec3b>(j, i);
                        }
                }
        }
        else if (image1Channels == 4) {
                for (int i = 0; i < resultImage.cols; ++i) {
                        for (int j = resultImage.rows - 1; j > resultImage.rows*(resultImage.cols - i) / resultImage.cols; --j) {
                                resultImage.at<cv::Vec4b>(j, i) = image2.at<cv::Vec4b>(j, i);
                        }
                }
        }
        return true;
}

/**
* @para image, given an image
* @para images, generate a sequence of image to realize horizontal animation effect
* @para animationType, animation type
* @para number,  stands for the size of generated image sequence
* if image is empty, return false, otherwise return true
*/
bool ImageShowEffects::horizontalAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType, const int number) {
        if (image.rows == 0) {
                return false;
        }
        int imageChannels = image.channels();
        int footLength = image.cols / number;
        int animationLength(0);
        int visibleWidth(0);
        images.clear();
        switch(animationType) {
        case ANIMATION_LEFT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row, column + (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, column + (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4){
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, column + (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (visibleWidth != image.cols) {
                        cv::Mat frame = image.clone();;
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_RIGHT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = frame.cols - visibleWidth; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row, column - (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = frame.cols - visibleWidth; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, column - (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleWidth = footLength*i;
                                for (int column = frame.cols - visibleWidth; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, column - (frame.cols - visibleWidth));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (visibleWidth != image.cols) {
                        cv::Mat frame = image.clone();;
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_LEFT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row, animationLength + column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, animationLength + column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = 0; column < visibleWidth; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, animationLength + column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationLength != image.cols) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_RIGHT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = animationLength; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row, column - animationLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = animationLength; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, column - animationLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleWidth = frame.cols - animationLength;
                                for (int column = animationLength; column < frame.cols; ++column) {
                                        for (int row = 0; row < frame.rows; ++row) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, column - animationLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationLength != image.cols) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        default:
                return false;
                // break;
        }
}

/**
* @para image, given an image
* @para images, generate a sequence of image to realize vertical animation effect
* @para animationType, animation type
* @para number,  stands for the size of generated image sequence
* if image is empty, return false, otherwise return true
*/
bool ImageShowEffects::verticalAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType, const int number) {
        if (image.rows == 0) {
                return false;
        }

        int imageChannels = image.channels();

        int footLength = image.rows / number;
        int animationLength(0);
        int visibleHeight(0);
        switch (animationType) {
        case ANIMATION_BOTTOM_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = frame.rows - visibleHeight; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = frame.rows - visibleHeight; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4){
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = frame.rows - visibleHeight; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (visibleHeight != image.rows) {
                        cv::Mat frame = image.clone();;
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_TOP_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                visibleHeight = footLength*i;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + (frame.rows - visibleHeight), column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (visibleHeight != image.rows) {
                        cv::Mat frame = image.clone();;
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_BOTTOM_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleHeight = frame.rows - animationLength;
                                for (int row = animationLength; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleHeight = frame.rows - animationLength;
                                for (int row = animationLength; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                animationLength = footLength*i;
                                visibleHeight = frame.rows - animationLength;
                                for (int row = animationLength; row < frame.rows; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_TOP_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                int animationLength = footLength*i;
                                int visibleHeight = frame.rows - animationLength;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels==3) { // RGB image
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                int animationLength = footLength*i;
                                int visibleHeight = frame.rows - animationLength;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                // visible region
                                int animationLength = footLength*i;
                                int visibleHeight = frame.rows - animationLength;
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < frame.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + animationLength, column);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        default:
                return false;
                // break;
        }
}

/**
* @para image, given an image
* @para images, generate a sequence of image to realize horizontal animation effect
* @para number,  stands for the size of generated image sequence
* if image is empty, return false, otherwise return true
*/
bool ImageShowEffects::horizontalAnimationLoad(const cv::Mat & image, std::vector<cv::Mat> & images, const int number) {
        if (image.rows == 0) {
                return false;
        }
        int imageChannels = image.channels();
        int footLength = image.cols / number;
        int animationLength(0);
        if (imageChannels == 1) { // gray image
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < frame.rows; ++row) {
                                for (int column = 0; column < animationLength; ++column) {
                                        frame.at<uchar>(row, column) = image.at<uchar>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        else if (imageChannels == 3) { // RGB image
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < frame.rows; ++row) {
                                for (int column = 0; column < animationLength; ++column) {
                                        frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        else if (imageChannels == 4) {
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < frame.rows; ++row) {
                                for (int column = 0; column < animationLength; ++column) {
                                        frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        if (animationLength != image.rows) {
                cv::Mat tempImage = image.clone();
                images.push_back(tempImage);
        }

        return true;
}

/**
* @para image, given an image
* @para images, generate a sequence of image to realize vertical animation effect
* @para number,  stands for the size of generated image sequence
* if image is empty, return false, otherwise return true
*/
bool ImageShowEffects::verticalAnimationLoad(const cv::Mat & image, std::vector<cv::Mat> & images, const int number) {
        if (image.rows == 0) {
                return false;
        }
        int imageChannels = image.channels();
        int footLength = image.rows / number;
//        int depth = image.depth();
        int animationLength(0);
        if (imageChannels == 1) { // gray image
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < animationLength; ++row) {
                                for (int column = 0; column < frame.cols; ++column) {
                                        frame.at<uchar>(row, column) = image.at<uchar>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        else if (imageChannels == 3) { // RGB image
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < animationLength; ++row) {
                                for (int column = 0; column < frame.cols; ++column) {
                                        frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        else if (imageChannels == 4) {
                for (int i = 0; i < number; ++i) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        animationLength = footLength*i;
                        for (int row = 0; row < animationLength; ++row) {
                                for (int column = 0; column < frame.cols; ++column) {
                                        frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row, column);
                                }
                        }
                        images.push_back(frame);
                }
        }
        if (animationLength != image.rows) {
                cv::Mat tempImage = image.clone();
                images.push_back(tempImage);
        }

        return true;
}

/**
* @para image, given an image
* @para images, generate a sequence of image to realize gradient animation effect
* @para animationType, animation type
* @para number,  stands for the size of generated image sequence
* if image is empty, return false, otherwise return true
*/
bool ImageShowEffects::gradientAnimation(const cv::Mat & image, std::vector<cv::Mat> & images, const int animationType, const int number) {
        if (image.rows == 0) {
                return false;
        }
        int imageChannels = image.channels();
        int stepXLength = image.rows / number;
        int stepYLength = image.cols / number;
        int animationXLength(0);
        int animationYLength(0);
        int visibleHeight(0);
        int visibleWidth(0);
        switch (animationType) {
        case ANIMATION_TOP_LEFT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + image.rows - animationXLength, column + image.cols - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + image.rows - animationXLength, column + image.cols - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + image.rows - animationXLength, column + image.cols - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = image.clone();
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_TOP_RIGHT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + image.rows - animationXLength, column - image.cols + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + image.rows - animationXLength, column - image.cols + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < animationXLength; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + image.rows - animationXLength, column - image.cols + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = image.clone();
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_BOTTON_LEFT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - (image.rows - animationXLength), column + (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - (image.rows - animationXLength), column + (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < animationYLength; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - (image.rows - animationXLength), column + (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = image.clone();
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_BOTTOM_RIGHT_IN:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - (image.rows - animationXLength), column - (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - (image.rows - animationXLength), column - (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = image.rows - animationXLength; row < image.rows; ++row) {
                                        for (int column = image.cols - animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - (image.rows - animationXLength), column - (image.cols - animationYLength));
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = image.clone();
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_TOP_LEFT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_TOP_RIGHT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = animationYLength; column <image.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row + animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = animationYLength; column <image.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row + animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = 0; row < visibleHeight; ++row) {
                                        for (int column = animationYLength; column <image.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row + animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_BOTTON_LEFT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = 0; column < visibleWidth; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - animationXLength, column + animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        case ANIMATION_BOTTOM_RIGHT_OUT:
                if (imageChannels == 1) { // gray image
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = animationYLength; column < image.cols; ++column) {
                                                frame.at<uchar>(row, column) = image.at<uchar>(row - animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 3) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec3b>(row, column) = image.at<cv::Vec3b>(row - animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                else if (imageChannels == 4) {
                        for (int i = 0; i < number; ++i) {
                                animationXLength = i * stepXLength;
                                animationYLength = i * stepYLength;
                                visibleWidth = image.cols - animationYLength;
                                visibleHeight = image.rows - animationXLength;
                                cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                                for (int row = animationXLength; row < image.rows; ++row) {
                                        for (int column = animationYLength; column < image.cols; ++column) {
                                                frame.at<cv::Vec4b>(row, column) = image.at<cv::Vec4b>(row - animationXLength, column - animationYLength);
                                        }
                                }
                                images.push_back(frame);
                        }
                }
                if (animationXLength != image.rows) {
                        cv::Mat frame = cv::Mat::zeros(image.rows, image.cols, image.type());
                        images.push_back(frame);
                }
                return true;
                // break;
        default:
                return false;
        }
}


/**
* zoom in local image, image is empty, return true, otherwise return false
* @para image, given image
* @para resultImage, output image
* @para position, given position
* @para diameter, width & height of the square
*/
bool ImageShowEffects::localZoomIn(const cv::Mat & image, cv::Mat & resultImage, const cv::Point & position, const int diameter) {
        if (image.rows == 0) {
                return false;
        }
        // TODO!!!!!!!!!!

        return true;
}

/**
* cascade two images
* @para Mat srcImage1
* @para Mat srcImage2
* @para Mat dstImage
* @para double alpha
*/
bool ImageShowEffects::cascade(const cv::Mat & srcImage1, const cv::Mat & srcImage2, cv::Mat & dstImage, const double alpha) {
        if (srcImage1.empty() || srcImage2.empty()) {
                std::cout << "Empty image error!" << std::endl;
                return false;
        }
        if (srcImage1.type() != srcImage2.type()) {
                std::cout << "Image type inconsistent!" << std::endl;
                return false;
        }
        int srcImage1Channels = srcImage1.channels();
        cv::Mat tempSrcImage2;
        resize(srcImage2, tempSrcImage2, cv::Size(srcImage1.cols, srcImage1.rows));
        dstImage.release();
        dstImage = cv::Mat::zeros(srcImage1.rows, srcImage1.cols, srcImage1.type());
        if (srcImage1Channels == 1) {
                for (int i = 0; i < dstImage.rows; ++i) {
                        for (int j = 0; j < dstImage.cols; ++j) {
                                dstImage.at<uchar>(i, j) = (int)(alpha*srcImage1.at<uchar>(i, j) + (1 - alpha)*tempSrcImage2.at <uchar>(i, j));
                        }
                }
        }
        else if (srcImage1Channels == 3) {
                for (int i = 0; i < dstImage.rows; ++i) {
                        for (int j = 0; j < dstImage.cols; ++j) {
                                dstImage.at<cv::Vec3b>(i, j) = alpha*srcImage1.at<cv::Vec3b>(i, j) + (1 - alpha)*tempSrcImage2.at <cv::Vec3b>(i, j);
                        }
                }
        }
        else if (srcImage1Channels == 4) {
                for (int i = 0; i < dstImage.rows; ++i) {
                        for (int j = 0; j < dstImage.cols; ++j) {
                                dstImage.at<cv::Vec4b>(i, j) = alpha*srcImage1.at<cv::Vec4b>(i, j) + (1 - alpha)*tempSrcImage2.at <cv::Vec4b>(i, j);
                        }
                }
        }
        return true;
}

/**
* cascade two images
* @para Mat srcImage
* @para Mat dstImage
* @para vector< Mat > resultImageVec
* @para int number
*/
bool ImageShowEffects::transition(const cv::Mat & srcImage, const cv::Mat & dstImage, std::vector< cv::Mat>  & resultImageVec, const unsigned int number) {
        if (srcImage.empty() || dstImage.empty()) {
                std::cout << "Empty image error!" << std::endl;
                return false;
        }
        if (srcImage.type() != dstImage.type()) {
                std::cout << "Image type inconsistent!" << std::endl;
                return false;
        }
        cv::Mat tempDstImage;
        resize(dstImage, tempDstImage, cv::Size(srcImage.cols, srcImage.rows));
        resultImageVec.push_back(srcImage);
        for (unsigned int i = 0; i < number; ++i) {
                cv::Mat tempResultImage;
                cascade(srcImage, tempDstImage, tempResultImage, (1 - double(i + 1) / number));
                resultImageVec.push_back(tempResultImage);
        }
        resultImageVec.push_back(tempDstImage);
        return true;
}

/**
* rectangular transition two images
* @para Mat srcImage
* @para Mat dstImage
* @para vector< Mat > resultImageVec
* @para int transferType
* @para int number
*/
bool ImageShowEffects::rectTransfer(const cv::Mat & srcImage, const cv::Mat & dstImage, std::vector< cv::Mat>  & resultImageVec, const int transferType, const unsigned int number) {
        if (srcImage.empty() || dstImage.empty()) {
                std::cout << "Empty image error!" << std::endl;
                return false;
        }
        if (srcImage.type() != dstImage.type()) {
                std::cout << "Image type inconsistent!" << std::endl;
                return false;
        }

        int srcImageChannels = srcImage.channels();

        cv::Mat tempDstImage;
        resize(dstImage, tempDstImage, cv::Size(srcImage.cols, srcImage.rows));

        resultImageVec.push_back(srcImage);

        int stepXLength = srcImage.rows / number;
        int stepYLength = srcImage.cols / number;
        int transferXLength(0);
        int transferYLength(0);
        int transferXStart(0);
        int transferXEnd(0);
        int transferYStart(0);
        int transferYEnd(0);

        if (srcImageChannels == 1) {
                for (unsigned int i = 0; i < number; ++i) {
                        transferXLength = stepXLength*(i + 1);
                        transferYLength = stepYLength*(i + 1);
                        switch (transferType) {
                        case RECT_TRANSFER_TOP_LEFT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_TOP_RIGHT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_LEFT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_BOTTOM_RIGHT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_CENTER_TO_BOEDER:
                                transferXStart = (srcImage.rows - transferXLength) / 2;
                                transferXEnd = transferXStart + transferXLength;
                                transferYStart = (srcImage.cols - transferYLength) / 2;;
                                transferYEnd = transferYStart + transferYLength;
                                break;
                        case RECT_TRANSFER_BORDER_TO_CENTER:
                                transferXStart = transferXLength / 2;
                                transferXEnd = srcImage.rows - transferXStart;
                                transferYStart = transferYLength / 2;;
                                transferYEnd = srcImage.cols - transferYStart;
                                break;
                        case RECT_TRANSFER_LEFT_TO_RIGHT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_RIGHT_TO_LEFT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_TOP_TO_BOTTOM:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_TO_TOP:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        default:
                                return false;
                        }
                        cv::Mat frame;
                        if (RECT_TRANSFER_BORDER_TO_CENTER == transferType) {
                                frame = tempDstImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<uchar>(row, col) = srcImage.at<uchar>(row, col);
                                        }
                                }
                        }
                        else {
                                frame = srcImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<uchar>(row, col) = tempDstImage.at<uchar>(row, col);
                                        }
                                }
                        }
                        resultImageVec.push_back(frame);
                }
        }
        else if (srcImageChannels == 3) {
                for (unsigned int i = 0; i < number; ++i) {
                        transferXLength = stepXLength*(i + 1);
                        transferYLength = stepYLength*(i + 1);
                        switch (transferType) {
                        case RECT_TRANSFER_TOP_LEFT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_TOP_RIGHT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_LEFT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_BOTTOM_RIGHT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_CENTER_TO_BOEDER:
                                transferXStart = (srcImage.rows-transferXLength)/2;
                                transferXEnd = transferXStart + transferXLength;
                                transferYStart = (srcImage.cols - transferYLength) / 2;;
                                transferYEnd = transferYStart + transferYLength;
                                break;
                        case RECT_TRANSFER_BORDER_TO_CENTER:
                                transferXStart = transferXLength / 2;
                                transferXEnd = srcImage.rows - transferXStart;
                                transferYStart = transferYLength / 2;;
                                transferYEnd = srcImage.cols - transferYStart;
                                break;
                        case RECT_TRANSFER_LEFT_TO_RIGHT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_RIGHT_TO_LEFT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_TOP_TO_BOTTOM:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_TO_TOP:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        default:
                                return false;
                        }
                        cv::Mat frame;
                        if (RECT_TRANSFER_BORDER_TO_CENTER == transferType) {
                                frame = tempDstImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<cv::Vec3b>(row, col) = srcImage.at<cv::Vec3b>(row, col);
                                        }
                                }
                        }
                        else {
                                frame = srcImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<cv::Vec3b>(row, col) = tempDstImage.at<cv::Vec3b>(row, col);
                                        }
                                }
                        }
                        resultImageVec.push_back(frame);
                }
        }
        else if (srcImageChannels == 4) {
                for (unsigned int i = 0; i < number; ++i) {
                        transferXLength = stepXLength*(i + 1);
                        transferYLength = stepYLength*(i + 1);
                        switch (transferType) {
                        case RECT_TRANSFER_TOP_LEFT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_TOP_RIGHT_TO:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_LEFT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_BOTTOM_RIGHT_TO:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_CENTER_TO_BOEDER:
                                transferXStart = (srcImage.rows - transferXLength) / 2;
                                transferXEnd = transferXStart + transferXLength;
                                transferYStart = (srcImage.cols - transferYLength) / 2;;
                                transferYEnd = transferYStart + transferYLength;
                                break;
                        case RECT_TRANSFER_BORDER_TO_CENTER:
                                transferXStart = transferXLength / 2;
                                transferXEnd = srcImage.rows - transferXStart;
                                transferYStart = transferYLength / 2;;
                                transferYEnd = srcImage.cols - transferYStart;
                                break;
                        case RECT_TRANSFER_LEFT_TO_RIGHT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = transferYLength;
                                break;
                        case RECT_TRANSFER_RIGHT_TO_LEFT:
                                transferXStart = 0;
                                transferXEnd = srcImage.rows;
                                transferYStart = srcImage.cols - transferYLength;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_TOP_TO_BOTTOM:
                                transferXStart = 0;
                                transferXEnd = transferXLength;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        case RECT_TRANSFER_BOTTOM_TO_TOP:
                                transferXStart = srcImage.rows - transferXLength;
                                transferXEnd = srcImage.rows;
                                transferYStart = 0;
                                transferYEnd = srcImage.cols;
                                break;
                        default:
                                return false;
                        }
                        cv::Mat frame;
                        if (RECT_TRANSFER_BORDER_TO_CENTER == transferType) {
                                frame = tempDstImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<cv::Vec4b>(row, col) = srcImage.at<cv::Vec4b>(row, col);
                                        }
                                }
                        }
                        else {
                                frame = srcImage.clone();
                                for (int row = transferXStart; row < transferXEnd; ++row) {
                                        for (int col = transferYStart; col < transferYEnd; ++col) {
                                                frame.at<cv::Vec4b>(row, col) = tempDstImage.at<cv::Vec4b>(row, col);
                                        }
                                }
                        }
                        resultImageVec.push_back(frame);
                }
        }
        resultImageVec.push_back(tempDstImage);
        return true;
}

/**
* load image effect
* @para Mat srcImage
* @para vector< Nat > resultImagevec
* @para int loadType orientation
* @para int blockHeight
* @para int blockWidth
*/
bool ImageShowEffects::mosaicLoad(const cv::Mat & srcImage, std::vector< cv::Mat > & resultImageVec, const int loadType, const int blockHeight, const int blockWidth) {
        if (srcImage.empty()) {
                std::cout << "Empty image error!" << std::endl;
                return false;
        }
        int srcImageWidth = srcImage.cols;
        int srcImageHeight = srcImage.rows;
        int srcImageChannels = srcImage.channels();
        int srcImageType = srcImage.type();
        cv::Mat initFrame = cv::Mat::zeros(srcImageHeight, srcImageWidth, srcImageType);
        int horizontalBlockNumber = srcImageWidth / blockWidth;
        int verticalBlockNumber = srcImageHeight / blockHeight;
        int XStart(0);
        int XEnd(0);
        int YStart(0);
        int YEnd(0);
        if (srcImageChannels == 1) {
                for (int i = 0; i <= verticalBlockNumber; ++i) {
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                if ((i + j) % 2 == 1) {
                                                        initFrame.at< uchar >(row, col) = 255;
                                                }
                                        }
                                }
                        }
                }
                resultImageVec.push_back(initFrame);
                cv::Mat tempFrame = initFrame.clone();

                switch (loadType) {
                case MOSAIC_LOAD_TOP_TO_BOTTOM:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< uchar >(row, col) = srcImage.at< uchar >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_BOTTOM_TO_TOP:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XEnd = std::min((verticalBlockNumber - i + 1)*blockHeight, srcImageHeight);
                                XStart = XEnd - blockHeight;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< uchar >(row, col) = srcImage.at< uchar >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_LEFT_TO_RIGHT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< uchar >(row, col) = srcImage.at< uchar >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_RIGHT_TO_LEFT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YEnd = std::min((horizontalBlockNumber - j + 1)*blockWidth, srcImageWidth);
                                YStart = YEnd - blockWidth;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< uchar >(row, col) = srcImage.at< uchar >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                default:
                        return false;
                }
        }
        else if (srcImageChannels == 3) {
                for (int i = 0; i <= verticalBlockNumber; ++i) {
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                if ((i + j) % 2 == 1) {
                                                        initFrame.at< cv::Vec3b >(row, col) = cv::Vec3b(255, 255, 255);
                                                }
                                        }
                                }
                        }
                }
                resultImageVec.push_back(initFrame);
                cv::Mat tempFrame = initFrame.clone();

                switch (loadType) {
                case MOSAIC_LOAD_TOP_TO_BOTTOM:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec3b >(row, col) = srcImage.at< cv::Vec3b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_BOTTOM_TO_TOP:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XEnd = std::min((verticalBlockNumber - i + 1)*blockHeight, srcImageHeight);
                                XStart = XEnd - blockHeight;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec3b >(row, col) = srcImage.at< cv::Vec3b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_LEFT_TO_RIGHT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec3b >(row, col) = srcImage.at< cv::Vec3b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_RIGHT_TO_LEFT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YEnd = std::min((horizontalBlockNumber - j + 1)*blockWidth, srcImageWidth);
                                YStart = YEnd - blockWidth;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec3b >(row, col) = srcImage.at< cv::Vec3b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                default:
                        return false;
                }
        }
        else if (srcImageChannels == 4) {
                for (int i = 0; i <= verticalBlockNumber; ++i) {
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                if ((i + j) % 2 == 1) {
                                                        initFrame.at< cv::Vec4b >(row, col) = cv::Vec4b(255, 255, 255, 255);
                                                }
                                        }
                                }
                        }
                }
                resultImageVec.push_back(initFrame);

                cv::Mat tempFrame = initFrame.clone();

                switch (loadType) {
                case MOSAIC_LOAD_TOP_TO_BOTTOM:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XStart = i*blockHeight;
                                XEnd = std::min(XStart + blockHeight, srcImageHeight);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec4b >(row, col) = srcImage.at< cv::Vec4b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_BOTTOM_TO_TOP:
                        YStart = 0;
                        YEnd = srcImageWidth;
                        for (int i = 0; i <= verticalBlockNumber; ++i) {
                                cv::Mat frame = tempFrame.clone();
                                XEnd = std::min((verticalBlockNumber - i + 1)*blockHeight, srcImageHeight);
                                XStart = XEnd - blockHeight;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec4b >(row, col) = srcImage.at< cv::Vec4b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_LEFT_TO_RIGHT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YStart = j*blockWidth;
                                YEnd = std::min(YStart + blockWidth, srcImageWidth);
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec4b >(row, col) = srcImage.at< cv::Vec4b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                case MOSAIC_LOAD_RIGHT_TO_LEFT:
                        XStart = 0;
                        XEnd = srcImageHeight;
                        for (int j = 0; j <= horizontalBlockNumber; ++j) {
                                cv::Mat frame = tempFrame.clone();
                                YEnd = std::min((horizontalBlockNumber - j + 1)*blockWidth, srcImageWidth);
                                YStart = YEnd - blockWidth;
                                for (int row = XStart; row < XEnd; ++row) {
                                        for (int col = YStart; col < YEnd; ++col) {
                                                frame.at< cv::Vec4b >(row, col) = srcImage.at< cv::Vec4b >(row, col);
                                        }
                                }
                                resultImageVec.push_back(frame);
                                tempFrame = frame.clone();
                        }
                        break;
                default:
                        return false;
                }
        }
        return true;
}

/**
* calculate image gradient
* @para Mat srcImage
* @para Mat gradient
*/
bool ImageShowEffects::gradient(const cv::Mat &srcImage, cv::Mat &gradient) {
        cv::Mat tempSrcImage;
        cv::GaussianBlur(srcImage, tempSrcImage, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
        /// Convert it to gray
        cv::Mat graySrcImage;
        cv::cvtColor(tempSrcImage, graySrcImage, CV_RGB2GRAY);
        /// Generate grad_x and grad_y
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y, grad;
        /// Gradient X
        //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
        Sobel(graySrcImage, grad_x, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT );
        convertScaleAbs(grad_x, abs_grad_x);
        /// Gradient Y
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel(graySrcImage, grad_y, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT );
        convertScaleAbs(grad_y, abs_grad_y);
        /// Total Gradient (approximate)
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradient);
        return true;
}


/**
* SLIC super pixel
* @para Mat srcImage
* @para Mat image_pixels_label
* @para int k
* @para int iteration count
* @para int spatial distance weight
*/
bool ImageShowEffects::SLIC(const cv::Mat & srcImage, cv::Mat &image_pixels_label, const int & k, const int & iteration_count, const int & spatial_distance_weight) {
    std::vector< std::vector< int > > k_cluster_center(k, std::vector<int>(5 ,0));
    // step 1
    int imageWidth = srcImage.cols;
    int imageHeight = srcImage.rows;
    int sqrt_k = std::sqrt(k);
    int col_step = imageWidth / sqrt_k;
    int row_step = imageHeight / sqrt_k;
    int row_coordinate = 0;
    for(int i = 0; i < sqrt_k; ++i) {
        int col_coordinate = 0;
        for(int j = 0; j < sqrt_k; ++j) {
            const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row_coordinate, col_coordinate);
            k_cluster_center.at(i*sqrt_k + j).at(0) = pixel[0];
            k_cluster_center.at(i*sqrt_k + j).at(1) = pixel[1];
            k_cluster_center.at(i*sqrt_k + j).at(2) = pixel[2];
            k_cluster_center.at(i*sqrt_k + j).at(3) = col_coordinate;// X
            k_cluster_center.at(i*sqrt_k + j).at(4) = row_coordinate;// Y
            col_coordinate += col_step;
        }
        row_coordinate += row_step;
    }
    /// Deal with remaining clusters successfully
    int remain_cluster_count = 0;
    if((remain_cluster_count = k - sqrt_k * sqrt_k) != 0) {
        // add remaining cluster center
        if(imageWidth <= imageHeight) {
            // add row
            int remain_row_coordinate = ((sqrt_k - 1) * row_step + imageHeight) >> 1;
            int remain_col_step = imageWidth / remain_cluster_count;
            int remain_cluster_index = k - remain_cluster_count;
            int remain_col_coordinate = 0;
            for(int i = 0; i < remain_cluster_count; ++i) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(remain_row_coordinate, remain_col_coordinate);
                std::vector<int> & ith_remain_cluster_center = k_cluster_center.at(remain_cluster_index);
                ith_remain_cluster_center.at(0) = pixel[0];
                ith_remain_cluster_center.at(1) = pixel[1];
                ith_remain_cluster_center.at(2) = pixel[2];
                ith_remain_cluster_center.at(3) = remain_col_coordinate;// X
                ith_remain_cluster_center.at(4) = remain_row_coordinate;// Y
                ++remain_cluster_index;
                remain_col_coordinate += remain_col_step;
            }
        }
        else {
            // add column
            int remain_col_coordinate = ((sqrt_k - 1) * col_step + imageWidth) >> 1;
            int remain_row_step = imageHeight / remain_cluster_count;
            int remain_cluster_index = k - remain_cluster_count;
            int remain_row_coordinate = 0;
            for(int i = 0; i < remain_cluster_count; ++i) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(remain_row_coordinate, remain_col_coordinate);
                std::vector<int> & ith_remain_cluster_center = k_cluster_center.at(remain_cluster_index);
                ith_remain_cluster_center.at(0) = pixel[0];
                ith_remain_cluster_center.at(1) = pixel[1];
                ith_remain_cluster_center.at(2) = pixel[2];
                ith_remain_cluster_center.at(3) = remain_col_coordinate;// X
                ith_remain_cluster_center.at(4) = remain_row_coordinate;// Y
                ++remain_cluster_index;
                remain_row_coordinate += remain_row_step;
            }
        }
    }
    // post-processing for step 1
    cv::Mat image_gradient;
    gradient(srcImage, image_gradient);
    for(int i = 0; i < k_cluster_center.size(); ++i) {
        std::vector<int> & ith_cluster_center = k_cluster_center.at(i);
        int ith_cluster_center_row = ith_cluster_center.at(4);
        int ith_cluster_center_row_start = std::max(0, ith_cluster_center_row - 1);
        int ith_cluster_center_row_end = std::min(ith_cluster_center_row_start + 2, imageHeight - 1);
        int ith_cluster_center_col = ith_cluster_center.at(3);
        int ith_cluster_center_col_start = std::max(0, ith_cluster_center_col-1);
        int ith_cluster_center_col_end = std::min(ith_cluster_center_col_start + 2, imageWidth - 1);
        int min_gradient = image_gradient.at<uchar>(ith_cluster_center_row, ith_cluster_center_col);
        for(int row = ith_cluster_center_row_start; row <= ith_cluster_center_row_end; ++row) {
            for(int col = ith_cluster_center_col_start; col <= ith_cluster_center_col_end; ++col) {
                if(image_gradient.at<uchar>(row, col) < min_gradient) {
                    ith_cluster_center.at(3) = col;
                    ith_cluster_center.at(4) = row;
                    min_gradient = image_gradient.at<uchar>(row, col);
                }
            }
        }
    }
    for(int i = 0; i < k_cluster_center.size(); ++i) {
        std::vector<int> & ith_cluster_center = k_cluster_center.at(i);
        const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(ith_cluster_center.at(4), ith_cluster_center.at(3));
        ith_cluster_center.at(0) = pixel[0];
        ith_cluster_center.at(1) = pixel[1];
        ith_cluster_center.at(2) = pixel[2];
    }

    /// step 2, reassign
    // initialize pixels label
    cv::Mat image_pixels_distance(imageHeight, imageWidth, CV_64FC1, cv::Scalar(1.5e+200));
    /// assignment
    double weight_calculation_constant = (double)spatial_distance_weight * spatial_distance_weight / k;//spatial_distance_weight^2/k
    for(int i = 0; i < iteration_count; ++i) {
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
            int jth_cluster_center_row = jth_cluster_center.at(4);
            int jth_cluster_center_row_start = std::max(0, jth_cluster_center_row - sqrt_k);
            int jth_cluster_center_row_end = std::min(jth_cluster_center_row_start + (sqrt_k << 1) - 1, imageHeight - 1);
            int jth_cluster_center_col = jth_cluster_center.at(3);
            int jth_cluster_center_col_start = std::max(0, jth_cluster_center_col - sqrt_k);
            int jth_cluster_center_col_end = std::min(jth_cluster_center_col_start + (sqrt_k << 1) - 1, imageWidth - 1);
            for(int row = jth_cluster_center_row_start; row <= jth_cluster_center_row_end; ++row) {
                for(int col = jth_cluster_center_col_start; col <= jth_cluster_center_col_end; ++col) {
                    const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                    double color_distance = std::pow((pixel[0] - jth_cluster_center.at(0)), 2) + std::pow((pixel[1] - jth_cluster_center.at(1)), 2) + std::pow((pixel[2] - jth_cluster_center.at(2)), 2);
                    double spatial_distance = std::pow(jth_cluster_center.at(3) - col, 2) + std::pow(jth_cluster_center.at(4)-row, 2);
                    double pixel_distance = std::sqrt(color_distance + spatial_distance * weight_calculation_constant);
                    if(pixel_distance < image_pixels_distance.at<double>(row, col)) {
                        image_pixels_distance.at<double>(row, col) = pixel_distance;
                        image_pixels_label.at<int>(row, col) = j;
                    }
                }
            }
        }
        // deal with unhandled area
        for(int row = 0; row < imageHeight; ++row) {
          for(int col = 0; col < imageWidth; ++col) {
              if(image_pixels_label.at<int>(row, col) == -1) {
                  for(int j = 0; j < k_cluster_center.size(); ++j) {
                      std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
                      const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                      double color_distance = std::pow((pixel[0] - jth_cluster_center.at(0)), 2) + std::pow((pixel[1] - jth_cluster_center.at(1)), 2) + std::pow((pixel[2] - jth_cluster_center.at(2)), 2);
                      double spatial_distance = std::pow(jth_cluster_center.at(3) - col, 2) + std::pow(jth_cluster_center.at(4)-row, 2);
                      double pixel_distance = std::sqrt(color_distance + spatial_distance * weight_calculation_constant);
                      if(pixel_distance < image_pixels_distance.at<double>(row, col)) {
                          image_pixels_distance.at<double>(row, col) = pixel_distance;
                          image_pixels_label.at<int>(row, col) = j;
                      }
                  }
              }
          }
        }
        /// compute new cluster centers
        std::vector<int> k_cluster_count(k_cluster_center.size(), 0);
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            k_cluster_center.at(j) = std::vector<int>(5, 0);
        }
        for(int row = 0; row < imageHeight; ++row) {
            for(int col = 0; col < imageWidth; ++col) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                int label = image_pixels_label.at<int>(row, col);
                std::vector<int> & label_th_cluster_center = k_cluster_center.at(label);
                label_th_cluster_center.at(0) += pixel[0];
                label_th_cluster_center.at(1) += pixel[1];
                label_th_cluster_center.at(2) += pixel[2];
                label_th_cluster_center.at(3) += col;
                label_th_cluster_center.at(4) += row;
                ++k_cluster_count.at(label);
            }
        }
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
            int jth_cluster_count = k_cluster_count.at(j);
            for(int k = 0; k < jth_cluster_center.size(); ++k) {
                jth_cluster_center.at(k) /= jth_cluster_count;
            }
        }
    }
    /// post-processing for step 2
    // using a connected components algorithm

    return true;
}

/**
* ASLIC super pixel
* @para Mat srcImage
* @para Mat image_pixels_label, type is CV_32SC1
* @para int k
* @para int iteration count
*/
bool ImageShowEffects::ASLIC(const cv::Mat & srcImage, cv::Mat &image_pixels_label, const int & k, const int & iteration_count) {
    std::vector< std::vector< int > > k_cluster_center(k, std::vector<int>(5 ,0));
    // step 1
    int imageWidth = srcImage.cols;
    int imageHeight = srcImage.rows;
    int sqrt_k = std::sqrt(k);
    int col_step = imageWidth / sqrt_k;
    int row_step = imageHeight / sqrt_k;
    int row_coordinate = 0;
    for(int i = 0; i < sqrt_k; ++i) {
        int col_coordinate = 0;
        for(int j = 0; j < sqrt_k; ++j) {
            const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row_coordinate, col_coordinate);
            k_cluster_center.at(i*sqrt_k + j).at(0) = pixel[0];
            k_cluster_center.at(i*sqrt_k + j).at(1) = pixel[1];
            k_cluster_center.at(i*sqrt_k + j).at(2) = pixel[2];
            k_cluster_center.at(i*sqrt_k + j).at(3) = col_coordinate;// X
            k_cluster_center.at(i*sqrt_k + j).at(4) = row_coordinate;// Y
            col_coordinate += col_step;
        }
        row_coordinate += row_step;
    }
    /// Deal with remaining clusters successfully
    int remain_cluster_count = 0;
    if((remain_cluster_count = k - sqrt_k * sqrt_k) != 0) {
        // add remaining cluster center
        if(imageWidth <= imageHeight) {
            // add row
            int remain_row_coordinate = ((sqrt_k - 1) * row_step + imageHeight) >> 1;
            int remain_col_step = imageWidth / remain_cluster_count;
            int remain_cluster_index = k - remain_cluster_count;
            int remain_col_coordinate = 0;
            for(int i = 0; i < remain_cluster_count; ++i) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(remain_row_coordinate, remain_col_coordinate);
                std::vector<int> & ith_remain_cluster_center = k_cluster_center.at(remain_cluster_index);
                ith_remain_cluster_center.at(0) = pixel[0];
                ith_remain_cluster_center.at(1) = pixel[1];
                ith_remain_cluster_center.at(2) = pixel[2];
                ith_remain_cluster_center.at(3) = remain_col_coordinate;// X
                ith_remain_cluster_center.at(4) = remain_row_coordinate;// Y
                ++remain_cluster_index;
                remain_col_coordinate += remain_col_step;
            }
        }
        else {
            // add column
            int remain_col_coordinate = ((sqrt_k - 1) * col_step + imageWidth) >> 1;
            int remain_row_step = imageHeight / remain_cluster_count;
            int remain_cluster_index = k - remain_cluster_count;
            int remain_row_coordinate = 0;
            for(int i = 0; i < remain_cluster_count; ++i) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(remain_row_coordinate, remain_col_coordinate);
                std::vector<int> & ith_remain_cluster_center = k_cluster_center.at(remain_cluster_index);
                ith_remain_cluster_center.at(0) = pixel[0];
                ith_remain_cluster_center.at(1) = pixel[1];
                ith_remain_cluster_center.at(2) = pixel[2];
                ith_remain_cluster_center.at(3) = remain_col_coordinate;// X
                ith_remain_cluster_center.at(4) = remain_row_coordinate;// Y
                ++remain_cluster_index;
                remain_row_coordinate += remain_row_step;
            }
        }
    }
    // post-processing for step 1
    cv::Mat image_gradient;
    gradient(srcImage, image_gradient);
    for(int i = 0; i < k_cluster_center.size(); ++i) {
        std::vector<int> & ith_cluster_center = k_cluster_center.at(i);
        int ith_cluster_center_row = ith_cluster_center.at(4);
        int ith_cluster_center_row_start = std::max(0, ith_cluster_center_row - 1);
        int ith_cluster_center_row_end = std::min(ith_cluster_center_row_start + 2, imageHeight - 1);
        int ith_cluster_center_col = ith_cluster_center.at(3);
        int ith_cluster_center_col_start = std::max(0, ith_cluster_center_col-1);
        int ith_cluster_center_col_end = std::min(ith_cluster_center_col_start + 2, imageWidth - 1);
        int min_gradient = image_gradient.at<uchar>(ith_cluster_center_row, ith_cluster_center_col);
        for(int row = ith_cluster_center_row_start; row <= ith_cluster_center_row_end; ++row) {
            for(int col = ith_cluster_center_col_start; col <= ith_cluster_center_col_end; ++col) {
                if(image_gradient.at<uchar>(row, col) < min_gradient) {
                    ith_cluster_center.at(3) = col;
                    ith_cluster_center.at(4) = row;
                    min_gradient = image_gradient.at<uchar>(row, col);
                }
            }
        }
    }
    for(int i = 0; i < k_cluster_center.size(); ++i) {
        std::vector<int> & ith_cluster_center = k_cluster_center.at(i);
        const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(ith_cluster_center.at(4), ith_cluster_center.at(3));
        ith_cluster_center.at(0) = pixel[0];
        ith_cluster_center.at(1) = pixel[1];
        ith_cluster_center.at(2) = pixel[2];
    }

    /// step 2, reassign
    // initialize pixels label
    cv::Mat image_pixels_distance(imageHeight, imageWidth, CV_64FC1, cv::Scalar(1.5e+200));
    /// assignment
    std::vector<double> k_mc_vector(k, 100.0), k_ms_vector(k, (double)imageWidth * imageHeight / k);
    std::vector<double> k_weight_calculation_factor_vector(k, 0.0);
    for(int i = 0; i < iteration_count; ++i) {
        // re-calculate weight_calculation_factor
        for(int j = 0; j < k; ++j) {
            k_weight_calculation_factor_vector.at(j) = k_mc_vector.at(j) / k_ms_vector.at(j);
            k_mc_vector.at(j) = k_ms_vector.at(j) = 0.0;
        }
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
            int jth_cluster_center_row = jth_cluster_center.at(4);
            int jth_cluster_center_row_start = std::max(0, jth_cluster_center_row - sqrt_k);
            int jth_cluster_center_row_end = std::min(jth_cluster_center_row_start + (sqrt_k << 1) - 1, imageHeight - 1);
            int jth_cluster_center_col = jth_cluster_center.at(3);
            int jth_cluster_center_col_start = std::max(0, jth_cluster_center_col - sqrt_k);
            int jth_cluster_center_col_end = std::min(jth_cluster_center_col_start + (sqrt_k << 1) - 1, imageWidth - 1);
            for(int row = jth_cluster_center_row_start; row <= jth_cluster_center_row_end; ++row) {
                for(int col = jth_cluster_center_col_start; col <= jth_cluster_center_col_end; ++col) {
                    const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                    double color_distance = std::pow((pixel[0] - jth_cluster_center.at(0)), 2) + std::pow((pixel[1] - jth_cluster_center.at(1)), 2) + std::pow((pixel[2] - jth_cluster_center.at(2)), 2);
                    if(k_mc_vector.at(j) < color_distance) {
                        k_mc_vector.at(j) = color_distance;
                    }
                    double spatial_distance = std::pow(jth_cluster_center.at(3) - col, 2) + std::pow(jth_cluster_center.at(4)-row, 2);
                    if(k_ms_vector.at(j) < spatial_distance) {
                        k_ms_vector.at(j) = spatial_distance;
                    }
                    double pixel_distance = std::sqrt(color_distance + spatial_distance * k_weight_calculation_factor_vector.at(j));
                    if(pixel_distance < image_pixels_distance.at<double>(row, col)) {
                        image_pixels_distance.at<double>(row, col) = pixel_distance;
                        image_pixels_label.at<int>(row, col) = j;
                    }
                }
            }
        }
        // deal with unhandled area
        for(int row = 0; row < imageHeight; ++row) {
          for(int col = 0; col < imageWidth; ++col) {
              if(image_pixels_label.at<int>(row, col) == -1) {
                  for(int j = 0; j < k_cluster_center.size(); ++j) {
                      std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
                      const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                      double color_distance = std::pow((pixel[0] - jth_cluster_center.at(0)), 2) + std::pow((pixel[1] - jth_cluster_center.at(1)), 2) + std::pow((pixel[2] - jth_cluster_center.at(2)), 2);
                      double spatial_distance = std::pow(jth_cluster_center.at(3) - col, 2) + std::pow(jth_cluster_center.at(4)-row, 2);
                      double pixel_distance = std::sqrt(color_distance + spatial_distance * k_weight_calculation_factor_vector.at(j));
                      if(pixel_distance < image_pixels_distance.at<double>(row, col)) {
                          image_pixels_distance.at<double>(row, col) = pixel_distance;
                          image_pixels_label.at<int>(row, col) = j;
                      }
                  }
                  int index = image_pixels_label.at<int>(row, col);
                  std::vector<int> &index_th_cluster_center = k_cluster_center.at(index);
                  const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                  double color_distance = std::pow((pixel[0] - index_th_cluster_center.at(0)), 2) + std::pow((pixel[1] - index_th_cluster_center.at(1)), 2) + std::pow((pixel[2] - index_th_cluster_center.at(2)), 2);
                  if(k_mc_vector.at(index) < color_distance) {
                      k_mc_vector.at(index) = color_distance;
                  }
                  double spatial_distance = std::pow(index_th_cluster_center.at(3) - col, 2) + std::pow(index_th_cluster_center.at(4)-row, 2);
                  if(k_ms_vector.at(index) < spatial_distance) {
                      k_ms_vector.at(index) = spatial_distance;
                  }
              }
          }
        }
        /// compute new cluster centers
        std::vector<int> k_cluster_count(k_cluster_center.size(), 0);
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            k_cluster_center.at(j) = std::vector<int>(5, 0);
        }
        for(int row = 0; row < imageHeight; ++row) {
            for(int col = 0; col < imageWidth; ++col) {
                const cv::Vec3b &pixel = srcImage.at<cv::Vec3b>(row, col);
                int label = image_pixels_label.at<int>(row, col);
                std::vector<int> & label_th_cluster_center = k_cluster_center.at(label);
                label_th_cluster_center.at(0) += pixel[0];
                label_th_cluster_center.at(1) += pixel[1];
                label_th_cluster_center.at(2) += pixel[2];
                label_th_cluster_center.at(3) += col;
                label_th_cluster_center.at(4) += row;
                ++k_cluster_count.at(label);
            }
        }
        for(int j = 0; j < k_cluster_center.size(); ++j) {
            std::vector<int> &jth_cluster_center = k_cluster_center.at(j);
            int jth_cluster_count = k_cluster_count.at(j);
            for(int k = 0; k < jth_cluster_center.size(); ++k) {
                jth_cluster_center.at(k) /= jth_cluster_count;
            }
        }
    }
    /// post-processing for step 2
    // using a connected components algorithm

    return true;
}
