/* ****************************************************************
 * FilePath     : /src/csvsplitter.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:43:38
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 23:37:16
 * Description  : functions to handle csv file
 * Revision     : v1.0 - process cglist.csv
 *                v3.0 - process cglist.csv & imagediffmap.csv,
 *                  merge images using OpenCV instead of generate
 *                  Magick Command-line scripts
 *                v3.1 - add prompt text for user
 * **************************************************************** */

#include "csvsplitter.h"

#include "cgpic.h"
#include "cglayer.h"
#include "cglayerindex.h"

#include <string>
#include <vector>
#include <cstdio>
//#include <iostream>

// CSVimagediffmapSplitter constructor.
// open "imagediffmap.csv" and get the first line to lineBuff_
CSVimagediffmapSplitter::CSVimagediffmapSplitter() {
    imagediffmap_.open("imagediffmap.csv", std::ios::in|std::ios::binary);
    std::getline(imagediffmap_, lineBuff_);
}

CSVimagediffmapSplitter::~CSVimagediffmapSplitter() {
    imagediffmap_.close();
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
    std::string findName = tmpLine.substr(0, splitPosition - 2);
    if (findName != currentFindName_) {
        seriesNameIndexs_.push_back(newSeriesNameIndex_);
        findNameIndex_.push_back(findName);
        currentFindName_ = findName;
        currentIndexNum_++;
    }

    int bgLayer;
    int upLayer;
    std::string layerNumString = tmpLine.substr(splitPosition - 2, 2);
    getLayerNum(layerNumString, &bgLayer, &upLayer);

    tmpLine = tmpLine.substr(splitPosition + 1);
    splitPosition = tmpLine.find(',');
    std::string seriesName = tmpLine.substr(0, splitPosition);
    seriesNameIndexs_[currentIndexNum_][bgLayer * 26 + upLayer] = seriesName;
}

// clear used info and get new for a new group
// corresponding to a new line in cglist.csv
void CSVimagediffmapSplitter::getNewGroup() {
    seriesNameIndexs_.clear();
    findNameIndex_.clear();
    currentIndexNum_ = -1;
    currentFindName_ = "none";

    currentGroupName_ = lineBuff_.substr(0, 5);

    do {
        if (lineBuff_.substr(0, 5) != currentGroupName_) {
            break;
        }
        lineSplitt(lineBuff_);
    } while (std::getline(imagediffmap_, lineBuff_));

}

