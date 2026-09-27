# Qt/C++ 上位机必学与面试高频清单

> 标注说明：⭐ 重要程度；🔥 面试必考；💡 加分项

## ① C++ 基础 ⭐⭐⭐⭐⭐

### 1. 语言核心
- 指针 / 引用 / const / static / extern / volatile 🔥
- 内存分区：栈、堆、全局/静态区、常量区、代码区 🔥
- new/delete 与 malloc/free 区别 🔥
- 深拷贝 / 浅拷贝 / 拷贝构造 / 赋值运算符重载 🔥
- 运算符重载
- 继承 / 多态 / 虚函数 / 纯虚函数 / 抽象类 / 虚析构 🔥
- 虚函数表 vtable / vptr / RTTI / dynamic_cast 🔥
- 四种 cast：static_cast、dynamic_cast、const_cast、reinterpret_cast 🔥
- 模板 / 泛型 / 特化 / 可变参数模板
- 异常 / noexcept / RAII 🔥
- 命名空间 / 友元 / explicit / override / final
- 编译链接：预处理、编译、汇编、链接 / 静态库 / 动态库 / ABI

### 2. 现代 C++（C++11/14/17/20）
- 智能指针：unique_ptr、shared_ptr、weak_ptr 🔥
- 循环引用与 weak_ptr 解决 🔥
- Lambda / 捕获列表 / mutable / 泛型 Lambda 🔥
- std::function / std::bind
- 左值 / 右值 / 右值引用 / 移动构造 / 移动赋值 🔥
- 完美转发 / std::forward / 万能引用 🔥
- auto / decltype / nullptr / constexpr / using
- 范围 for / 结构化绑定 / 初始化列表
- 并发：std::thread、mutex、condition_variable、atomic、future、async 🔥

### 3. STL
- vector / list / deque / array / string
- map / set / unordered_map / unordered_set 🔥
- 迭代器 / 迭代器失效 🔥
- 容器底层：vector 扩容、map 红黑树、unordered_map 哈希表 🔥
- 算法：sort、find、transform、accumulate
- 函数对象 / 仿函数 / std::function

### 4. 多线程与并发 ⭐⭐⭐⭐⭐
- 线程创建 / join / detach
- 互斥锁 / 读写锁 / 自旋锁 / 条件变量 / 信号量 🔥
- 死锁产生条件与避免 🔥
- 原子操作 / 内存序 / 无锁队列
- 线程池 / 任务队列
- 跨线程通信 / 线程安全 / 可重入

## ② Qt UI ⭐⭐⭐⭐⭐

### 1. Qt 核心机制
- QObject / 元对象系统 / MOC 🔥
- 信号槽机制 / connect 第五参数 / 队列连接 / 直接连接 🔥
- 事件循环 / QEvent / 事件过滤器 / 自定义事件 🔥
- 父子对象树 / 内存管理 / deleteLater 🔥
- 属性系统 Q_PROPERTY / Q_INVOKABLE
- 定时器 QTimer
- 国际化 / 资源系统 / qrc

### 2. Qt Widgets
- 常用控件：QPushButton、QLabel、QLineEdit、QComboBox、QTableWidget
- 布局：QHBoxLayout、QVBoxLayout、QGridLayout、QFormLayout
- 主窗口：QMainWindow、菜单、工具栏、状态栏、Dock
- 对话框：QDialog、QMessageBox、QFileDialog
- Model/View：QAbstractItemModel、QTableView、QTreeView、QListView、Delegate 🔥
- QSS 样式表 / 自定义控件 / QPainter
- QGraphicsView / QGraphicsScene
- 图表：Qt Charts、QCustomPlot

### 3. QML / Qt Quick
- QML 语法 / 属性绑定 / 信号与槽
- Qt Quick Controls 2
- Model/View / ListView / GridView
- C++ 与 QML 交互：Q_PROPERTY、Q_INVOKABLE、qmlRegisterType 🔥
- QQmlApplicationEngine / QQuickView
- Qt Quick 布局 / 动画 / 状态机
- Widgets 与 QML 选型对比 🔥

## ③ 工业通信 ⭐⭐⭐⭐⭐

### 1. 串口通信
- QSerialPort / 串口参数：波特率、数据位、停止位、校验位、流控 🔥
- 异步读写 / readyRead / 粘包与拆包 🔥
- RS232 / RS485 / TTL 区别 🔥
- 超时 / 重连 / 心跳

### 2. Modbus
- Modbus RTU / Modbus TCP 🔥
- 功能码：01/02/03/04/05/06/0F/10 🔥
- 寄存器：线圈、离散输入、输入寄存器、保持寄存器 🔥
- CRC16 / 字节序 / 大小端 🔥
- Qt Modbus 模块 / libmodbus
- 主站 / 从站 / 轮询 / 异常码

### 3. CAN / CANopen
- CAN 帧格式：标准帧、扩展帧、数据帧、远程帧 🔥
- CAN 波特率 / 仲裁 / 错误处理
- CANopen：PDO、SDO、NMT、对象字典
- Qt CAN / SocketCAN / Linux CAN
- CAN 分析仪 / 报文解析

### 4. 其他工业协议
- OPC UA
- MQTT
- Profinet / EtherCAT / Modbus Plus（了解）
- 自定义二进制协议 / 协议设计 / 状态机 🔥

