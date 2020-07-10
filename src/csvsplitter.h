/* ****************************************************************
 * FilePath     : /src/csvsplitter.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 11:53:59
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:37:40
 * Description  : CSVSplitter class for cglist.csv file handling
 * Revision     : v1.0 - process cglist.csv
 *                v3.0 - process cglist.csv & imagediffmap.csv,
 *                  merge images using OpenCV instead of generate
 *                  Magick Command-line scripts
 *                v3.3 - move class CSVimagediffmapSplitter for
 *                  imagediffmap.csv file handling to new file
 *                  cgpicindex.h. use new layer identify rule.
 * **************************************************************** */

#ifndef CSVSPLITTER_H_
#define CSVSPLITTER_H_

#include <fstream>
#include <vector>
#include <string>

#include "cgpic.h"
#include "cglayerindex.h"
#include "cgpicindex.h"

// class for splitting cglist.csv
class CSVFileSplitter {
    private:
        std::string csvFileName_;
        std::vector<std::vector<CGPic> > csvCGPicSeries_;
        int csvCGPicSeriesNum_;
        int totalPicNum_;

        CSVimagediffmapSplitter csvImageDiffmapSplitter_;

        CGLayerIndex* currentMainSeries_;
        CGLayerIndex* currentAddSeries_;
        // **No longer used after v3.0 due to getting info from imagediffmap.csv
        //void cgInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer);
        std::vector<CGPic> csvLineSplit(const std::string &csvLine);

        void titleLineSplit(const std::string &titleLine);
        bool nameInLine_;
        int itemBeforeCg_;

        void removeEOLChar(std::string *inString);
        void stringToLowercase(std::string *inString);

    public:
        CSVFileSplitter(const std::string &csvFileName = "cglist.csv");
        ~CSVFileSplitter();

        int totalGroup();
        int totalPic();

        // **No longer used after v3.0 due to the use of OpenCV
        //void debugPrint();
        // **No longer used after v3.0 due to the use of OpenCV
        //void writeBatFile();
};

#endif
