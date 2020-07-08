## 完整的解包合成流程

这是一份使用相关工具进行解包和CG合成的教程。\
解包方法有很多，这里只说明其中一种（我最开始用的一种）。\
这个工具也是基于这种方法原本的不足而编写的。

[返回中文说明](/doc/README_zh-CN.md)
[Back to README](/README.md)

### 需要的工具

* [KrKrExtract](https://github.com/xmoeproject/KrkrExtract/releases) : xmoe的KiriKiri解包工具
* [ImageMagick](https://imagemagick.org/script/download.php#windows) : 用于CG合成的图像处理工具

### 解包与合成过程

1. 将KrKrExtract放入游戏目录下，并将游戏程序拖到KrKrExtract.exe上运行。

2. `Text Decryption` 选择 `Text`。将`data.xp3`拖进KrKrExtract的窗口，进行解码。解码完成后找到`./KrkrExtract_Output/data/main/` 目录下的 `cglist.csv` 及 `imagediffmap.csv` 两个文件。文件中包含了CG列表，并且根据列表中的CG名我们可以得到每张CG的合成规则。打开文件，并使用`ANSI`(GB2312)编码重新保存。此后合成使用编码修改后的文件。

3. `PSB Package` 中勾选 `JSON` 和 `Unpack Animation` 两项。将`evimage.xp3`拖进KrKrExtract的窗口，进行解码。解码后在 `./KrkrExtract_Output/evimage/` 得到 `*.json` 文件和同名目录，目录中包括 `<n>.tlg` 文件。json文件中包含了CG的相关信息，图层则在各个tlg文件中。

4. 将`*.json`、json同名目录及目录中`<n>.tlg`、ANSI编码的`cglist.csv`放在同一目录下，执行`CGMerge`

### Reference

1. <https://xmoeproject.github.io/KrkrExtract/#fh5co-info-unpack>
