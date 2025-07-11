// command.h - 命令类定义
#pragma once
#include <string>
#include <vector>
using namespace std;

//枚举定义了SQL操作类型
enum class CommandType
{
    CREATE,  // 创建表
    INSERT,  // 插入数据
    SELECT,  // 查询数据
    DELETE,  // 删除数据
    UPDATE,  // 更新数据
    DROP,    // 删除表
    EXPORT,  // 导出表为CSV
    UNKNOWN  // 未知命令
};

// 命令基类
class Command
{
public:
    CommandType type = CommandType::UNKNOWN; // 命令类型,默认为UNKOWN
    virtual ~Command() = default; //定义虚析构函数
};

//各命令子类
//CREATE TABLE
class CreateCommand : public Command//公共继承命令基类
{
public:
    string tableName; 
    vector<pair<string, string>> columns; 
};

//INSERT INTO
class InsertCommand : public Command
{
public:
    string tableName; 
    vector<string> values; 
};

//SELECT
class SelectCommand : public Command
{
public:
    string tableName; 
    string condition; 
};

//DELETE FROM
class DeleteCommand : public Command
{
public:
    string tableName; 
    string condition; 
};

//UPDATE
class UpdateCommand : public Command
{
public:
    string tableName; 
    string setColumn; 
    string setValue;  
    string condition; 
};

//DROP TABLE
class DropCommand : public Command
{
public:
    string tableName; 
};

//EXPORT TABLE ... TO ...
class ExportTableCommand : public Command
{
public:
    string tableName;
    string filePath;
};
