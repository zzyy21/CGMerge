/* ****************************************************************
 * FilePath     : /src/csvsplitter.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:43:38
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:41:16
 * Description  : functions to handle cglist.csv file
 * Revision     : v1.0 - process cglist.csv
 *                v3.0 - process cglist.csv & imagediffmap.csv,
 *                  merge images using OpenCV instead of generate
 *                  Magick Command-line scripts
 *                v3.1 - add prompt text for user
 *                v3.2 - fix bug of commented line in cglist.csv
 *                v3.3 - move class CSVimagediffmapSplitter for
 *                  imagediffmap.csv file handling to new file
 *                  cgpicindex.cpp.
 *                  use new layer identify rule
 * **************************************************************** */

#include "csvsplitter.h"

#include <string>
#include <vector>
#include <cstdio>
//#include <iostream>

#include "cgpic.h"
#include "cglayer.h"
#include "cglayerindex.h"
#include "cgpicindex.h"

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
    std::string cgName;
    if (nameInLine_) {
        std::string charaNo = csvLine.substr(7, 1);
        splitPosition = tmpLine.find(',');
        cgName = charaNo + "_" + tmpLine.substr(0, splitPosition);
        tmpLine = tmpLine.substr(splitPosition + 1);
    }
    else {
        cgName = csvLine.substr(5, 5);
    }

    // remove '\t' before cgs if exist
    if (tmpLine.at(0) == '\t') {
        tmpLine = tmpLine.substr(1);
    }

    // process the end of line to make it easier to split
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
            csvImageDiffmapSplitter_.findInfo(addcgInfo, &addSeriesName, &addBgLayer, &addUpLayer);

            // new add series, create a new layer index for the new series
            if (addSeriesName != (*currentAddSeries_).seriesName()) {
                delete currentAddSeries_;
                printf("\xD5\xFD\xD4\xDA\xCC\xE1\xC8\xA1 %s \xD6\xD0\xCD\xBC\xB2\xE3\xA1\xAD\r\n", addSeriesName.c_str());
                currentAddSeries_ = new CGLayerIndex(addSeriesName);
            }

            // before v3.2
            //// addUpLayer != 0 -> append add series upper layer
            //if (addUpLayer != -1) {
            //    tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, addUpLayer));
            //}
            //// append add series background layer
            //tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer, 0));

            // after v3.3
            // addUpLayer != -1 -> append add series upper layer
            if (addUpLayer != -1) {
                tmpCGPic.addLayer((*currentAddSeries_).findLayer(addUpLayer));
            }
            // append add series background layer
            tmpCGPic.addLayer((*currentAddSeries_).findLayer(addBgLayer));

            tmpCGpicInfo = tmpCGpicInfo.substr(0, addPosition);
        }

        csvImageDiffmapSplitter_.findInfo(tmpCGpicInfo, &mainSeriesName, &mainBgLayer, &mainUpLayer);

        // new main series, create a new layer index for the new series
        if (mainSeriesName != (*currentMainSeries_).seriesName()) {
            delete currentMainSeries_;
            printf("\xD5\xFD\xD4\xDA\xCC\xE1\xC8\xA1 %s \xD6\xD0\xCD\xBC\xB2\xE3\xA1\xAD\r\n", mainSeriesName.c_str());
            currentMainSeries_ = new CGLayerIndex(mainSeriesName);
        }

        // before v3.2
        //// mainUpLayer != 0 -> append main series upper layer
        //if (mainUpLayer) {
        //    tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer, mainUpLayer));
        //}
        //// append main series background layer
        //tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer, 0));

        // after v3.3
        // mainUpLayer != 0 -> append main series upper layer
        if (mainUpLayer != -1) {
            tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainUpLayer));
        }
        // append main series background layer
        tmpCGPic.addLayer((*currentMainSeries_).findLayer(mainBgLayer));

        tmpCGPic.setSize((*currentMainSeries_).imageWidth(), (*currentMainSeries_).imageHeight());
        char cgNumChar[3];
        sprintf(cgNumChar, "%03d", cgPicNum);
        std::string tmpCGFileName = cgName + "_" + cgNumChar + ".png";
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

// turn all upper case char to lower case in input string
// @param 1 - inString: pointer to string to be handle
void CSVFileSplitter::stringToLowercase(std::string *inString) {
    for (size_t i = 0; i < (*inString).size(); i++) {
        char tmpChar = (*inString).at(i);
        if ((tmpChar >= 'A') && (tmpChar <= 'Z')) {
            (*inString)[i] = tmpChar - 'A' + 'a';
        }
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

    // line 1: always "#CGモード一覧\r\n" (ANSI)
    std::getline(csvFile, lineBuff);
    // line 2: title line
    std::getline(csvFile, lineBuff);
    titleLineSplit(lineBuff);

    // initial create dummy index name
    currentMainSeries_ = new CGLayerIndex;
    currentAddSeries_ = new CGLayerIndex;

    while (std::getline(csvFile, lineBuff)) {
        char tmpChar = lineBuff.at(0);
        // ":" divisse between character or between cg and sd
        if (tmpChar == ':') {
            continue;
        }
        // comment line, appear in Riddle Joker
        if (tmpChar == '#') {
            continue;
        }
        // blank line
        if ((tmpChar == '\r') || (tmpChar == '\n')) {
            continue;
        }

        stringToLowercase(&lineBuff);

        // cg line start with "thum_evxxx,..." or "thum_EVxxx,..."
        // sd line start with "thum_sdxxx,..." or "thum_SDxxx,..."
        tmpChar = lineBuff.at(5);
        if ((tmpChar == 's') || (tmpChar == 'S')) {
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

// split the title line for number of item before cg info
// set the variables (itemBeforeCG_, nameInLine_)
// @param 1 - titleLine: title line in cglist.csv (ANSI)
// example: "#サムネール, 画像1, 画像2, 画像3, ...\r\n" --> (1, false)
//          "#サムネール,CG名称,\t画像1, 画像2, 画像3, ...\r\n" --> (2, true)
void CSVFileSplitter::titleLineSplit(const std::string &titleLine) {
    size_t splitPosition;
    splitPosition = titleLine.find('1');
    std::string tmpLine = titleLine.substr(0, splitPosition - 4);

    itemBeforeCg_ = 0;
    size_t findStart = 0;
    while ((findStart = tmpLine.find(',', findStart + 1)) != std::string::npos) {
        itemBeforeCg_++;
    }

    if (itemBeforeCg_ > 1) {
        nameInLine_ = true;
    }
    else {
        nameInLine_ = false;
    }
    
}
