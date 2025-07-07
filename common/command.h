/**
 * command.h - 命令类定义
 * 定义了数据库操作的各种命令类型和数据结构
 * 采用命令模式设计，便于扩展和维护
 */

#pragma once
#include <string>
#include <vector>
using namespace std;

/**
 * 命令类型枚举
 * 定义了系统支持的所有SQL操作类型
 */
enum class CommandType
{
    CREATE,  // 创建表
    INSERT,  // 插入数据
    SELECT,  // 查询数据
    DELETE,  // 删除数据
    UPDATE,  // 更新数据
    DROP,    // 删除表
    UNKNOWN  // 未知命令
};

/**
 * 命令基类
 * 所有具体命令类的父类，定义了命令的基本接口
 */
class Command
{
public:
    CommandType type = CommandType::UNKNOWN; // 命令类型
    virtual ~Command() = default; // 虚析构函数，支持多态
};

/**
 * 创建表命令类
 * 用于处理CREATE TABLE语句
 */
class CreateCommand : public Command
{
public:
    string tableName; // 表名
    vector<pair<string, string>> columns; // 列定义：(列名, 数据类型)
};

/**
 * 插入数据命令类
 * 用于处理INSERT INTO语句
 */
class InsertCommand : public Command
{
public:
    string tableName; // 表名
    vector<string> values; // 要插入的值列表
};

/**
 * 查询数据命令类
 * 用于处理SELECT语句
 */
class SelectCommand : public Command
{
public:
    string tableName; // 表名
    string condition; // WHERE条件（格式：column=value）
};

/**
 * 删除数据命令类
 * 用于处理DELETE FROM语句
 */
class DeleteCommand : public Command
{
public:
    string tableName; // 表名
    string condition; // WHERE条件（格式：column=value）
};

/**
 * 更新数据命令类
 * 用于处理UPDATE语句
 */
class UpdateCommand : public Command
{
public:
    string tableName; // 表名
    string setColumn; // 要更新的列名
    string setValue;  // 要更新的新值
    string condition; // WHERE条件（格式：column=value）
};

/**
 * 删除表命令类
 * 用于处理DROP TABLE语句
 */
class DropCommand : public Command
{
public:
    string tableName; // 表名
};
