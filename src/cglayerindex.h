/* ****************************************************************
 * FilePath     : /src/cglayerindex.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-23 20:26:21
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-07 23:25:33
 * Description  : get layer index from file
 * Revision     : v1.0 - Get layer info from txt file by expimg
 *                v2.0 - Get layer info from json file by KrkrExtract
 * **************************************************************** */

#ifndef CGLAYERINDEX_H_
#define CGLAYERINDEX_H_

#include "cglayer.h"

#include <string>
#include <vector>

// CGLayerIndex
// use std::vector<CGLayer> to store a set of layer information
// use layerIndex_[26 * 26] to store the index of CGLayer
// background layer identifier 'A'->0, 'B'->1, ..., 'Z'->25
// upper layer identifier 'a'->0, 'b'->1, ..., 'z'->25
// layerIndex_[bg * 26 + up] is the position of this layer in vector

class CGLayerIndex {
    private:
        std::string seriesName_;
        int imageWidth_;
        int imageHeight_;
        std::vector<CGLayer> layers_;
        int layerIndex_[26 * 26];
        std::vector<int> availableIndex_;
        int layerNum_;

        // **No longer used due after v2.0 to the use of json layer info
        //int getValue(const std::string &line);
        void getPicId(const std::string &line, int* p_bgLayer, int* p_upLayer);
        // **No longer used due after v2.0 to the use of json layer info
        //int getInfoTxt();
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
