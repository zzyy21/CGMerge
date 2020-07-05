/* ****************************************************************
 * FilePath     : /src/cglayer.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:10
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-06 00:15:52
 * Description  : class for layer information
 * Revision     : v1.0 - first release
 *                v2.0 - Add function to call tlg2png to deal with
 *                  tlg files extracted by KrkrExtract.
 *                  Modify the rule for generating fileName_ to
 *                  match files processed by tlg2png.
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

        void tlg2png();

    public:
        CGLayer(const std::string &seriesName, int layerid, int left, int top);
        ~CGLayer();

        int left();
        int top();
        std::string fileName();
};

#endif
