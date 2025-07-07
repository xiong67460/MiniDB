/**
 * catalog_manager.h - 目录管理器头文件
 * 负责管理数据库的元数据信息，包括表的创建和结构定义
 * 实现了数据库的目录管理功能
 */

#pragma once
#include <string>
#include <vector>
using namespace std;

/**
 * 目录管理器类
 * 提供表的创建和元数据管理功能
 */
class CatalogManager
{
public:
    /**
     * 创建新表
     * @param tableName 表名
     * @param columns 列定义列表，每个元素为(列名, 数据类型)对
     * @return 创建成功返回true，失败返回false
     */
    static bool createTable(const string &tableName, const vector<pair<string, string>> &columns);

    /**
     * 删除表
     * @param tableName 表名
     * @return 删除成功返回true，失败返回false
     */
    static bool dropTable(const string &tableName);
};
