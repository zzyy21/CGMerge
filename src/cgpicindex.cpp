/* ****************************************************************
 * FilePath     : /src/cgpicindex.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-07-10 19:31:33
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:43:42
 * Description  : functions to handle imagediffmap.csv file
 *                build a index for cgpic.
 * Revision     : v3.3 - new file for class CSVimagediffmapSplitter
 *                  imagediffmap.csv file handling.
 *                  get layer identify id from imagediffmap.csv.
 *                  use new layer identify rule.
 * **************************************************************** */

#include <fstream>
#include <string>
#include <vector>

#include "cgpicindex.h"

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//  pic id string,  series name,    seton,  layer name  (:background)
//  ev101aa,        ev101a,         seton,  Aa
//  ev101ab,        ev101a,         seton,  Ab           :Aa
//  ev118_b,        ev118_a,        diff,   B
//  EV215MAA,       EV215A,         diff,   MAA
//  EV607JA,        EV607A,         diff,   AA のコピー
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// CSVimagediffmapSplitter constructor.
// open "imagediffmap.csv" and get the first line to lineBuff_
CSVimagediffmapSplitter::CSVimagediffmapSplitter() {
    imagediffmap_.open("imagediffmap.csv", std::ios::in|std::ios::binary);

    // get the first uncomment line
    while (std::getline(imagediffmap_, lineBuff_)) {
        char tmpChar = lineBuff_.at(0);
        if (tmpChar == '#') {
            continue;
        }
        else if ((tmpChar == '\r') || (tmpChar == '\n')) {
            continue;
        }
        else {
            break;
        }
    }

    while (getNewGroup()) {}

    imagediffmap_.close();
}

CSVimagediffmapSplitter::~CSVimagediffmapSplitter() {
}

// get layer identifier from input line with two char
// @param 1 - line: input line with format:"<Aa>"
// @param 2 - p_bgLayer: pointer to store background identifier
// @param 3 - p_upLayer: pointer to upper layer identifier
void CSVimagediffmapSplitter::getLayerNum(const std::string &layerNumString, int* p_bgLayer, int* p_upLayer) {
    char tmpChar;
    tmpChar = layerNumString.at(0);
    if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_bgLayer = tmpChar - 'a';
    }
    else if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_bgLayer = tmpChar - 'A';
    }

    tmpChar = layerNumString.at(1);
    if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_upLayer = tmpChar - 'a';
    }
    else if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_upLayer = tmpChar - 'A';
    }
}

// get v3.3 layer identifier from input layer name
// @param 1 - layerName: input string layer name (lower case only)
// @param 2 - p_bgLayer: pointer to store background identifier
// @param 3 - p_upLayer: pointer to upper layer identifier
void CSVimagediffmapSplitter::layerIdcode(const std::string &layerName, int* p_bgLayer, int* p_upLayer) {
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
    //  string end with " のコピー": "<AA> のコピー"
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
    else if (length == 11) {
        *p_bgLayer = layerName.at(0) - 'a' + 26;
        *p_upLayer = layerName.at(1) - 'a';
    }
}

// get index id from input pic id string
// @param 1 - idString: input pic id string
// @param 2 - p_seriesId: pointer to series identifier
// @param 3 - p_bgLayer: pointer to store background identifier
// @param 4 - p_upLayer: pointer to store upper layer identifier
void CSVimagediffmapSplitter::getIndexId(const std::string &idString, int* p_seriesId, int* p_bgLayer, int* p_upLayer) {
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //  idString --> seriesId:
    //      _ series    --> seriesId += 2
    //      m series    --> seriesId += 1
    //  example:
    //      cgInfo       groupName   idString    seriesId    layerId
    //      ev101bc      ev101       aa          0           (1, 2)
    //      ev202mad     ev202       mad         1           (0, 3)
    //      ev304_aa     ev304       _aa         2           (0, 0)
    //      ev404_mad    ev404       _mad        3           (0, 3)
    //      ev502_B      ev502       _B          2           (0, 1)
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    int isUnderlineSeries = 0;
    int isMSeries = 0;
    char tmpChar = idString.at(0);
    std::string tmpString;
    if (tmpChar == '_') {
        isUnderlineSeries = 1;
        tmpString = idString.substr(1);
    }
    else {
        tmpString = idString;
    }

    int length = tmpString.size();
    // mAa
    if (length == 3) {
        isMSeries = 1;
        tmpString = tmpString.substr(1);
    }
    // A
    else if (length == 1) {
        tmpString = "a" + tmpString;
    }

    getLayerNum(tmpString, p_bgLayer, p_upLayer);

    *p_seriesId = isUnderlineSeries * 2 + isMSeries;
}

