/* ****************************************************************
 * FilePath     : /include/csvsplitter.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 11:53:59
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-24 11:53:59
 * Description  : CSVSplitter class for csv file handling
 * Revision     : 
 * **************************************************************** */

#ifndef CSVSPLITTER_H_
#define CSVSPLITTER_H_

#include "cgpic.h"
#include "cglayerindex.h"

#include <fstream>
#include <vector>
#include <string>

class CSVFileSplitter {
    private:
        std::string csvFileName_;
        std::vector<std::vector<CGPic> > csvCGPicSeries_;
        int csvCGPicSeriesNum_;

        CGLayerIndex* currentMainSeries_;
        CGLayerIndex* currentAddSeries_;
        void cgInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer);
        std::vector<CGPic> csvLineSplit(const std::string &csvLine);

    public:
        CSVFileSplitter(const std::string &csvFileName);
        ~CSVFileSplitter();

        void debugPrint();
        void writeBatFile();
};


#endif
