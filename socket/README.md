
## [HTTP vs SOCKET][1]

[Socket][2]

[C Socket][3]

[粘包原理][4]

[分包和拆包][5]
## TCP vs UDP
#### TCP：
1. [基于面向连接的协议。][6]
2. 可靠性和数据包的序列性是有保证的。
3. 自动为你的数据封包。
4. 确保包的流量不会超出你的网络链接的负载上限。
5. 简单易用，就像操作文件一样。
6. [会发生粘包][7]。
 
#### UDP：
1. 没有连接的概念，如果你想要，自己去实现去。
2. 没有关于可靠性和包序列性的保证，包可能会丢失，重复，乱序。
3. 你必须自己去封包。
4. 你必须自己确保自己的数据包不会流量过大从而导致超过链接负载上限。
5. 你必须自己处理包的丢失，重复，乱序的情况，如果你不想他们对你的程序造成麻烦，必须要自己实现代码来做出应对。

## [JSON][8]
```
{"key": "value"}
{"key": 1}
{"key": [1, 2, 3]}
{"key": {"key1": "value1"}}
[{"key1": "value1"}, {"key2": "value2"}, {"key3": "value3"}]
```
key必须是字符串，value可以是任意类型(int, String, Array...)或者Json格式

[1]: http://www.cnblogs.com/meier1205/p/5971313.html
[2]: http://blog.csdn.net/ccit0519/article/details/24790971
[3]: http://blog.csdn.net/lovekun1989/article/details/41042273
[4]: http://blog.csdn.net/junecau/article/details/6889324
[5]: http://www.cnblogs.com/QG-whz/p/5537447.html
[6]: http://www.cnblogs.com/Jessy/p/3536163.html
[7]: http://www.cnblogs.com/visily/archive/2013/03/15/2961190.html
[8]: https://baike.baidu.com/item/JSON/2462549?fr=aladdin
