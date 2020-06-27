# CGMerge

CGMerge是一个合成柚子社galgame CG的工具。  
将从csv文件中读取CG列表及合成规则并合成CG。  
这个工具本身不具备解包的功能，需要搭配解包工具使用。

## Ciallo～(∠・ω< )⌒★

[English Version of README](/README.md)

## 目录

* [如何使用](##如何使用)
* [开发配置](##开发配置)
* [许可证](##许可证)

## 如何使用

[完整的解包合成流程](/doc/full_flow.md)

### 安装ImageMagick工具

`ImageMagick` command-line tools 将被用于png的合成。[下载页面](https://imagemagick.org/script/download.php#windows)

### 需要的输入文件（解包获得）

data.xp3 -> main/`cglist.csv`, 并将该文件`转换为ANSI编码`格式  
evimage -> *.pimg -> `*+pimg+n.tlg.png` & `*+pimg+layers.txt`

输入文件需要和工具放在同一目录下。

### 执行合成

打开CGMerge.exe（默认读取cglist.csv）  
或  
执行命令读取特定csv文件

``` cmd
CGMerge yourfile.csv
```

## 开发配置

Makefile仅供在cmd+MinGW-w64下使用。

Windows cmd + MinGW-w64:

``` cmd
make
```

如果使用别的环境，Makefile可能不起作用，你需要手动编译。

源文件目录：`src/`  
头文件目录：`include/`  
不需要额外的库。

## 许可

本项目采用[GPL3.0](http://www.gnu.org/licenses/gpl.html)许可
