/* ****************************************************************
 * FilePath     : /src/cglayerindex.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-23 20:26:21
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-05 20:30:18
 * Description  : get layer index from file
 * Revision     : v1.0 - Get layer info from txt file by expimg
 *                v2.0 - Get layer info from json file by KrkrExtract
 * **************************************************************** */

#ifndef CGLAYERINDEX_H_
#define CGLAYERINDEX_H_

#include "cglayer.h"

#include <string>
#include <vector>

class CGLayerIndex {
    private:
        std::string seriesName_;
        int imageWidth_;
        int imageHeight_;
        std::vector<CGLayer> layers_;
        int layerIndex_[26 * 26];
        std::vector<int> availableIndex_;
        int layerNum_;

        int getValue(const std::string &line);
        void getPicId(const std::string &line, int* p_bgLayer, int* p_upLayer);
        int getInfoTxt();
        int getInfoJson();

    public:
        CGLayerIndex();
        CGLayerIndex(const std::string &seriesName);
        ~CGLayerIndex();

        std::string seriesName();
        int layerNum();
        int imageWidth();
        int imageHeight();
        CGLayer findLayer(int bgLayer, int upLayer);
        CGLayer findLayer(int indexNum);
};

#endif
