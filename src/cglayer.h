/* ****************************************************************
 * FilePath     : /src/cglayer.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:32:10
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 16:23:54
 * Description  : class for layer information
 * Revision     : v1.0 - first release
 *                v2.0 - Add function to call tlg2png to deal with
 *                  tlg files extracted by KrkrExtract.
 *                  Modify the rule for generating fileName_ to
 *                  match files processed by tlg2png.
 *                v3.0 - add fuctions using OpenCV, read image
 *                  files and pad to target size.
 * **************************************************************** */

#ifndef CGLAYER_H_
#define CGLAYER_H_

#include <string>
#include "opencv2/opencv.hpp"

class CGLayer {
    private:
        std::string seriesName_;
        int layerid_;
        int width_;
        int height_;
        int left_;
        int top_;
        std::string fileName_;

        cv::Mat img_;

        void tlg2png();

        void readImg();

    public:
        CGLayer(const std::string &seriesName, int layerid, int width,
                int height, int left, int top);
        ~CGLayer();

        int left();
        int top();
        std::string fileName();

        cv::Mat img();
};

#endif
