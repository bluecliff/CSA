CSA
===

压缩后缀数组(Compressed suffix array)的实现，采用了vitter论文中的第一种实现，逐层压缩
实现了一个CSA类，可以完成压缩后缀数组的创建，存储，读取，暂时未实现在CSA上的模式查询。
包含CSA类的定义头文件后，CSA的构建直接调用构造函数即可。具体可参考buildcsa.cpp示例文件
