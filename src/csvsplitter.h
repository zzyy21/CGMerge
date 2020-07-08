/* ****************************************************************
 * FilePath     : /src/csvsplitter.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 11:53:59
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 14:45:25
 * Description  : CSVSplitter class for csv file handling
 * Revision     : v1.0 - process cglist.csv
 *                v3.0 - process cglist.csv & imagediffmap.csv
 * **************************************************************** */

#ifndef CSVSPLITTER_H_
#define CSVSPLITTER_H_

#include "cgpic.h"
#include "cglayerindex.h"

#include <fstream>
#include <vector>
#include <string>

// class for splitting imagediffmap.csv
class CSVimagediffmapSplitter {
    private:
        std::ifstream imagediffmap_;
        std::string lineBuff_;
        std::string currentGroupName_;

        std::vector<std::string> newSeriesNameIndex_ = std::vector<std::string>(26 * 26);
        std::vector<std::vector<std::string> > seriesNameIndexs_;
        std::vector<std::string> findNameIndex_;
        int currentIndexNum_;
        std::string currentFindName_;

        void getLayerNum(const std::string &layerNumString, int* p_bgLayer, int* p_upLayer);
        void lineSplitt(const std::string &line);

    public:
        CSVimagediffmapSplitter();
        ~CSVimagediffmapSplitter();

        void getNewGroup();

        void findInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer);

};

// class for splitting cglist.csv
class CSVFileSplitter {
    private:
        std::string csvFileName_;
        std::vector<std::vector<CGPic> > csvCGPicSeries_;
        int csvCGPicSeriesNum_;

        CSVimagediffmapSplitter csvImageDiffmapSplitter;

        CGLayerIndex* currentMainSeries_;
        CGLayerIndex* currentAddSeries_;
        // **No longer used after v3.0 due to getting info from imagediffmap.csv
        //oid cgInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer);
        std::vector<CGPic> csvLineSplit(const std::string &csvLine);

        void removeEOLChar(std::string *inString);

    public:
        CSVFileSplitter(const std::string &csvFileName = "cglist.csv");
        ~CSVFileSplitter();

        void debugPrint();
        void writeBatFile();
};


#endif
