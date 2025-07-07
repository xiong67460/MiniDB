# MiniDB - 简单数据库管理系统

## 项目简介

MiniDB 是一个用 C++ 实现的简单数据库管理系统，支持基本的 SQL 操作。该项目由 Yao Guanxiong 开发，主要用于学习和演示数据库系统的基本原理。

## 功能特性

### 支持的 SQL 操作

1. **CREATE TABLE** - 创建新表
   ```sql
   CREATE TABLE student (id int, name string, age int);
   ```

2. **INSERT INTO** - 插入数据
   ```sql
   INSERT INTO student VALUES (1, "张三", 20);
   ```

3. **SELECT** - 查询数据
   ```sql
   SELECT * FROM student;                    -- 查询所有记录
   SELECT * FROM student WHERE name="张三";   -- 条件查询
   ```

4. **DELETE FROM** - 删除数据
   ```sql
   DELETE FROM student WHERE id=1;
   ```

5. **DROP TABLE** - 删除表
   ```sql
   DROP TABLE student;
   ```

6. **UPDATE** - 更新数据
   ```sql
   UPDATE student SET score=100 WHERE name="张三";
   ```

### 系统特性

- **文件存储**: 数据以 tbl 格式存储在 `data/` 目录
- **元数据管理**: 表结构信息存储在 `metadata/` 目录
- **逻辑删除**: 删除操作采用逻辑删除方式，在记录前添加 `#` 标记
- **交互式界面**: 提供命令行交互界面
- **智能输入**: 自动处理前导空格和尾部空格、分号
- **专业提示**: 提供详细的操作反馈和错误信息

## 项目结构

```
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

## 编译和运行

### 编译要求

- C++17 或更高版本
- 支持 `std::filesystem` 的编译器

### 编译命令

```bash
# 使用 g++ 编译
g++ -std=c++17 -o MiniDB main.cpp parser/parser.cpp catalog/catalog_manager.cpp record/record_manager.cpp

# 使用 clang++ 编译
clang++ -std=c++17 -o MiniDB main.cpp parser/parser.cpp catalog/catalog_manager.cpp record/record_manager.cpp
```

### 运行程序

```bash
./MiniDB
```

## 使用示例

启动程序后，你会看到欢迎信息：

```
hello, welcome to MiniDB by YGX
Type 'exit' to quit

SQL> 
```

### 创建表

```sql
SQL> CREATE TABLE student (id int, name string, age int);
Table 'student' created successfully with 3 columns.
```

### 插入数据

```sql
SQL> INSERT INTO student VALUES (1, "张三", 20);
Successfully inserted 3 values into table 'student'.

SQL> INSERT INTO student VALUES (2, "李四", 22);
Successfully inserted 3 values into table 'student'.
```

### 查询数据

```sql
SQL> SELECT * FROM student;
Found 2 record(s) in table 'student':
----------------------------------------
1       张三      20
2       李四      22
----------------------------------------

SQL> SELECT * FROM student WHERE name="张三";
Found 1 record(s) in table 'student' where name = 张三:
----------------------------------------
1       张三      20
----------------------------------------
```

### 删除数据

```sql
SQL> DELETE FROM student WHERE id=1;
Successfully deleted 1 record(s) from table 'student' where id = 1.
```

### 删除表

```sql
SQL> DROP TABLE student;
Table 'student' dropped successfully.
```

### 智能输入处理

程序会自动处理前导空格和尾部空格、分号：

```sql
SQL>    CREATE TABLE test (id int, name string);    -- 前导空格会被自动去除
Table 'test' created successfully with 2 columns.

SQL> SELECT * FROM student;  -- 尾部空格和分号会被自动去除
Found 1 record(s) in table 'student':
----------------------------------------
2       李四      22
----------------------------------------
```

### 错误处理

当操作失败时，系统会提供详细的错误信息：

```sql
SQL> SELECT * FROM nonexistent_table;
No records found in table 'nonexistent_table'.

SQL> DELETE FROM student WHERE id=999;
No records found in table 'student' where id = 999 to delete.

SQL> INVALID SQL COMMAND;
Unrecognized SQL command. Supported commands:
  - CREATE TABLE <table_name> (<column_definitions>)
  - INSERT INTO <table_name> VALUES (<values>)
  - SELECT * FROM <table_name> [WHERE <condition>]
  - DELETE FROM <table_name> WHERE <condition>
```

### 退出程序

```sql
SQL> exit

Thank you for using MiniDB. Goodbye!
```

### 更新数据

```sql
SQL> UPDATE student SET score=100 WHERE name="张三";
Successfully updated 1 record(s) in table 'student' where name = 张三.
```

## 技术实现

### 架构设计

- **命令模式**: 使用命令模式处理不同的 SQL 操作
- **模块化设计**: 将功能分为解析器、目录管理器、记录管理器等模块
- **文件存储**: 采用简单的文件系统存储数据

### 数据存储

- **数据文件**: 存储在 `data/表名.tbl` 文件中，采用 CSV 格式
- **元数据文件**: 存储在 `metadata/表名.meta` 文件中，记录表结构
- **逻辑删除**: 删除的记录在行首添加 `#` 标记

### 解析器

- 支持基本的 SQL 语法解析
- 使用字符串匹配和分割技术
- 生成相应的命令对象
- 智能处理前导空格和尾部空格、分号

### 用户界面

- 专业的操作反馈信息
- 详细的错误提示和帮助信息
- 清晰的数据展示格式
- 友好的退出提示

## 扩展建议

1. **索引支持**: 添加 B+ 树索引提高查询性能
2. **事务管理**: 实现 ACID 事务特性
3. **并发控制**: 添加锁机制支持多用户访问
4. **SQL 扩展**: 支持更多 SQL 语法（如 JOIN、GROUP BY 等）
5. **数据类型**: 支持更多数据类型（如 DATE、FLOAT 等）

## 作者

**Yao Guanxiong** - MiniDB 项目开发者

## 许可证

本项目仅供学习和研究使用。 