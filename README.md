# Pokemon 

### 2016 BUPT C++ Programming Experiment -- Pokemon

----

## Change  Log

### v0.1.0 (2016/10/9)

* 完成Pokemon基类
* TODO 
  * 派生类覆盖虚函数 

  * 单元测试


### v0.1.1 (2016/10/10)

* 改善构造函数
* 初步完成工厂模式
* 优化了随机产生属性的方法
* TODO
  * 特殊攻击函数加血bug修复
  * 用户自定义小精灵名字 当前无名字属性
  * 单元测试
  * 派生类的Evolution函数


### v0.1.2 (2016/10/11)

* 特殊攻击函数封装 加血bug修复
* 输出小精灵状态
* 修复细节bug
* 将Evolution函数改为Pokemon类的成员函数
* TODO
  * 放弃自定义小精灵名字
  * 单元测试

###  v0.1.3 (2016/10/12)

* 修复升级和进化bug
* 完成单元测试
* 注意: Create口袋妖怪时应将Kind和Level匹配才可以成功创建


### v0.1.4 (2016/10/13)

* 确定C/S框架 原源程序在Server端运行 将QWidget程序改为QConsole程序
* 存在自定义小精灵名字的方法
* TODO
  * server端数据库
  * server client 间 socket通信

### v0.1.5 (2016/10/14)

* 创建Player类
* Client端窗口切换
* 使用SQLite3维护数据库
* TODO
  * 学习ORMLite动态创建数据库
  * server client间 socket通信

### v0.1.6 (2016/10/21)

* 完成PoorORMLite 添加`PoorORMLite.h`到*server* 

* TODO

  *   创建与`Class Player`对应的`Struct Player`存储`Player` 信息 `Primary Key`为`name`
  *   创建与`Class Pokemon`对应的`Struct Pokemon`存储`Pokemon`信息 `Primary Key`为`name` 并存储`owner`作为`condition: Where`

### v0.1.7(2016/11/19)

* 对`Player`测试创建新玩家对象并输出信息
* 数据库小精灵实例化 传`struct PokemonInfo` 
* 捕捉精灵 对战精灵实例化 传`kind level name`
* 玩家实例化 传`struct PlayerInfo`新玩家新信息 老玩家从数据库获取
* TODO

  * 将`Player` 和 `Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话


### v0.1.8(2016/11/20)

* `Player`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句不正确)
* 新增`Helper.h`用于消除未知的头文件包含问题
* TODO 
  * 将`Pokemon` 与数据库链接测试
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.1.9(2016/11/21)

* `Pokemon`与数据库测试连接
* `PoorORMLite`修复bug(`insert`语句*sqlCmd*中*where*前无空格 导致数字无法得到识别 但字符串可以识别)
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话

### v0.2.0(2016/11/23)

* `PokemonServer`初步完成`Sign In登录` `Sign On注册`界面及跳转
* TODO
  * 将`Socket Server`和`Socket Client`分别添加至*project server* 和 *project client*
  * 添加`session.h` `session.cpp`测试会话