/* ****************************************************************
 * FilePath     : /src/csvsplitter.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:43:38
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-24 19:43:38
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

std::vector<CGPic> CSVFileSplitter::csvLineSplit(const std::string &csvLine) {
    std::vector<CGPic> cgPics;
    std::string tmpLine = csvLine;
    size_t splitPosition;

    splitPosition = tmpLine.find(',');
    tmpLine = tmpLine.substr(splitPosition + 1);

    splitPosition = tmpLine.find(',');
    std::string cgName = tmpLine.substr(0, splitPosition);

    splitPosition = tmpLine.find('\t');
    tmpLine = tmpLine.substr(splitPosition + 1);
    removeEOLChar(&tmpLine);
    tmpLine = tmpLine + ",";

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

            // new add series
            if (addSeriesName != (*currentAddSeries_).seriesName()) {
                delete currentAddSeries_;
                currentAddSeries_ = new CGLayerIndex(addSeriesName);
            }

            if (addUpLayer) {
                tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, addUpLayer));
            }
            tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, 0));

            tmpCGpicInfo = tmpCGpicInfo.substr(0, addPosition);
        }

        cgInfo(tmpCGpicInfo, &mainSeriesName, &mainBgLayer, &mainUpLayer);

        // new main series
        if (mainSeriesName != (*currentMainSeries_).seriesName()) {
            delete currentMainSeries_;
            currentMainSeries_ = new CGLayerIndex(mainSeriesName);
        }

        if (mainUpLayer) {
            tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer, mainUpLayer));
        }
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

void CSVFileSplitter::writeBatFile() {
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
