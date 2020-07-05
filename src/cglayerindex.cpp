/* ****************************************************************
 * FilePath     : /src/cglayerindex.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-23 20:26:07
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-05 23:59:50
 * Description  : layer index operation
 * Revision     : v1.0 - Get layer info from txt file by expimg
 *                v2.0 - Get layer info from json file by KrkrExtract
 * **************************************************************** */

#include "cglayerindex.h"

#include <vector>
#include <string>
#include <fstream>

#include "cglayer.h"
#include "json.hpp"

// Used to extract values from txt format lines
// No longer used after v2.0 due to the use of json layer info
/*
int CGLayerIndex::getValue(const std::string &line) {
    std::string tmpString;
    tmpString = line.substr(line.find(':') + 1);
    return stoi(tmpString);
}
*/

void CGLayerIndex::getPicId(const std::string &line, int* p_bgLayer, int* p_upLayer) {
    int infoStart = line.rfind(' ');
    char tmpChar;

    tmpChar = line.at(infoStart + 1);
    if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_bgLayer = tmpChar - 'A';
    }
    else if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_bgLayer = tmpChar - 'a';
    }

    tmpChar = line.at(infoStart + 2);
    if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_upLayer = tmpChar - 'A';
    }
    else if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_upLayer = tmpChar - 'a';
    }
}

// Used to get infos from txt file
// No longer used due after v2.0 to the use of json layer info
/*
int CGLayerIndex::getInfoTxt() {
    std::string lineBuff;
    std::ifstream txtFile;
    std::string txtFileName = seriesName_ + "+pimg+layers.txt";
    txtFile.open(txtFileName, std::ios::in|std::ios::binary);

    // width * height
    std::getline(txtFile, lineBuff);
    imageWidth_ = getValue(lineBuff);
    std::getline(txtFile, lineBuff);
    imageHeight_ = getValue(lineBuff);
    std::getline(txtFile, lineBuff);

    // get layers info
    int layerNum = 0;
    while (std::getline(txtFile, lineBuff)) {
        // name
        int bgLayer;
        int upLayer;
        getPicId(lineBuff, &bgLayer, &upLayer);

        // layer_id
        std::getline(txtFile, lineBuff);
        int layerid = getValue(lineBuff);

        // width
        std::getline(txtFile, lineBuff);

        // height
        std::getline(txtFile, lineBuff);

        // left
        std::getline(txtFile, lineBuff);
        int left = getValue(lineBuff);

        // top
        std::getline(txtFile, lineBuff);
        int top = getValue(lineBuff);

        // opacity
        std::getline(txtFile, lineBuff);

        // layer_type
        std::getline(txtFile, lineBuff);

        // type
        std::getline(txtFile, lineBuff);

        // visible
        std::getline(txtFile, lineBuff);

        // blank line
        std::getline(txtFile, lineBuff);

        CGLayer cglayer(seriesName_, layerid, left, top);
        layers_.push_back(cglayer);
        layerIndex_[bgLayer * 26 + upLayer] = layerNum;
        availableIndex_.push_back(bgLayer * 26 + upLayer);
        layerNum++;
    }

    txtFile.close();

    return layerNum;
}
*/

int CGLayerIndex::getInfoJson() {
    std::string jsonFileName = seriesName_ + ".json";
    std::ifstream jsonFile(jsonFileName, std::ios::in|std::ios::binary);

    nlohmann::json jsonLayerIndex;
    jsonFile >> jsonLayerIndex;

    imageWidth_ = jsonLayerIndex["width"];
    imageHeight_ = jsonLayerIndex["height"];

    int totalLayers = jsonLayerIndex["layers"].size();
    std::string layerName;
    int bgLayer;
    int upLayer;
    int layerid;
    int left;
    int top;
    for (int layerNum = 0; layerNum < totalLayers; layerNum++) {
        layerName = jsonLayerIndex["layers"][layerNum]["name"];
        getPicId(layerName, &bgLayer, &upLayer);

        layerid = jsonLayerIndex["layers"][layerNum]["layer_id"];

        left = jsonLayerIndex["layers"][layerNum]["left"];
        top = jsonLayerIndex["layers"][layerNum]["top"];

        CGLayer cglayer(seriesName_, layerid, left, top);
        layers_.push_back(cglayer);
        layerIndex_[bgLayer * 26 + upLayer] = layerNum;
        availableIndex_.push_back(bgLayer * 26 + upLayer);
    }

    jsonFile.close();

    return totalLayers;
}

CGLayerIndex::CGLayerIndex() {
    seriesName_ = "dummy";
}

CGLayerIndex::CGLayerIndex(const std::string &seriesName) {
    for (int i = 0; i < 26 * 26; i++) {
        layerIndex_[i] = -1;
    }

    seriesName_ = seriesName;

    layerNum_ = getInfoJson();
}

CGLayerIndex::~CGLayerIndex() {
}

std::string CGLayerIndex::seriesName() {
    return seriesName_;
};

int CGLayerIndex::layerNum() {
    return(layerNum_);
}

int CGLayerIndex::imageWidth() {
    return imageWidth_;
};

int CGLayerIndex::imageHeight() {
    return imageHeight_;
};

CGLayer CGLayerIndex::findLayer(int bgLayer, int upLayer) {
    // TODO: if layer exist?
    return layers_[layerIndex_[bgLayer * 26 + upLayer] ];
}

CGLayer CGLayerIndex::findLayer(int availableIndexNo) {
    // TODO: if layer exist?
    return layers_[layerIndex_[availableIndex_[availableIndexNo] ] ];
}
