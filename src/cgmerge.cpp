/* ****************************************************************
 * FilePath     : /src/cgmerge.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:42:04
 * Modifed by   : zzyy21
 * Last Modify  : 2020-07-08 17:15:18
 * Description  : main entry of CGMerge
 * Revision     : v1.0 - using Magick command-line tool batch file
 *                v3.0 - using built-in merge fuction with OpenCV
 * **************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "csvsplitter.h"
#include "cglayerindex.h"
#include "cglayer.h"
#include "cgpic.h"

int main(int argc, char const *argv[]) {
    std::string csvFileName;
    if (argc < 2) {
        csvFileName = "cglist.csv";
        std::cout << "open default cglist.csv\r\n";
    }
    else {
        csvFileName = argv[1];
        std::cout << "open csv file " << csvFileName << "\r\n";
    }

    CSVFileSplitter csvSplitter(csvFileName);

    //csvSplitter.writeBatFile();
    //system("merge.bat");

    return 0;
}
