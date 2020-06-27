/* ****************************************************************
 * FilePath     : /src/cgmerge.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-24 19:42:04
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-24 19:42:04
 * Description  : main entry of CGMerge
 * Revision     : 
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
        std::cout << "open default cglist.csv\r\n";
        csvFileName = "cglist.csv";
    }
    else {
        std::cout << "open csv file " << csvFileName << "\r\n";
        csvFileName = argv[1];
    }

    CSVFileSplitter csvSplitter(csvFileName);
    csvSplitter.writeBatFile();

    system("merge.bat");

    return 0;
}
