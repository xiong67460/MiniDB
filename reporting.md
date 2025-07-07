#                                  项目报告  

##                                                     ——简易数据库引擎（MiniDB）    

####                                           姓名：么冠雄              学号10234700475                                                                   

### 一、项目简介：

简易数据库引擎（MiniDB）是一个用 C++ 实现的简单数据库管理系统，支持以下的SQL基础操作：创建表格，删除表格，单次插入与删除数据，“单个等于条件“查询与更新数据。

代码原码：https://github.com/xiong67460/MiniDB.git

##### 项目结构：

```markdown
MiniDB/
├── main.cpp                 # 主程序入口
├── common/
│   └── command.h           # 命令类定义
├── parser/
│   ├── parser.h            # SQL解析器头文件
│   └── parser.cpp          # SQL解析器实现
├── catalog/
│   ├── catalog_manager.h   # 目录管理器头文件
│   └── catalog_manager.cpp # 目录管理器实现
├── record/
│   ├── record_manager.h    # 记录管理器头文件
│   └── record_manager.cpp  # 记录管理器实现
├── data/                   # 数据文件目录
├── metadata/               # 元数据文件目录
└── README.md              # 项目说明文档
```

##### 项目编译与运行：

编译需要C++17或以上的版本，且需要编译器支持 `std::filesystem` 。

使用 g++ 编译:

```bash
g++ -std=c++17 -o MiniDB main.cpp parser/parser.cpp catalog/catalog_manager.cpp record/record_manager.cpp
```

也可以使用 clang++ 编译:

```bash
clang++ -std=c++17 -o MiniDB main.cpp parser/parser.cpp catalog/catalog_manager.cpp record/record_manager.cpp
```

在编译完成后可以双击MiniDB.exe文件或者在终端输入以下指令来运行项目

```bash
./MiniDB
```



### 二、项目功能

##### 1.创建表格：

```sql
create table stu(id int,name string,score int);
--create table 表名（属性1 数据类型，属性2 数据类型，...）;
```

##### 2.删除表格：

```sql
drop table stu;
--drop table 表名;
```

##### 3.插入数据：

```sql
insert into stu values(1,ygx,99);
--insert into stu values(属性1的值，属性2的值...);
```

##### 4.查询数据