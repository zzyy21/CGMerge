## 完整的解包合成流程

这是一份使用相关工具进行解包和CG合成的教程。\
解包方法有很多，这里只说明其中一种（我最开始用的一种）。\
这个工具也是基于这种方法原本的不足而编写的。

[返回中文说明](/doc/README_zh-CN.md)
[Back to README](/README.md)

### 需要的工具

* [KrKrExtract](https://github.com/xmoeproject/KrkrExtract/releases) : xmoe的KiriKiri解包工具
* [expimg](http://asmodean.reverse.net/pages/expimg.html) : 从pimg中提取tlg和layers info
* [tlg2png](https://github.com/vn-tools/tlg2png/releases/tag/v1.0) : tlg转换为png工具
* [ImageMagick](https://imagemagick.org/script/download.php#windows) : 用于CG合成的图像处理工具

### 解包与合成过程

1. 将KrKrExtract放入游戏目录下，并将游戏程序拖到KrKrExtract.exe上运行。

2. 将`data.xp3`拖进KrKrExtract的窗口，进行解码。解码完成后找到`./KrkrExtract_Output/data/main/cglist.csv`文件。这个文件包含了CG列表，并且根据列表中的CG名我们可以得到每张CG的合成规则。打开该文件，并使用`ANSI`(GB2312)编码重新保存。此后合成使用编码修改后的文件。

3. 将`evimage.xp3`拖进KrKrExtract的窗口，进行解码。解码后得到`*.pimg`的CG文件包。这些文件包中包含了CG的图层及相关信息。

4. 将expimg放入pimg解包目录下，并在该目录新建bat文件，从pimg中提取相关文件。

    ``` cmd
    for %i in (*.pimg) do expimg %i
    ```

5. 将tlg2png放入pimg解包目录下，并在该目录执行下列命令，将tlg文件转换为png格式。

    ``` cmd
    for %i in (*.tlg) do tlg2png %i %i.png
    ```

6. 将`*.png`、`*.layers.txt`、ANSI编码的`cglist.csv`放在同一目录下，执行`CGMerge`

### Reference

1. <https://xmoeproject.github.io/KrkrExtract/#fh5co-info-unpack>
