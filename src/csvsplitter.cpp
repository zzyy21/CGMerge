/* ****************************************************************
 * FilePath     : /src/csvsplitter.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:43:38
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-07 23:15:37
 * Description  : functions to handle csv file
 * Revision     : 
 * **************************************************************** */

#include "csvsplitter.h"

#include "cgpic.h"
#include "cglayer.h"
#include "cglayerindex.h"
#include "stringhandle.h"

#include <string>
#include <vector>
#include <cstdio>
#include <iostream>

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

// return a vector of CGPic get from csv line
// @param 1 - csvLine: line from csv file. 
// example format: "thum_ev104,芳04,\tev104aa,ev104ab,ev104ac,ev104ad,ev104ae,ev104af"
// example format: "thum_ev311,ム11,\tev311aa,ev311ab,ev311ab|*ev311_aa"
std::vector<CGPic> CSVFileSplitter::csvLineSplit(const std::string &csvLine) {
    std::vector<CGPic> cgPics;
    std::string tmpLine = csvLine;
    size_t splitPosition;

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
            cgInfo(addcgInfo, &addSeriesName, &addBgLayer, &addUpLayer);

            // new add series, create a new layer index for the new series
            if (addSeriesName != (*currentAddSeries_).seriesName()) {
                delete currentAddSeries_;
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

        cgInfo(tmpCGpicInfo, &mainSeriesName, &mainBgLayer, &mainUpLayer);

        // new main series, create a new layer index for the new series
        if (mainSeriesName != (*currentMainSeries_).seriesName()) {
            delete currentMainSeries_;
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
        sprintf(cgNumChar, "%02d", cgPicNum);
        std::string tmpCGFileName = cgName + "_" + cgNumChar + ".png";
        tmpCGPic.setFileName(tmpCGFileName);
        cgPics.push_back(tmpCGPic);

        tmpLine = tmpLine.substr(splitPosition + 1);
    }

    return cgPics;
}

// CSVFileSplitter constructor, handle the file with input name.
// @param 1 - csvFileName: path to csv file
CSVFileSplitter::CSVFileSplitter(const std::string &csvFileName) {
    csvFileName_ = csvFileName;
    csvCGPicSeriesNum_ = 0;
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
            csvCGPicSeries_.push_back(csvLineSplit(lineBuff));
            csvCGPicSeriesNum_++;
        }
    }

    delete currentMainSeries_;
    delete currentAddSeries_;

    csvFile.close();
}

CSVFileSplitter::~CSVFileSplitter() {
}

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