// split a line from imagediffmap.csv to get cg series info
// @param 1 - line: line in imagediffmap.csv
// example: "ev112mbc,ev112mm,seton,Bc:Ba" -->
//          series name of ("ev112m", 1, 2) is "ev112mm"
// example: "ev701bb,ev701b,seton,Bb:Ba" -->
//          series name of ("ev701", 1, 1) is "ev701b"
void CSVimagediffmapSplitter::lineSplitt(const std::string &line) {
    std::string tmpLine = line;
    size_t splitPosition;

    splitPosition = tmpLine.find(',');
    std::string idString = tmpLine.substr(5, splitPosition - 5);
    int seriesId;
    int bgLayer;
    int upLayer;
    getIndexId(idString, &seriesId, &bgLayer, &upLayer);

    tmpLine = tmpLine.substr(splitPosition + 1);
    splitPosition = tmpLine.find(',');
    std::string seriesName = tmpLine.substr(0, splitPosition);
    seriesNameIndexs_[seriesId][bgLayer * 26 + upLayer] = seriesName;

    tmpLine = tmpLine.substr(splitPosition + 1);
    splitPosition = tmpLine.find(',');
    std::string mergeTypeStr = tmpLine.substr(0, splitPosition);
    if (mergeTypeStr == "diff") {
        tmpLine = tmpLine.substr(splitPosition + 1);
        removeEOLChar(&tmpLine);
        int bgLayerId;
        int upLayerId;
        layerIdcode(tmpLine, &bgLayerId, &upLayerId);
        bgLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = 0;
        upLayerId += bgLayerId * 26;
        if (upLayerId) {
            upLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = upLayerId;
        }
        else {
            upLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = -1;
        }
        
    }
    else if (mergeTypeStr == "seton") {
        tmpLine = tmpLine.substr(splitPosition + 1);
        removeEOLChar(&tmpLine);
        splitPosition = tmpLine.find(':');
        if (splitPosition != std::string::npos) {
            tmpLine = tmpLine.substr(0, splitPosition);
        }
        int bgLayerId;
        int upLayerId;
        layerIdcode(tmpLine, &bgLayerId, &upLayerId);
        bgLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = bgLayerId * 26;
        if (upLayerId) {
            upLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = bgLayerId * 26 + upLayerId;
        }
        else {
            upLayerIndexs_[seriesId][bgLayer * 26 + upLayer] = -1;
        }
    }
}

// turn all upper case char to lower case in input string
// @param 1 - inString: pointer to string to be handle
void CSVimagediffmapSplitter::stringToLowercase(std::string *inString) {
    for (size_t i = 0; i < (*inString).size(); i++) {
        char tmpChar = (*inString).at(i);
        if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
            (*inString)[i] = tmpChar - 'A' + 'a';
        }
    }
}

// remove all '\r' and \n' at the end of input string
// @param 1 - inString: pointer to string to be handle
void CSVimagediffmapSplitter::removeEOLChar(std::string *inString) {
    int size = (*inString).size();
    char endChar = (*inString).at(size - 1);
    while ((endChar == '\r') || (endChar == '\n')) {
        (*inString).erase(size - 1, 1);
        size--;
        endChar = (*inString).at(size - 1);
    }
}

