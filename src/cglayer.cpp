/* ****************************************************************
 * FilePath     : /src/cglayer.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:07
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:29:06
 * Description  : layer information
 * Revision     : v1.0 - first release
 *                v2.0 - Add function to call tlg2png to deal with
 *                  tlg files extracted by KrkrExtract.
 *                  Modify the rule for generating fileName_ to
 *                  match files processed by tlg2png.
 *                v3.0 - add fuctions using OpenCV, read image
 *                  files and pad to target size.
 *                v3.2 - modify to optimize appending layer
 *                v3.3 - add support for negative left/top
 * **************************************************************** */

#include "cglayer.h"

#include <string>
//#include <iostream>
#include "opencv2/opencv.hpp"

#include "tlg2png/Image.h"
#include "tlg2png/TlgConverter.h"

// No longer used after v3.2
/*
// CGLayer constructor, get layer infomation from parameters.
// set info variables and call tlg2png to extract png image
// get the image into Mat
// @param 1 - seriesName: layer series name
// @param 2 - layerid: layer id, to find tlg file
// @param 3 - width: target width of the layer
// @param 4 - height: target height of the layer
// @param 5 - left: column position when append layer on background
// @param 6 - top: row position when append layer on background
CGLayer::CGLayer(const std::string &seriesName, int layerid, int width,
                 int height, int left, int top) {
    seriesName_ = seriesName;
    layerid_ = layerid;
    width_ = width;
    height_ = height;
    left_ = left;
    top_ = top;

    // v1.0
    // fileName_ = seriesName_ + "+pimg+" + std::to_string(layerid_) + ".tlg.png";

    // v2.0
    fileName_ = seriesName_ + "/" + std::to_string(layerid_) + ".png";

    tlg2png();

    readImg();
}
*/

// CGLayer constructor, get layer infomation from parameters.
// set info variables and call tlg2png to extract png image
// get the image into Mat
// @param 1 - seriesName: layer series name
// @param 2 - layerid: layer id, to find tlg file
// @param 3 - width: target width of the layer
// @param 4 - height: target height of the layer
// @param 5 - left: column position when append layer on background
// @param 6 - top: row position when append layer on background
CGLayer::CGLayer(const std::string &seriesName, int layerid, int left, int top) {
    seriesName_ = seriesName;
    layerid_ = layerid;
    left_ = left;
    top_ = top;

    // v1.0
    // fileName_ = seriesName_ + "+pimg+" + std::to_string(layerid_) + ".tlg.png";

    // v2.0
    fileName_ = seriesName_ + "/" + std::to_string(layerid_) + ".png";

    tlg2png();

    readImg();
}

CGLayer::~CGLayer() {
}

// return column position when append layer on background
int CGLayer::left() {
    return left_;
};

// return row position when append layer on background
int CGLayer::top() {
    return top_;
};

// return png file path of the layer
std::string CGLayer::fileName() {
    return fileName_;
}

// find tlg file and extract to png
void CGLayer::tlg2png() {
    std::string tlgFilePath = seriesName_ + "/" + std::to_string(layerid_) + ".tlg";
    TlgConverter converter;
    Image image = converter.read(tlgFilePath);
    converter.save(image, fileName_);
}

// get cv::Mat from image file
void CGLayer::readImg() {
    img_ = cv::imread(fileName_, cv::IMREAD_UNCHANGED);

    // No longer used after v3.2
    /*
    cv::Scalar padPixel(0, 0, 0, 0);
    int padTop = top_;
    int padBottom = height_ - img_.rows - top_;
    int padLeft = left_;
    int padRight = width_ - img_.cols - left_;
    cv::copyMakeBorder(img_, img_, padTop, padBottom, padLeft, padRight, cv::BORDER_CONSTANT, padPixel);
    */

    // negative left or top
    // appear in amairo isle, ev414a
    if ((left_ < 0) && (top_ < 0)) {
        cv::Mat partImg_(img_, cv::Rect(-left_, -top_, img_.cols + left_, img_.rows + top_));
        cv::Mat tmpMat = partImg_.clone();
        img_ = tmpMat;
        left_ = 0;
        top_ = 0;
    }
    else if ((left_ < 0) && (top_ >= 0)) {
        cv::Mat partImg_(img_, cv::Rect(-left_, 0, img_.cols + left_, img_.rows));
        cv::Mat tmpMat = partImg_.clone();
        img_ = tmpMat;
        left_ = 0;
    }
    else if ((left_ >= 0) && (top_ < 0)) {
        cv::Mat partImg_(img_, cv::Rect(0, -top_, img_.cols, img_.rows + top_));
        cv::Mat tmpMat = partImg_.clone();
        img_ = tmpMat;
        top_ = 0;
    }

}

// return the readed image in cv::Mat format
cv::Mat CGLayer::img() {
    return img_;
}