## ④ 网络通信 ⭐⭐⭐⭐⭐

### 1. TCP / UDP
- TCP 三次握手 / 四次挥手 🔥
- TCP 粘包 / 拆包 / 封包 🔥
- QTcpServer / QTcpSocket / QUdpSocket
- 心跳 / 重连 / 超时 / 断线处理 🔥
- 字节序 / 网络字节序 / 大小端转换 🔥
- 并发服务器：多线程 / 线程池 / select / epoll

### 2. 应用层协议
- HTTP / HTTPS / QNetworkAccessManager
- WebSocket
- MQTT
- JSON / XML / Protobuf
- RESTful API

### 3. Boost.Asio
- io_context / 异步模型
- TCP / UDP / 定时器
- 线程池 / strand
- 与 Qt 事件循环集成

## ⑤ 工程化 ⭐⭐⭐⭐

### 1. 构建与版本
- CMake 🔥
- qmake / Qt Creator
- Git：分支、合并、rebase、冲突、tag 🔥
- Linux：常用命令、Shell、权限、进程、网络、GDB 🔥
- 交叉编译 / 部署 / 打包
- windeployqt / linuxdeployqt / Qt Installer Framework

### 2. 日志与调试
- spdlog / Qt 日志
- 日志分级 / 滚动 / 异步
- GDB / Valgrind / AddressSanitizer
- 内存泄漏 / 性能分析 / 火焰图
- 单元测试：GoogleTest / Qt Test 🔥
- CI/CD：Jenkins / GitLab CI / GitHub Actions

### 3. 数据与配置
- JSON / QJsonDocument / nlohmann/json
- XML / INI / QSettings
- 数据库：SQLite / MySQL / QSqlDatabase
- 文件 IO / QFile / QDataStream / QTextStream

## ⑥ 图形 / 视觉 / 视频 ⭐⭐⭐

### 1. 图形
- QPainter / 2D 绘图
- OpenGL / QOpenGLWidget / 着色器
- QGraphicsView 框架
- Qt Charts / QCustomPlot
- 数据可视化 / 实时曲线

### 2. 视觉
- OpenCV：Mat、图像读写、滤波、边缘、形态学、模板匹配
- 相机采集：USB / GigE / 工业相机 SDK
- 图像格式 / 颜色空间 / 像素格式 🔥
- 标定 / 测量 / 缺陷检测（了解）

### 3. 视频
- GStreamer / FFmpeg
- RTSP / RTMP / H.264 / H.265
- 视频解码 / 渲染 / 同步
- Qt Multimedia / QVideoWidget

## ⑦ 面试高频问题速查 🔥

### C++ 方向
1. 多态的实现原理？虚函数表是什么？🔥
2. 为什么基类析构要虚析构？🔥
3. shared_ptr 循环引用怎么解决？🔥
4. 左值、右值、移动语义、完美转发？🔥
5. vector 扩容机制？迭代器为什么会失效？🔥
6. map 和 unordered_map 区别？🔥
7. 死锁的条件与避免？🔥
8. 线程同步方式有哪些？🔥
9. static、const、volatile 的作用？🔥
10. 内存泄漏、野指针、悬空指针？🔥

### Qt 方向
1. 信号槽原理？connect 第五参数？🔥
2. 队列连接与直接连接区别？跨线程信号槽？🔥
3. QThread 正确用法？moveToThread？🔥
4. Qt 事件循环是什么？🔥
5. QObject 父子对象内存管理？deleteLater？🔥
6. Qt 线程同步有哪些类？🔥
7. QSerialPort 异步读写与粘包处理？🔥
8. QTcpSocket 粘包怎么解决？🔥
9. Model/View 原理？🔥
10. Widgets 与 QML 如何选型？🔥

### 工业通信方向
1. Modbus RTU 与 TCP 区别？🔥
2. Modbus 功能码与寄存器？🔥
3. CRC16 怎么计算？🔥
4. 大小端与字节序？🔥
5. CAN 标准帧与扩展帧？🔥
6. CANopen PDO/SDO 区别？🔥
7. TCP 三次握手四次挥手？🔥
8. 心跳、重连、断线重连设计？🔥
9. 自定义协议如何设计？🔥
10. 状态机在通信中的应用？🔥

### 项目与架构
1. 上位机整体架构怎么设计？🔥
2. 多线程如何划分？UI 线程与工作线程？🔥
3. 如何做日志、异常处理、配置管理？🔥
4. 如何优化界面卡顿？🔥
5. 如何打包部署 Qt 程序？🔥
6. 如何做单元测试与 CI/CD？🔥
7. 串口/网络通信断线重连？🔥
8. 大数据量实时曲线怎么优化？🔥
9. 如何保证通信可靠性？🔥
10. 你做过的项目难点与解决方案？🔥

## ⑧ 学习优先级建议

1. C++ 基础 + 多线程 + STL 🔥
2. Qt 核心机制 + Widgets + Model/View 🔥
3. 串口 + Modbus + TCP/UDP 🔥
4. CMake + Git + Linux + 调试 🔥
5. 工业协议 + 项目架构 + 性能优化 🔥
6. OpenCV / OpenGL / GStreamer（按岗位选学）