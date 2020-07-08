/* ****************************************************************
 * FilePath     : /src/cgpic.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:42:02
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 17:31:18
 * Description  : class for single cg picture
 * Revision     : v1.0 - first version for generate Magick convert
 *                  command-lines
 *                v3.0 - add fuctions using OpenCV, merge image
 *                  layers and save image file.
 * **************************************************************** */

#ifndef CGPIC_H_
#define CGPIC_H_

#include "cglayer.h"

#include <string>
#include <vector>

class CGPic {
    private:
        std::string fileName_;
        int width_;
        int height_;
        std::vector<CGLayer> layers_;

        void imgAppendLayer(const cv::Mat& up, cv::Mat* p_bg);

    public:
        CGPic();
        ~CGPic();

        void setSize(int width, int height);
        void addLayer(const CGLayer &layer);
        void setFileName(const std::string &fileName);

        // **No longer used after v3.0 due to the use of OpenCV
        //std::string magickMergeScript();
        void saveImage();
};

#endif
