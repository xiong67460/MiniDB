//catalog_manager.cpp - 目录管理器实现

#include "catalog_manager.h"
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

//在metadata目录下创建表的元数据文件，记录表的结构信息
bool CatalogManager::createTable(const string &tableName, const vector<pair<string, string>> &columns)
{
    // 若metadata目录不存在进行创建
    fs::create_directory("metadata");

    // 生成元数据文件路径
    string filename = "metadata/" + tableName + ".meta";
    ofstream fout(filename);
    if (!fout.is_open())
        return false;

    // 写入表名
    fout << "Table: " << tableName << "\n";

    // 写入各列名与数据类型
    fout << "Columns:\n";
    for (const auto &[name, type] : columns)
    {
        fout << name << " " << type << "\n";
    }
    fout.close();
    return true;
}

bool CatalogManager::dropTable(const string &tableName)
{
    // 删除元数据文件与数据文件
    string metaFile = "metadata/" + tableName + ".meta";
    string dataFile = "data/" + tableName + ".tbl";
    bool metaRemoved = std::filesystem::remove(metaFile);
    bool dataRemoved = std::filesystem::remove(dataFile);
    return metaRemoved || dataRemoved;
}
