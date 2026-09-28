**C++11/14 核心知识 + Boost.Asio + 网络编程**，最后做成一个真正能跑的“消息收发服务”，面试时也很好讲。

我给你设计成一个**由浅入深的项目大纲**，你可以直接拿这个当学习路线。

# C++ + Boost.Asio 收发服务实战

## 一、项目目标

最终实现一个：

```text
┌─────────────┐
│   Client A  │
└──────┬──────┘
       │ TCP
       ↓
┌─────────────────────────┐
│      Asio Server        │
│                         │
│  接收 → 解包 → 处理     │
│              ↓          │
│         消息分发         │
│              ↓          │
│  编码 → 发送             │
└────────────┬────────────┘
             │
             ↓
       ┌───────────┐
       │  Client B │
       └───────────┘
```

最终你至少实现：

* TCP 服务端
* TCP 客户端
* 多客户端连接
* 异步收发
* 消息队列
* 消息分发
* 心跳
* 断线检测
* 自动重连
* 粘包/半包处理
* 简单协议
* 多线程
* 日志
* RAII
* 智能指针
* Lambda
* `std::function`
* `std::move`
* `std::thread`
* `mutex`
* `condition_variable`
* `atomic`

---

# 二、第一阶段：C++ 基础

## 1. 指针与引用

* [ ] 指针
* [ ] 引用
* [ ] `const`
* [ ] `const` 与指针
* [ ] `nullptr`
* [ ] 生命周期
* [ ] 悬空指针
* [ ] 内存泄漏

```cpp
int* p = nullptr;
这是空指针，不是悬空指针。

int* p;
这是未初始化指针，里面是随机值，也叫野指针。

int* p = new int(10);
delete p;
这是典型的悬空指针。
```
## 2. RAII

* [ ] 构造函数
* [ ] 析构函数
* [ ] 作用域
* [ ] RAII 思想
* [ ] 资源自动释放

项目中：

```text
Socket
Mutex
File
Memory
```

都可以用 RAII 思想管理。

---

# 三、第二阶段：智能指针

## 3. `unique_ptr`

* [ ] `std::unique_ptr`
* [ ] `make_unique`
* [ ] 移动所有权
* [ ] 为什么不能复制

项目：

```cpp
std::unique_ptr<Server> server;
```

---

## 4. `shared_ptr`

* [ ] `std::shared_ptr`
* [ ] `make_shared`
* [ ] 引用计数
* [ ] `use_count()`
* [ ] 共享所有权

项目：

```cpp
std::shared_ptr<ClientSession>
```

多个模块共同管理一个客户端连接对象。

---

## 5. `weak_ptr`

* [ ] `weak_ptr`
* [ ] `lock()`
* [ ] `expired()`
* [ ] 循环引用

项目：

```text
Server
  ↓
shared_ptr
  ↓
Session
  ↓
weak_ptr
  ↓
Server
```

---

# 四、第三阶段：移动语义

## 6. 左值 / 右值

* [ ] 左值
* [ ] 右值
* [ ] 左值引用
* [ ] 右值引用

---

## 7. `std::move`

* [ ] 移动语义
* [ ] 移动构造
* [ ] 移动赋值
* [ ] `std::move`

项目：

```cpp
Message msg;

queue.push(std::move(msg));
```

---

## 8. 完美转发

* [ ] `T&&`
* [ ] 万能引用
* [ ] `std::forward`

后期再做，不用一开始就啃。

---

# 五、第四阶段：函数与 Lambda

## 9. 函数指针

```cpp
void (*callback)();
```

* [ ] 函数地址
* [ ] 函数指针
* [ ] 回调

---

## 10. `std::function`

```cpp
std::function<void()> callback;
```

* [ ] 可调用对象
* [ ] Lambda
* [ ] 函数指针
* [ ] 函数对象
* [ ] 回调函数

项目：

```cpp
using MessageHandler =
    std::function<void(const Message&)>;
```

---

## 11. Lambda

* [ ] `[=]`
* [ ] `[&]`
* [ ] `[this]`
* [ ] `mutable`
* [ ] 参数
* [ ] 返回值

Asio 里面会大量使用：

```cpp
socket.async_read_some(
    buffer,
    [this](auto ec, auto bytes) {
        ...
    }
);
```

