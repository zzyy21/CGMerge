/* ****************************************************************
 * FilePath     : /src/cgpic.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 15:06:55
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 17:31:30
 * Description  : operation to single cg picture
 * Revision     : v1.0 - first version for generate Magick convert
 *                  command-lines
 *                v1.0(200705) - add png:include-chunk definition
 *                v1.0(200706) - modify quality parameter setting
 *                v3.0 - add fuctions using OpenCV, merge image
 *                  layers and save image file.
 * **************************************************************** */

#include "cglayer.h"
#include "cgpic.h"

#include <vector>

CGPic::CGPic() {
}

CGPic::~CGPic() {
}

// append 4 channel upper layer on 3 channel background 
// layer in same size. using opencv library.
// @param 1 - up: upper layer (4 channels)
// @param 2 - p_bg: pointer to the background layer, also the result
void CGPic::imgAppendLayer(const cv::Mat& up, cv::Mat* p_bg) {
    std::vector<cv::Mat> srcChannels;
    std::vector<cv::Mat> outChannels;
    split(up, srcChannels);
    split(*p_bg, outChannels);

    for (int i = 0; i < 3; i++) {
        // out_RGB <--- dst_RGB(1 - src_A)
        outChannels[i] = outChannels[i].mul(255.0 - srcChannels[3], 1 / 255.0);
        // out_RGB <--- src_RGBsrc_A + dst_RGB(1 - src_A)
        outChannels[i] += srcChannels[i].mul(srcChannels[3], 1 / 255.0);
    }

    cv::merge(outChannels, *p_bg);
}

// set picture size
// @param 1 - width: width of the pic
// @param 2 - height: height of the pic
void CGPic::setSize(int width, int height) {
    width_ = width;
    height_ = height;
}

// append a layer to the top of picture
// @param 1 - layer: layer to be added
void CGPic::addLayer(const CGLayer &layer) {
    layers_.push_back(layer);
}

// set the output file name of the picture
// @param 1 - fileName: name to be set
void CGPic::setFileName(const std::string &fileName) {
    fileName_ = fileName;
}

// No longer used after v3.0 due to the use of OpenCV
/*
// return the command-line script to merge the picture using Magick
std::string CGPic::magickMergeScript() {
    std::string commandLine = "magick convert -size ";
    commandLine += std::to_string(width_) + "x" + std::to_string(height_) + " ";

    for (int i = layers_.size() - 1; i >= 0; i--) {
        commandLine += "-page ";
        commandLine += "+" + std::to_string(layers_[i].left());
        commandLine += "+" + std::to_string(layers_[i].top()) + " ";
        commandLine += "\"" + layers_[i].fileName() + "\" ";
    }

    // Set Magick png compression level, maximum compression
    // * For the MNG and PNG image formats, the quality value sets the zlib
    // * compression level (quality / 10) and filter-type (quality % 10).
    // * The default PNG "quality" is 75, which means compression level 7
    // * with adaptive PNG filtering, unless the image has a color map,
    // * in which case it means compression level 7 with no PNG filtering.
    // * If filter-type is 5, adaptive filtering is used when quality is
    // * greater than 50 and the image does not have a color map,
    // * otherwise no filtering is used.
    // * For compression level 0 (quality value less than 10),
    // * the Huffman-only strategy is used, which is fastest
    // * but not necessarily the worst compression.
    // *** v1.0 use 95 for highest compression
    // *** v1.0 2020-07-06 use 05 for fastest processing, as the highest
    // ***      compression is slow but does not obviously reduce the size
    commandLine += "-quality 05 ";
    // Define ancillary chunks in PNG. Avoid writing time information to cause
    // different verification of merged image files.
    // * Ancillary chunks to be excluded from or included in PNG output.
    // * The value can be the name of a PNG chunk-type such as bKGD,
    // * a comma-separated list of chunk-names (which can include the word date,
    // * the word all, or the word none). Although PNG chunk-names are
    // * case-dependent, you can use all lowercase names if you prefer.
    commandLine += "-define png:include-chunk=none ";

    commandLine += "-mosaic CGOutput\\" + fileName_;

    return commandLine;
}
*/

void CGPic::saveImage() {
    int layerNum = layers_.size();
    cv::Mat img;
    // background layer in 3 channels
    cv::cvtColor(layers_[layerNum - 1].img(), img, cv::COLOR_BGRA2BGR);

    for (int i = layerNum - 2; i >= 0; i--) {
        imgAppendLayer(layers_[i].img(), &img);
    }

    std::string outputPath = "CGOutput\\" + fileName_;
    // huffman only compression strategy
    // fastest, acceptable file size
    std::vector<int> compressionParams = {cv::IMWRITE_PNG_STRATEGY, cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY};

    cv::imwrite(outputPath, img, compressionParams);
}
