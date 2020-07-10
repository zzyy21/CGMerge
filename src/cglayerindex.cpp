/* ****************************************************************
 * FilePath     : /src/cglayerindex.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-23 20:26:07
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:30:35
 * Description  : layer index operation
 * Revision     : v1.0 - Get layer info from txt file by expimg
 *                v2.0 - Get layer info from json file by KrkrExtract
 *                v3.0 - Modify function call to fit the change of
 *                  using OpenCV
 *                v3.2 - modify to optimize appending layer
 *                v3.3 - use new layer identify rule
 * **************************************************************** */

#include "cglayerindex.h"

#include <vector>
#include <string>
#include <fstream>

#include "cglayer.h"
#include "json.hpp"

// No longer used after v2.0 due to the use of json layer info
/*
// return the value get from txt format lines
// @param 1 - line: input line with format:"xxx:   <value>"
int CGLayerIndex::getValue(const std::string &line) {
    std::string tmpString;
    tmpString = line.substr(line.find(':') + 1);
    return stoi(tmpString);
}
*/

// No longer use after v3.3, use getLayerId istead
/*
// get pictrue background and upper layer identifier from input line
// @param 1 - line: input line with format:"name   :<Aa>"
// @param 2 - p_bgLayer: pointer to store background identifier
// @param 3 - p_upLayer: pointer to upper layer identifier
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
*/

// turn all upper case char to lower case in input string
// @param 1 - inString: pointer to string to be handle
void CGLayerIndex::stringToLowercase(std::string *inString) {
    for (size_t i = 0; i < (*inString).size(); i++) {
        char tmpChar = (*inString).at(i);
        if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
            (*inString)[i] = tmpChar - 'A' + 'a';
        }
    }
}

// get v3.3 layer identifier from input layer name
// @param 1 - layerName: input string layer name (lower case only)
// @param 2 - p_bgLayer: pointer to store background identifier
// @param 3 - p_upLayer: pointer to upper layer identifier
void CGLayerIndex::getLayerId(const std::string &layerName, int* p_bgLayer, int* p_upLayer) {
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //  simple two char:            "<Aa>"
    //      getLayerNum(<AA>),      (0, 0)
    //  single char layer:          "<A>"
    //  appear in cafe stella
    //      getLayerNum(a<A>),      (0, 0)      (bg is always 0))
    //  three char start with M:    "M<AA>"
    //  appear in sanoba witch
    //      getLayerNum(<AA>),      (0, 0)
    //      "M",                    bg += 26
    //                              (26, 0)
    //  string end with " \u306e\u30b3\u30d4\u30fc": 
    //                              "<AA> \u306e\u30b3\u30d4\u30fc"
    //  appear in amairo isle
    //      getLayerNum(<AA>),      (0, 0)
    //      " のコピー",             bg += 26
    //                              (26, 0)
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    int length = layerName.size();
    if (length == 2) {
        *p_bgLayer = layerName.at(0) - 'a';
        *p_upLayer = layerName.at(1) - 'a';
    }
    else if (length == 1) {
        *p_bgLayer = 0;
        *p_upLayer = layerName.at(0) - 'a';
    }
    else if (length == 3) {
        *p_bgLayer = layerName.at(1) - 'a' + 26;
        *p_upLayer = layerName.at(2) - 'a';
    }
    else if (length == 15) {
        *p_bgLayer = layerName.at(0) - 'a' + 26;
        *p_upLayer = layerName.at(1) - 'a';
    }
}

// No longer used due after v2.0 to the use of json layer info
/*
// get series layer infos from txt file extracted by expimg
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

// get series layer infos from json file extracted by KrkrExtraxct
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
        stringToLowercase(&layerName);
        getLayerId(layerName, &bgLayer, &upLayer);

        layerid = jsonLayerIndex["layers"][layerNum]["layer_id"];

        left = jsonLayerIndex["layers"][layerNum]["left"];
        top = jsonLayerIndex["layers"][layerNum]["top"];

        // No longer used after v3.2
        //CGLayer cglayer(seriesName_, layerid, imageWidth_, imageHeight_, left, top);
        CGLayer cglayer(seriesName_, layerid, left, top);
        layers_.push_back(cglayer);
        layerIndex_[bgLayer * 26 + upLayer] = layerNum;
        availableIndex_.push_back(bgLayer * 26 + upLayer);
    }

    // if no layer "aa", set the last layer as "aa"
    // appear in amairo isle, patch.xp3 ev214n
    if (layerIndex_[0] == -1) {
        CGLayer cglayer(seriesName_, layerid, left, top);
        layers_.push_back(cglayer);
        layerIndex_[0] = totalLayers;
        availableIndex_.push_back(0);
        totalLayers++;
    }

    jsonFile.close();

    return totalLayers;
}

// default constructor of CGLayerIndex,
// create object with seriesName_ = "dummy"
CGLayerIndex::CGLayerIndex() {
    seriesName_ = "dummy";
}

// CGLayerIndex constructor,
// create index object with series name in parameter,
// and read json file to get layer information
// @param 1 - seriesName: series name of this index
CGLayerIndex::CGLayerIndex(const std::string &seriesName) {
    for (int i = 0; i < 2 * 26 * 26; i++) {
        layerIndex_[i] = -1;
    }

    seriesName_ = seriesName;

    layerNum_ = getInfoJson();
}

CGLayerIndex::~CGLayerIndex() {
}

// return series name of the index
std::string CGLayerIndex::seriesName() {
    return seriesName_;
};

// return total layer number in the index
int CGLayerIndex::layerNum() {
    return(layerNum_);
}

// return image width of the cg series
int CGLayerIndex::imageWidth() {
    return imageWidth_;
};

// return image height of the cg series
int CGLayerIndex::imageHeight() {
    return imageHeight_;
};

// No longer use after v3.3 due to the new layer id
/*
// return CGLayer by background and upper layer identifier
// @param 1 - bgLayer: background identifier
// @param 2 - upLayer: upper layer identifier
CGLayer CGLayerIndex::findLayer(int bgLayer, int upLayer) {
    // TODO: if layer exist?
    return layers_[layerIndex_[bgLayer * 26 + upLayer] ];
}

// return CGLayer by position number in index.
// @param 1 - availableIndexNo: index No
CGLayer CGLayerIndex::findLayer(int availableIndexNo) {
    // TODO: if layer exist?
    return layers_[layerIndex_[availableIndex_[availableIndexNo] ] ];
}
*/

// return CGLayer by background and upper layer identifier
// @param 1 - layerId: layer to get.
CGLayer CGLayerIndex::findLayer(int layerId) {
    // TODO: if layer exist?
    return layers_[layerIndex_[layerId] ];
}