// clear used info and get new for a new group
// corresponding to a new line in cglist.csv
// return a bool indicating whether there is a next group
bool CSVimagediffmapSplitter::getNewGroup() {
    seriesNameIndexs_.clear();
    seriesNameIndexs_.resize(4, newSeriesNameIndex_);
    bgLayerIndexs_.clear();
    bgLayerIndexs_.resize(4, newLayerIdcodeIndex_);
    upLayerIndexs_.clear();
    upLayerIndexs_.resize(4, newLayerIdcodeIndex_);

    std::string currentGroupName = lineBuff_.substr(0, 5);

    bool getlineSuccess = false;
    do {
        char tmpChar = lineBuff_.at(0);
        // comment line, appear in Riddle Joker
        if (tmpChar == '#') {
            continue;
        }
        // blank line
        if ((tmpChar == '\r') || (tmpChar == '\n')) {
            continue;
        }
        stringToLowercase(&lineBuff_);
        if (lineBuff_.substr(0, 5) != currentGroupName) {
            getlineSuccess = true;
            break;
        }
        lineSplitt(lineBuff_);
    } while (std::getline(imagediffmap_, lineBuff_));

    int groupIdi = currentGroupName.at(2) - '1';
    int groupIdj = (currentGroupName.at(3) - '0') * 10 + currentGroupName.at(4) - '1';
    int groupId = groupIdi * 30 + groupIdj;
    groupSeriesNameIndexs_.push_back(seriesNameIndexs_);
    groupBgLayerIndexs_.push_back(bgLayerIndexs_);
    groupUpLayerIndexs_.push_back(upLayerIndexs_);
    groupIndex_[groupId] = groupsPushed;
    groupsPushed++;

    return getlineSuccess;
}

// find cg information from cg name string get from imagediffmap.csv
// @param 1 - cgInfo: cg name string
// @param 3 - p_cgSeriesName: pointer to store cg's series name
// @param 3 - p_bgLayer: pointer to store background identifier
// @param 4 - p_upLayer: pointer to upper layer identifier
// example: "ev101aa" --> "ev101a", 0, 0; "ev113_mbc" --> "ev113_mm", 1, 2;
void CSVimagediffmapSplitter::findInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer) {
    std::string tmpString;

    tmpString = cgInfo.substr(0, 5);
    int groupIdi = tmpString.at(2) - '1';
    int groupIdj = (tmpString.at(3) - '0') * 10 + tmpString.at(4) - '1';
    int groupId = groupIdi * 30 + groupIdj;
    int groupPos = groupIndex_[groupId];

    tmpString = cgInfo.substr(5);
    int seriesId;
    int bgLayer;
    int upLayer;
    getIndexId(tmpString, &seriesId, &bgLayer, &upLayer);

    *p_cgSeriesName = groupSeriesNameIndexs_[groupPos][seriesId][bgLayer * 26 + upLayer];
    *p_bgLayer = groupBgLayerIndexs_[groupPos][seriesId][bgLayer * 26 + upLayer];
    *p_upLayer = groupUpLayerIndexs_[groupPos][seriesId][bgLayer * 26 + upLayer];
}

// No longer used after v3.0 due to getting info from imagediffmap.csv
// replaced by void CSVimagediffmapSplitter::findInfo(...)
/*
// get cg information from cg name string get from csv file
// @param 1 - cgInfo: cg name string
// @param 3 - p_cgSeriesName: pointer to store cg's series name
// @param 3 - p_bgLayer: pointer to store background identifier
// @param 4 - p_upLayer: pointer to upper layer identifier
// example: "ev101aa" --> "ev101a", 0, 0; "ev113_mbc" --> "ev113_mm", 1, 2;
void CSVFileSplitter::cgInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer) {
    int stringLen = cgInfo.length();
    std::string tmpString = cgInfo.substr(0, stringLen - 2);
    char tmpChar;

    // Senren Banka Yoshino animal ear CG
    if (tmpString.at(stringLen - 3) == 'm') {
        tmpString = tmpString + "m";
    }
    else {
        tmpString = tmpString + "a";
    }
    *p_cgSeriesName = tmpString;

    tmpChar = cgInfo.at(stringLen - 2);
    if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_bgLayer = tmpChar - 'a';
    }
    else if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_bgLayer = tmpChar - 'A';
    }

    tmpChar = cgInfo.at(stringLen - 1);
    if ((tmpChar >= 'a') && (tmpChar <= 'z')) {
        *p_upLayer = tmpChar - 'a';
    }
    else if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
        *p_upLayer = tmpChar - 'A';
    }
}
*/
