/* ****************************************************************
 * FilePath     : /src/cgpic.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 15:06:55
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-24 15:06:55
 * Description  : operation to single cg picture
 * Revision     : 
 * **************************************************************** */

#include "cglayer.h"
#include "cgpic.h"

#include <vector>

CGPic::CGPic() {
}

CGPic::~CGPic() {
}

void CGPic::setSize(int width, int height) {
    width_ = width;
    height_ = height;
}

void CGPic::addLayer(const CGLayer &layer) {
    layers_.push_back(layer);
}

void CGPic::setFileName(const std::string &fileName) {
    fileName_ = fileName;
}

std::string CGPic::magickMergeScript() {
    std::string commandLine = "magick convert -size ";
    commandLine += std::to_string(width_) + "x" + std::to_string(height_) + " ";

    for (int i = layers_.size() - 1; i >= 0; i--) {
        commandLine += "-page ";
        commandLine += "+" + std::to_string(layers_[i].left());
        commandLine += "+" + std::to_string(layers_[i].top()) + " ";
        commandLine += layers_[i].fileName() + " ";
    }

    commandLine += "-quality 95 -mosaic CGOutput\\" + fileName_;

    return commandLine;
}
