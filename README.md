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

### Install ImageMagick

`ImageMagick` command-line tools is needed for png merging. [Download page](https://imagemagick.org/script/download.php#windows)

### Input files needed (get from extracted resources)

data.xp3 -> main/`cglist.csv`, This file must `be converted into ANSI encoding format`  
evimage -> *.pimg -> `*+pimg+n.tlg.png` & `*+pimg+layers.txt`

The input files need to be placed in the same directory as the tool's executable file.

### Merge CGs

Double click to run CGMerge.exe (read cglist.csv by default)  
or  
run the command below to read specified csv file

``` cmd
CGMerge yourfile.csv
```

## Development setting

The Makefile is only tested for cmd and MinGW-w64.

Windows cmd + MinGW-w64:

``` cmd
make
```

If the Makefile does not work in your enviroment, you may build the files manually.

The `source files` are located in `src/`  
and the `headers` are located in `include/`  
No extra library is needed.

## License

CGMerge is released under the [GPL License](http://www.gnu.org/licenses/gpl.html).
