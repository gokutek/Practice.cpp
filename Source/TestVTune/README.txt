1. VTune的安裝目录下有include、lib32、lib64几个目录，将它们将到搜索路径中去；
2. 链接库的输入里加上：libittnotify.lib；
2. 包含头文件："ittnotify.h"；
3. To be able to see user tasks in your results, enable the Analyze user tasks checkbox in analysis settings；
4. API的用法可参考帮助文档：[User's Guide]、[API Support]；
