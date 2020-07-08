# CGMerge

CGMerge is a tool to merge CG of YuzuSoft galgame.  
This tool will read the CG list and merge rules from the csv file and merge resources into CGs.  
This tool does not have the function of extracting files from the game, and needs to be used with extracting tools.

## Ciallo～(∠・ω< )⌒★

CGMerge是一个合成柚子社galgame CG的工具。  
将从csv文件中读取CG列表及合成规则并合成CG。  
这个工具本身不具备解包的功能，需要搭配解包工具使用。  
[查看中文说明](/doc/README_zh-CN.md)

## How to use

[Full instruction of extract resources and merge CG (zh-CN only)](/doc/full_flow.md)

### Input files needed (get from extracted resources)

data.xp3 -> main/`cglist.csv` & `imagediffmap.csv`, These two files must `be converted into ANSI encoding format`  
evimage.xp3 -> `*.json` & `*/n.tlg`

The input files need to be placed in the same directory as the tool's executable file.

### Merge CGs

After all needed input files prepared  
Double click to run CGMerge.exe  
or  
run the program in command line

``` cmd
CGMerge
```

## Development setting

The Makefile is only tested for cmd and MinGW-w64.

Windows cmd + MinGW-w64:

``` cmd
make
```

If the Makefile does not work in your enviroment, you may build the files manually.

The `source files` are located in `src/`

Thirdparty libraries used:  

1. [tlg2png](https://github.com/vn-tools/tlg2png) for extracting images from tlg file. Source files for building this library is attached in 3rdparty/3rdparty_src/tlg2png/

2. [libpng](http://www.libpng.org/pub/png/libpng.html), dependency of tlg2png

3. [zlib](https://www.zlib.net/), dependency of libpng

4. [OpenCV](https://opencv.org/), used in image processing functions

Thirdparty library include path: `3rdparty/include/`

Thirdparty library path: `3rdparty/lib/`  
The libraries provided in the project are `dll` file compiled with `MinGW-w64`, you may use your own build if this not work or you use a different enviroment.  
Version of library provided in `3rdparty/lib/`:

````txt
tlg2png: v1.0
libpng: 1.6.37
zlib: 1.2.11
OpenCV: 4.3.0 (required functions only)
````

## License

CGMerge is released under the [GPL License](http://www.gnu.org/licenses/gpl.html).

Thirdparty libraries used are subject to their license.
