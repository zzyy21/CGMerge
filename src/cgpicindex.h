/* ****************************************************************
 * FilePath     : /src/cgpicindex.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-07-10 19:31:21
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-10 19:45:06
 * Description  : CSVimagediffmapSplitter class for imagediffmap.csv 
 *                file handling. build a index for cgpic.
 * Revision     : v3.3 - new file for class CSVimagediffmapSplitter
 *                  imagediffmap.csv file handling.
 *                  get layer identify id from imagediffmap.csv.
 *                  use new layer identify rule.
 * **************************************************************** */

#ifndef CGPICINDEX_H_
#define CGPICINDEX_H_

#include <fstream>
#include <string>
#include <vector>

// class for splitting imagediffmap.csv
class CSVimagediffmapSplitter {
    private:
        std::ifstream imagediffmap_;
        std::string lineBuff_;

        std::vector<std::string> newSeriesNameIndex_ = std::vector<std::string>(26 * 26);
        std::vector<int> newLayerIdcodeIndex_ = std::vector<int>(26 * 26);
        std::vector<std::vector<std::string> > seriesNameIndexs_;
        std::vector<std::vector<int> > bgLayerIndexs_;
        std::vector<std::vector<int> > upLayerIndexs_;

        std::vector<std::vector<std::vector<std::string> > > groupSeriesNameIndexs_;
        std::vector<std::vector<std::vector<int> > > groupBgLayerIndexs_;
        std::vector<std::vector<std::vector<int> > > groupUpLayerIndexs_;
        std::vector<std::vector<std::string> > groupFindNameIndex_;
        int groupIndex_[8 * 30]; // max 8 character * 30 group/character
        int groupsPushed = 0;
        bool getNewGroup();

        void getLayerNum(const std::string &layerNumString, int* p_bgLayer, int* p_upLayer);
        void layerIdcode(const std::string &layerName, int* p_bgLayer, int* p_upLayer);
        void getIndexId(const std::string &idString, int* seriesId, int* p_bgLayer, int* p_upLayer);
        void lineSplitt(const std::string &line);

        void stringToLowercase(std::string *inString);
        void removeEOLChar(std::string *inString);

    public:
        CSVimagediffmapSplitter();
        ~CSVimagediffmapSplitter();

        void findInfo(const std::string &cgInfo, std::string* p_cgSeriesName, int* p_bgLayer, int* p_upLayer);

};

#endif
