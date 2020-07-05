/* ****************************************************************
 * FilePath     : /include/cgpic.h
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-22 22:42:02
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-22 22:42:02
 * Description  : class for single cg picture
 * Revision     : 
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

    public:
        CGPic();
        ~CGPic();

        void setSize(int width, int height);
        void addLayer(const CGLayer &layer);
        void setFileName(const std::string &fileName);

        std::string magickMergeScript();
};

#endif
