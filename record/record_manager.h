/**
 * record_manager.h - 记录管理器头文件
 * 负责管理数据库表中的数据记录，包括增删改查操作
 * 实现了数据的持久化存储和检索功能
 */

#pragma once
#include <string>
#include <vector>
using namespace std;

/**
 * 记录管理器类
 * 提供对表中数据记录的各种操作
 */
class RecordManager
{
public:
    /**
     * 插入记录
     * @param tableName 表名
     * @param values 要插入的值列表
     * @return 插入成功返回true，失败返回false
     */
    static bool insertRecord(const string &tableName, const vector<string> &values);

    /**
     * 查询所有记录
     * @param tableName 表名
     * @return 返回所有记录的二维向量
     */
    static vector<vector<string>> selectAll(const string &tableName);

    /**
     * 条件查询记录
     * @param tableName 表名
     * @param column 查询的列名
     * @param value 查询的值
     * @return 返回符合条件的记录
     */
    static vector<vector<string>> selectWhere(const string &tableName, const string &column, const string &value);

    /**
     * 条件删除记录
     * @param tableName 表名
     * @param column 删除条件的列名
     * @param value 删除条件的值
     * @return 返回删除的记录数量
     */
    static int deleteWhere(const string &tableName, const string &column, const string &value);
};