所以这个必须熟。

---

# 六、第五阶段：STL

## 12. `vector`

* [ ] `push_back`
* [ ] `emplace_back`
* [ ] `size`
* [ ] `capacity`
* [ ] 扩容

项目：

```cpp
std::vector<char> buffer;
```

---

## 13. `unordered_map`

⭐⭐⭐⭐⭐

项目直接实现：

```cpp
std::unordered_map<
    UserId,
    std::shared_ptr<ClientSession>
>
```

实现：

```text
userId
  ↓
ClientSession
  ↓
Socket
```

---

## 14. `queue`

实现：

```text
生产者
   ↓
Message Queue
   ↓
消费者
```

---

## 15. STL 算法

* [ ] `find`
* [ ] `sort`
* [ ] `remove_if`
* [ ] `for_each`
* [ ] `transform`

---

# 七、第六阶段：Boost.Asio 基础 ⭐⭐⭐⭐⭐

这里开始进入你的核心项目。

## 16. `io_context`

理解：

> **Asio 的事件循环核心。**

```cpp
boost::asio::io_context io;
io.run();
```

---

## 17. TCP Socket

掌握：

```cpp
boost::asio::ip::tcp::socket
```

理解：

```text
Socket
   ↓
TCP连接
```

---

## 18. Acceptor

```cpp
boost::asio::ip::tcp::acceptor
```

负责：

```text
监听端口
   ↓
接收客户端
   ↓
创建 Socket
```

---

# 八、第七阶段：先写同步 TCP

不要一上来就异步。

## 19. 同步 Server

实现：

```text
Server
 ↓
Accept
 ↓
Read
 ↓
Process
 ↓
Write
```

功能：

* [ ] 创建 `io_context`
* [ ] 创建 `acceptor`
* [ ] `accept`
* [ ] `read`
* [ ] `write`
* [ ] 关闭 Socket

---

## 20. 同步 Client

实现：

```text
Connect
 ↓
Send
 ↓
Receive
 ↓
Print
```

做到：

```text
Client A → Server → Client A
```

---

# 九、第八阶段：异步 TCP ⭐⭐⭐⭐⭐

这是整个项目最重要的部分。

## 21. `async_accept`

```cpp
acceptor.async_accept(...)
```

理解：

```text
等待连接
   ↓
不阻塞线程
   ↓
连接到达
   ↓
回调执行
```

---

## 22. `async_read`

掌握：

```cpp
boost::asio::async_read(...)
```

---

## 23. `async_write`

掌握：

```cpp
boost::asio::async_write(...)
```

---

## 24. 异步 Session

设计：

```text
Server
  │
  ├── Session A
  │
  ├── Session B
  │
  └── Session C
```

每一个客户端对应一个：

```cpp
ClientSession
```

负责：

```text
Socket
读取
解析
发送
断开
```

---

# 十、第九阶段：多客户端

## 25. Session 管理

```cpp
std::unordered_map<
    uint64_t,
    std::shared_ptr<ClientSession>
> sessions;
```

实现：

* [ ] 添加客户端
* [ ] 删除客户端
* [ ] 查找客户端
* [ ] 广播消息
* [ ] 指定客户端发送

---

## 26. 广播

```text
        Server
       /  |  \
      /   |   \
     A    B    C

A → Server

Server → B
       → C
```

---

# 十一、第十阶段：消息协议 ⭐⭐⭐⭐⭐

这里就是你即时通讯项目最有价值的部分。

## 27. 解决 TCP 粘包 / 半包

理解：

```text
TCP = 字节流
```

不是：

```text
一个 write
↓
对应一个 read
```

设计：

```text
┌────────┬──────────┬─────────┐
│ Magic  │ Length   │ Payload │
│ 2 Byte │ 4 Byte   │ N Byte  │
└────────┴──────────┴─────────┘
```

---

## 28. PacketCodec

自己实现：

```cpp
class PacketCodec
{
public:
    void append(const char* data, size_t size);

    bool tryDecode(Message& message);
};
```

内部维护：

```text
接收到的数据
      ↓
Buffer
      ↓
检查完整包
      ↓
解析
      ↓
Message
```

---

# 十二、第十一阶段：消息队列