// find cg information from cg name string get from imagediffmap.csv
// @param 1 - cgInfo: cg name string
// @param 3 - p_cgSeriesName: pointer to store cg's series name
// @param 3 - p_bgLayer: pointer to store background identifier
// @param 4 - p_upLayer: pointer to upper layer identifier
// example: "ev101aa" --> "ev101a", 0, 0; "ev113_mbc" --> "ev113_mm", 1, 2;
void CSVimagediffmapSplitter::findInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer) {
    int stringLen = cgInfo.length();
    std::string tmpString;

    tmpString = cgInfo.substr(stringLen - 2, 2);
    getLayerNum(tmpString, p_bgLayer, p_upLayer);

    tmpString = cgInfo.substr(0, stringLen - 2);
    int findNameNum = seriesNameIndexs_.size();
    for (int i = 0; i < findNameNum; i++) {
        if (tmpString == findNameIndex_[i]) {
            *p_cgSeriesName = seriesNameIndexs_[i][(*p_bgLayer) * 26 + (*p_upLayer)];
            break;
        }
    }

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

// return a vector of CGPic get from csv line
// @param 1 - csvLine: line from csv file. 
// example format: "thum_ev104,芳04,\tev104aa,ev104ab,ev104ac,ev104ad,ev104ae,ev104af"
// example format: "thum_ev311,ム11,\tev311aa,ev311ab,ev311ab|*ev311_aa"
std::vector<CGPic> CSVFileSplitter::csvLineSplit(const std::string &csvLine) {
    std::vector<CGPic> cgPics;
    std::string tmpLine = csvLine;
    size_t splitPosition;

    std::string charaNo = tmpLine.substr(7, 1);

    // useless thumbnail info
    splitPosition = tmpLine.find(',');
    tmpLine = tmpLine.substr(splitPosition + 1);

    // cg series name displayed in game, used in output file name
    splitPosition = tmpLine.find(',');
    std::string cgName = tmpLine.substr(0, splitPosition);

    // process the end of line to make it easier to split
    splitPosition = tmpLine.find('\t');
    tmpLine = tmpLine.substr(splitPosition + 1);
    removeEOLChar(&tmpLine);
    tmpLine = tmpLine + ",";

    csvImageDiffmapSplitter.getNewGroup();

    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // if the cg name is "ev418bc|*ev418_ab"
    // it has four layers in two series, in the order from background to uppest:
    // ("ev418a", 1, 0), main series background layer
    // ("ev418a", 1, 2), main series upper layer
    // ("ev418_a", 0, 0), add series background layer
    // ("ev418_a", 0, 1), add series upper layer
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    std::string mainSeriesName;
    int mainBgLayer;
    int mainUpLayer;

    std::string addSeriesName;
    int addBgLayer;
    int addUpLayer;

    int cgPicNum = 0;

    while ((splitPosition = tmpLine.find(',')) != std::string::npos) {
        cgPicNum++;
        CGPic tmpCGPic;
        std::string tmpCGpicInfo = tmpLine.substr(0, splitPosition);

        size_t addPosition = tmpCGpicInfo.find("|*");
        if (addPosition != std::string::npos) {
            std::string addcgInfo = tmpCGpicInfo.substr(addPosition + 2);
            csvImageDiffmapSplitter.findInfo(addcgInfo, &addSeriesName, &addBgLayer, &addUpLayer);

            // new add series, create a new layer index for the new series
            if (addSeriesName != (*currentAddSeries_).seriesName()) {
                delete currentAddSeries_;
                printf("\xD5\xFD\xD4\xDA\xCC\xE1\xC8\xA1 %s \xD6\xD0\xCD\xBC\xB2\xE3\xA1\xAD\r\n", addSeriesName.c_str());
                currentAddSeries_ = new CGLayerIndex(addSeriesName);
            }

            // addUpLayer != 0 -> append add series upper layer
            if (addUpLayer) {
                tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, addUpLayer));
            }
            // append add series background layer
            tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, 0));

            tmpCGpicInfo = tmpCGpicInfo.substr(0, addPosition);
        }

        csvImageDiffmapSplitter.findInfo(tmpCGpicInfo, &mainSeriesName, &mainBgLayer, &mainUpLayer);

        // new main series, create a new layer index for the new series
        if (mainSeriesName != (*currentMainSeries_).seriesName()) {
            delete currentMainSeries_;
            printf("\xD5\xFD\xD4\xDA\xCC\xE1\xC8\xA1 %s \xD6\xD0\xCD\xBC\xB2\xE3\xA1\xAD\r\n", mainSeriesName.c_str());
            currentMainSeries_ = new CGLayerIndex(mainSeriesName);
        }

        // mainUpLayer != 0 -> append main series upper layer
        if (mainUpLayer) {
            tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer, mainUpLayer));
        }
        // append main series background layer
        tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer, 0));

        tmpCGPic.setSize((*currentMainSeries_).imageWidth(), (*currentMainSeries_).imageHeight());
        char cgNumChar[3];
        sprintf(cgNumChar, "%03d", cgPicNum);
        std::string tmpCGFileName = charaNo + "_" + cgName + "_" + cgNumChar + ".png";
        tmpCGPic.setFileName(tmpCGFileName);
        cgPics.push_back(tmpCGPic);

        tmpLine = tmpLine.substr(splitPosition + 1);
    }

    return cgPics;
}

// remove all '\r' and \n' at the end of input string
// @param 1 - inString: pointer to string to be handle
void CSVFileSplitter::removeEOLChar(std::string *inString) {
    int size = (*inString).size();
    char endChar = (*inString).at(size - 1);
    while ((endChar == '\r') || (endChar == '\n')) {
        (*inString).erase(size - 1, 1);
        size--;
        endChar = (*inString).at(size - 1);
    }
}

