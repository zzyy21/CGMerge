/* ****************************************************************
 * FilePath     : /src/cglayer.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:07
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-06 00:15:43
 * Description  : layer information
 * Revision     : v1.0 - first release
 *                v2.0 - Add function to call tlg2png to deal with
 *                  tlg files extracted by KrkrExtract.
 *                  Modify the rule for generating fileName_ to
 *                  match files processed by tlg2png.
 * **************************************************************** */

#include "cglayer.h"

#include <string>
//#include <iostream>

#include "tlg2png/Image.h"
#include "tlg2png/TlgConverter.h"

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
}

CGLayer::~CGLayer() {
}

int CGLayer::left() {
    return left_;
};

int CGLayer::top() {
    return top_;
};

std::string CGLayer::fileName() {
    return fileName_;
}

void CGLayer::tlg2png() {
    std::string tlgFilePath = seriesName_ + "/" + std::to_string(layerid_) + ".tlg";
    TlgConverter converter;
    Image image = converter.read(tlgFilePath);
    converter.save(image, fileName_);
}
