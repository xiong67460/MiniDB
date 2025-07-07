//catalog_manager.h - 目录管理器头文件

#pragma once
#include <string>
#include <vector>
using namespace std;
class CatalogManager
{
public:
   //创建新表
    static bool createTable(const string &tableName, const vector<pair<string, string>> &columns);
    //删除表
    static bool dropTable(const string &tableName);
};