// CSVFileSplitter constructor, handle the file with input name.
// @param 1 - csvFileName: path to csv file
CSVFileSplitter::CSVFileSplitter(const std::string &csvFileName) {
    csvFileName_ = csvFileName;
    csvCGPicSeriesNum_ = 0;
    totalPicNum_ = 0;
    std::ifstream csvFile;
    std::string lineBuff;
    csvFile.open(csvFileName_, std::ios::in|std::ios::binary);

    std::getline(csvFile, lineBuff);
    std::getline(csvFile, lineBuff);

    // initial create dummy index name
    currentMainSeries_ = new CGLayerIndex;
    currentAddSeries_ = new CGLayerIndex;

    while (std::getline(csvFile, lineBuff)) {
        // lines below ":" is SD pic
        if (lineBuff.at(0) == ':') {
            break;
        }
        else {
            // before v3.0, store the info to generate Magick script
            //csvCGPicSeries_.push_back(csvLineSplit(lineBuff));
            //csvCGPicSeriesNum_++;

            // v3.0 merge image using OpenCV
            csvCGPicSeriesNum_++;
            printf("\r\n\xA1\xAA\xA1\xAA\xA1\xAA\xA1\xAA\xA1\xAA \xB5\xDA %i \xD7\xE9 \xA1\xAA\xA1\xAA\xA1\xAA\xA1\xAA\xA1\xAA\r\n", csvCGPicSeriesNum_);
            std::vector<CGPic> cgGroup = csvLineSplit(lineBuff);
            int cgNum = cgGroup.size();
            totalPicNum_ += cgNum;
            printf("\xB5\xDA %i \xD7\xE9\xA3\xAC\xB9\xB2 %i \xD5\xC5\xA3\xBA\r\n", csvCGPicSeriesNum_, cgNum);
            for (int i = 0; i < cgNum; i++) {
                printf("\xD5\xFD\xD4\xDA\xBA\xCF\xB3\xC9\xB5\xDA %i \xD5\xC5\xA1\xAD\r\n", i + 1);
                cgGroup[i].saveImage();
            }
        }
    }

    delete currentMainSeries_;
    delete currentAddSeries_;

    csvFile.close();
}

CSVFileSplitter::~CSVFileSplitter() {
}

// return total number of CG group
int CSVFileSplitter::totalGroup() {
    return csvCGPicSeriesNum_;
}

// return total number of CG Image
int CSVFileSplitter::totalPic() {
    return totalPicNum_;
}

// No longer used after v3.0 due to the use of OpenCV
/*
// print stored cgs info for debug
void CSVFileSplitter::debugPrint() {
    std::cout << "csvCGPicSeriesNum_ = " << csvCGPicSeriesNum_ << std::endl;
    for (int i = 0; i < csvCGPicSeriesNum_; i++) {
        std::vector<CGPic> tmpCGPics = csvCGPicSeries_[i];
        int tmpCGPicNum = tmpCGPics.size();
        std::cout << "tmpCGPics.size(): " << tmpCGPicNum << std::endl;

        for (int j = 0; j < tmpCGPicNum; j++) {
            CGPic tmpCGPic = tmpCGPics[j];

            std::cout << tmpCGPic.magickMergeScript() << std::endl;
        }
    }
}
*/

// No longer used after v3.0 due to the use of OpenCV
/*
// generate a batch script with merging command-line of all CGs stored
void CSVFileSplitter::writeBatFile() {
    // Prompt text uses GB2312 encoding formatt
    std::ofstream outFile("merge.bat", std::ios::out | std::ios::binary);
    outFile << "@echo off\r\n";

    outFile << "if not exist \"CGOutput\" (\r\n"
            << "  mkdir \"CGOutput\"\r\n"
            << ")\r\n";

    outFile << "echo \xB9\xB2\xD3\xD0 " << csvCGPicSeriesNum_
            << " \xD7\xE9\xCD\xBC\xC6\xAC\r\n";

    for (int i = 0; i < csvCGPicSeriesNum_; i++) {
        std::vector<CGPic> tmpCGPics = csvCGPicSeries_[i];
        int tmpCGPicNum = tmpCGPics.size();
        outFile << "echo \xB5\xDA " << i + 1 << "/" << csvCGPicSeriesNum_ << " \xD7\xE9\xA3"
                << "\xAC\xB9\xB2 " << tmpCGPicNum << " \xD5\xC5\xA3\xBA\r\n";

        for (int j = 0; j < tmpCGPicNum; j++) {
            CGPic tmpCGPic = tmpCGPics[j];
            outFile << "echo \xD5\xFD\xD4\xDA\xBA\xCF\xB3\xC9\xB5\xDA "
                    << j + 1 << " \xD5\xC5\xA1\xAD\r\n";
            outFile << tmpCGPic.magickMergeScript() << "\r\n";
        }
    }

    outFile << "echo \x43\x47\xBA\xCF\xB3\xC9\xCD\xEA\xB3\xC9\r\n";
    outFile << "pause > nul\r\n";
    outFile << "del %0\r\n";
}
*/
