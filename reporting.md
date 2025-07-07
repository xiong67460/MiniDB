#                                  项目报告  

##                                                     ——简易数据库引擎（MiniDB）    

####                                           姓名：么冠雄              学号:10234700475                                                                   

### 一、项目简介：

简易数据库引擎（MiniDB）是一个用 C++ 实现的简单数据库管理系统，支持以下的SQL基础操作：创建表格，删除表格，单次插入与删除数据，"单个等于条件"查询与更新数据。

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

```sql
select* from stu;
--select*from 表名； 查询表中全部数据
select* from stu where id=1;
--select*from 表名 where 属性=值; 条件查询
```

##### 5.更新数据

```sql
update stu set score=96 where name=ygx;
--update 表名 set 属性=值 where 属性=值;
```

##### 6.删除数据

```sql
delete from stu where id=1;
--delete fromm 表名 where 属性=值;
```


### 三、主要模块说明

- **main.cpp**：程序入口，命令行交互，分发SQL命令。
- **common/command.h**：定义各种SQL命令类型和命令类。
- **parser/**：SQL解析器，将SQL字符串转为命令对象。
- **catalog/**：目录管理器，负责表结构的创建和删除。
- **record/**：记录管理器，负责数据的插入、查询、删除、更新。
- **data/**：存放表数据文件（CSV格式）。
- **metadata/**：存放表结构元数据文件。

### 四、主要实现思路

- 命令模式：每种SQL操作对应一个命令类，便于扩展。
- 解析器：用字符串处理方法解析SQL，支持基本语法。
- 数据存储：数据以CSV格式存储在data目录，表结构信息存储在metadata目录。
- 逻辑删除：删除数据时在行首加#，不物理删除。
- 文件操作：用C++标准库读写文件，保证跨平台。

### 五、测试用例与效果

```sql
SQL> create table stu(id int,name string,score int);
Table 'stu' created successfully with 3 columns.

SQL> insert into stu values(1,ygx,99);
Successfully inserted 3 values into table 'stu'.

SQL> select* from stu;
Found 1 record(s) in table 'stu':
1       ygx     99

SQL> update stu set score=100 where name=ygx;
Successfully updated 1 record(s) in table 'stu' where name = ygx.

SQL> delete from stu where id=1;
Successfully deleted 1 record(s) from table 'stu' where id = 1.

SQL> drop table stu;
Table 'stu' dropped successfully.
```

### 六、遇到的问题与解决办法

- **SQL解析**：字符串分割和空格处理容易出错，采用统一的clean函数去除多余空格和分号。
- **文件操作**：文件路径和权限问题，使用std::filesystem保证兼容性。
- **数据一致性**：逻辑删除和更新时注意同步写回文件，避免数据丢失。

### 七、总结与展望

本项目实现了一个简单的数据库系统，支持基本的表和数据操作。通过本项目熟悉了C++文件操作、字符串处理和简单的模块化设计。后续可以考虑增加索引、事务、更多SQL语法等功能，让系统更强大。

