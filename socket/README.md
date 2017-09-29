
[Socket][1]

[C Socket][2]

[粘包原理][3]

[分包和拆包][4]
## TCP vs UDP
#### TCP：
1. [基于面向连接的协议。][5]
2. 可靠性和数据包的序列性是有保证的。
3. 自动为你的数据封包。
4. 确保包的流量不会超出你的网络链接的负载上限。
5. 简单易用，就像操作文件一样。
6. [会发生粘包][6]。
 
#### UDP：
1. 没有连接的概念，如果你想要，自己去实现去。
2. 没有关于可靠性和包序列性的保证，包可能会丢失，重复，乱序。
3. 你必须自己去封包。
4. 你必须自己确保自己的数据包不会流量过大从而导致超过链接负载上限。
5. 你必须自己处理包的丢失，重复，乱序的情况，如果你不想他们对你的程序造成麻烦，必须要自己实现代码来做出应对。

[1]: http://blog.csdn.net/ccit0519/article/details/24790971
[2]: http://blog.csdn.net/lovekun1989/article/details/41042273
[3]: http://blog.csdn.net/junecau/article/details/6889324
[4]: http://lib.csdn.net/article/computernetworks/19124
[5]: http://www.cnblogs.com/Jessy/p/3536163.html
[6]: http://www.cnblogs.com/visily/archive/2013/03/15/2961190.html
