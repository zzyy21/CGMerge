/* ****************************************************************
 * FilePath     : /src/cgpic.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 15:06:55
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-06 18:26:55
 * Description  : operation to single cg picture
 * Revision     : v1.0 - first version for generate Magick convert
 *                  command-lines
 *                v1.0(200705) - add png:include-chunk definition
 *                v1.0(200706) - modify quality parameter setting
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
