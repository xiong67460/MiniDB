/**
 * catalog_manager.cpp - 目录管理器实现
 * 实现了表的创建和元数据文件的生成
 */

#include "catalog_manager.h"
#include <fstream>
#include <filesystem>
using namespace std;

namespace fs = filesystem;

/**
 * 创建新表
 * 在metadata目录下创建表的元数据文件，记录表的结构信息
 * @param tableName 表名
 * @param columns 列定义列表
 * @return 创建成功返回true，失败返回false
 */
bool CatalogManager::createTable(const string &tableName, const vector<pair<string, string>> &columns)
{
    // 确保metadata目录存在
    fs::create_directory("metadata");

    // 生成元数据文件路径
    string filename = "metadata/" + tableName + ".meta";
    ofstream fout(filename);
    if (!fout.is_open())
        return false;

    // 写入表的基本信息
    fout << "Table: " << tableName << "\n";
    fout << "Columns:\n";
    
    // 写入列定义信息
    for (const auto &[name, type] : columns)
    {
        fout << name << " " << type << "\n";
    }
    fout.close();
    return true;
}

bool CatalogManager::dropTable(const string &tableName)
{
    // 删除元数据文件
    string metaFile = "metadata/" + tableName + ".meta";
    string dataFile = "data/" + tableName + ".tbl";
    bool metaRemoved = std::filesystem::remove(metaFile);
    bool dataRemoved = std::filesystem::remove(dataFile);
    return metaRemoved || dataRemoved;
}