## 29. 线程安全队列 ⭐⭐⭐⭐⭐

自己实现：

```cpp
template<typename T>
class ThreadSafeQueue
{
};
```

使用：

```text
Network Thread
       ↓
   MessageQueue
       ↓
Worker Thread
```

使用：

* [ ] `mutex`
* [ ] `condition_variable`
* [ ] `unique_lock`
* [ ] `queue`

---

# 十三、第十二阶段：多线程

## 30. `std::thread`

实现：

```text
Main Thread
     │
     ├── Network Thread
     │
     └── Worker Thread
```

---

## 31. `mutex`

保护：

```cpp
sessions
```

---

## 32. `lock_guard`

```cpp
std::lock_guard<std::mutex> lock(mutex);
```

---

## 33. `unique_lock`

配合：

```cpp
condition_variable
```

---

## 34. `condition_variable`

实现：

```text
没有消息
   ↓
Worker 等待

来了消息
   ↓
notify_one()

Worker 唤醒
   ↓
处理消息
```

---

## 35. `atomic`

例如：

```cpp
std::atomic<bool> running;
```

控制服务启动 / 停止。

---

# 十四、第十三阶段：Asio 多线程模型 ⭐⭐⭐⭐⭐

重点理解：

```cpp
io_context.run();
```

可以由多个线程调用：

```text
             io_context
            /          \
           /            \
     Thread 1         Thread 2
```

然后研究：

```cpp
boost::asio::strand
```

解决：

> **同一个 Session 的异步操作如何避免并发执行冲突。**

这个非常值得面试讲。

---

# 十五、第十四阶段：心跳

实现：

```text
Client
  │
  │ heartbeat
  ↓
Server
  │
  │ heartbeat
  ↓
Client
```

使用：

```cpp
boost::asio::steady_timer
```

实现：

* [ ] 定时发送心跳
* [ ] 超时检测
* [ ] 断开异常客户端

---

# 十六、第十五阶段：自动重连

客户端：

```text
连接断开
    ↓
等待
    ↓
重新连接
    ↓
连接成功
    ↓
恢复通信
```

可以加入：

```text
1s
2s
4s
8s
...
```

指数退避。

---

# 十七、第十六阶段：日志

使用你已经接触过的：

```text
spdlog
```

记录：

```text
[INFO] Client connected
[INFO] Message received
[INFO] Message sent
[WARN] Client timeout
[ERROR] Socket error
```

---

# 十八、第十七阶段：最终项目架构

最终可以整理成：

```text
TcpMessageService
│
├── Server
│   ├── Acceptor
│   └── SessionManager
│
├── ClientSession
│   ├── Socket
│   ├── Read
│   ├── Write
│   ├── Heartbeat
│   └── SendQueue
│
├── Protocol
│   ├── Packet
│   ├── PacketCodec
│   └── Message
│
├── Message
│   ├── MessageQueue
│   └── MessageDispatcher
│
├── Thread
│   ├── NetworkThread
│   └── WorkerThread
│
├── Common
│   ├── ThreadSafeQueue
│   └── Logger
│
└── Client
    ├── Connector
    ├── Session
    └── Reconnect
```

# 🎯 你最后应该达到的能力

完成这个项目以后，你应该能够自己解释：

```text
C++11/14
│
├── 指针 / 引用
├── RAII
├── 智能指针
├── 左值 / 右值
├── 移动语义
├── Lambda
├── std::function
├── STL
├── 模板
│
├── 多线程
│   ├── thread
│   ├── mutex
│   ├── condition_variable
│   └── atomic
│
└── Boost.Asio
    ├── io_context
    ├── socket
    ├── acceptor
    ├── async_read
    ├── async_write
    ├── strand
    ├── timer
    └── 多客户端
```

**我尤其建议你按这个顺序做：**

> **同步 TCP → 异步 TCP → Session → 多客户端 → 协议 → 粘包半包 → 消息队列 → 多线程 → Strand → 心跳 → 重连**

不要一开始就写“大而全”。**每完成一个阶段，就把对应的 C++ 知识点写进你的面试笔记，并能用代码解释它为什么存在。**这样这套东西最后会变成一个非常完整的 C++ 网络项目，而不是又一份背出来的八股。
