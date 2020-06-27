/* ****************************************************************
 * FilePath     : /src/cglayer.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:07
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-22 22:32:07
 * Description  : layer information
 * Revision     : 
 * **************************************************************** */

#include "cglayer.h"

#include <string>
//#include <iostream>

CGLayer::CGLayer(const std::string &seriesName, int layerid, int left, int top) {
    seriesName_ = seriesName;
    layerid_ = layerid;
    left_ = left;
    top_ = top;
    fileName_ = seriesName_ + "+pimg+" + std::to_string(layerid_) + ".tlg.png";
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
