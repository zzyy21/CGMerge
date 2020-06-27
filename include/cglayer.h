/* ****************************************************************
 * FilePath     : /include/cglayer.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:10
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-22 22:32:10
 * Description  : class for layer information
 * Revision     : 
 * **************************************************************** */

#ifndef CGLAYER_H_
#define CGLAYER_H_

#include <string>

class CGLayer {
    private:
        std::string seriesName_;
        int layerid_;
        // int width_;
        // int height_;
        int left_;
        int top_;
        std::string fileName_;

    public:
        CGLayer(const std::string &seriesName, int layerid, int left, int top);
        ~CGLayer();

        int left();
        int top();
        std::string fileName();
};

#endif
