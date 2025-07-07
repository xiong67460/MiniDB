/**
 * record_manager.cpp - 记录管理器实现
 * 实现了对数据库表中数据记录的各种操作
 */

#include "record_manager.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;
namespace fs = filesystem;

/**
 * 清理字符串的辅助函数
 * 去除字符串首尾的双引号
 * @param s 要清理的字符串
 * @return 清理后的字符串
 */
string cleanStr(const string &s)
{
    string res = s;
    if (!res.empty() && res.front() == '"' && res.back() == '"')
    {
        res = res.substr(1, res.size() - 2);
    }
    return res;
}

/**
 * 插入记录到指定表
 * 将数据以CSV格式追加到数据文件中
 * @param tableName 表名
 * @param values 要插入的值列表
 * @return 插入成功返回true，失败返回false
 */
bool RecordManager::insertRecord(const string &tableName, const vector<string> &values)
{
    // 确保data目录存在
    fs::create_directory("data");

    // 生成数据文件路径
    string filename = "data/" + tableName + ".tbl";
    ofstream fout(filename, ios::app); // 以追加模式打开文件
    if (!fout.is_open())
        return false;

    // 将值写入文件，用逗号分隔
    for (size_t i = 0; i < values.size(); ++i)
    {
        fout << values[i];
        if (i != values.size() - 1)
            fout << ",";
    }
    fout << "\n";
    fout.close();
    return true;
}

/**
 * 查询表中的所有记录
 * 读取数据文件并解析CSV格式的数据
 * @param tableName 表名
 * @return 返回所有记录的二维向量
 */
vector<vector<string>> RecordManager::selectAll(const string &tableName)
{
    string filename = "data/" + tableName + ".tbl";
    ifstream fin(filename);
    vector<vector<string>> result;

    if (!fin.is_open())
        return result;

    string line;
    while (getline(fin, line))
    {
        // 跳过空行和已删除的记录（以#开头）
        if (line.empty() || line[0] == '#')
            continue;

        // 解析CSV格式的行数据
        stringstream ss(line);
        string field;
        vector<string> row;
        while (getline(ss, field, ','))
        {
            row.push_back(field);
        }
        result.push_back(row);
    }

    return result;
}

/**
 * 根据条件查询记录
 * 支持WHERE子句的条件查询
 * @param tableName 表名
 * @param column 查询的列名
 * @param value 查询的值
 * @return 返回符合条件的记录
 */
vector<vector<string>> RecordManager::selectWhere(const string &tableName, const string &column, const string &value)
{
    string filename = "data/" + tableName + ".tbl";
    ifstream fin(filename);
    vector<vector<string>> result;
    if (!fin.is_open())
        return result;

    // 工具函数：去除首尾空格
    auto trim = [](string s) -> string
    {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c)
                                        { return !isspace(c); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](char c)
                             { return !isspace(c); })
                    .base(),
                s.end());
        return s;
    };

    // 工具函数：去除首尾空格和包裹的双引号
    auto cleanStr = [&](string s) -> string
    {
        s = trim(s);
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        {
            s = s.substr(1, s.size() - 2);
        }
        return s;
    };

    string cleanedValue = cleanStr(value);

    // 从元数据文件获取字段名
    vector<string> columns;
    ifstream meta("metadata/" + tableName + ".meta");
    string line;
    while (getline(meta, line))
    {
        if (line.find("Columns:") != string::npos)
            break;
    }
    while (getline(meta, line))
    {
        if (line.empty())
            break;
        stringstream ss(line);
        string colName;
        ss >> colName;
        columns.push_back(colName);
    }

    // 找出目标列的位置索引
    int index = -1;
    for (int i = 0; i < columns.size(); ++i)
    {
        if (columns[i] == column)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return result;

    // 扫描数据文件并匹配条件
    while (getline(fin, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        stringstream ss(line);
        string field;
        vector<string> row;

        while (getline(ss, field, ','))
            row.push_back(field);

        // 检查指定列的值是否匹配
        if (row.size() > index && trim(row[index]) == trim(cleanedValue))
        {
            result.push_back(row);
        }
    }

    return result;
}

/**
 * 根据条件删除记录
 * 采用逻辑删除方式，在记录前添加#标记
 * @param tableName 表名
 * @param column 删除条件的列名
 * @param value 删除条件的值
 * @return 返回删除的记录数量
 */
int RecordManager::deleteWhere(const string &tableName, const string &column, const string &value)
{
    auto trim = [](string s) -> string {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c) { return !isspace(c); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](char c) { return !isspace(c); }).base(), s.end());
        return s;
    };

    string filename = "data/" + tableName + ".tbl";
    ifstream fin(filename);
    vector<string> lines;
    if (!fin.is_open())
        return 0;

    // 从元数据文件获取字段名
    vector<string> columns;
    ifstream meta("metadata/" + tableName + ".meta");
    string line;
    while (getline(meta, line))
    {
        if (line.find("Columns:") != string::npos)
            break;
    }
    while (getline(meta, line))
    {
        if (line.empty())
            break;
        stringstream ss(line);
        string colName;
        ss >> colName;
        columns.push_back(colName);
    }

    // 找出目标列的位置索引
    int index = -1;
    for (int i = 0; i < columns.size(); ++i)
    {
        if (columns[i] == column)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return 0;

    // 处理每一行数据
    int count = 0;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            lines.push_back(line);
            continue;
        }
        if (line[0] == '#')
        {
            lines.push_back(line);
            continue;
        }

        // 解析行数据
        stringstream ss(line);
        string field;
        vector<string> row;

        while (getline(ss, field, ','))
            row.push_back(field);
            
        // 检查是否匹配删除条件
        if (row.size() > index && trim(row[index]) == trim(value))
        {
            lines.push_back("#" + line); // 标记为删除
            count++;
        }
        else
        {
            lines.push_back(line);
        }
    }

    // 重写文件
    fin.close();
    ofstream fout(filename);
    for (const auto &l : lines)
        fout << l << "\n";
    return count;
}

/**
 * 根据条件更新记录
 * 采用逻辑更新方式，在记录前添加#标记
 * @param tableName 表名
 * @param setColumn 要更新的列名
 * @param setValue 要更新的值
 * @param whereColumn 删除条件的列名
 * @param whereValue 删除条件的值
 * @return 返回更新的记录数量
 */
int RecordManager::updateWhere(const string &tableName, const string &setColumn, const string &setValue, const string &whereColumn, const string &whereValue)
{
    auto trim = [](string s) -> string {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c) { return !isspace(c); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](char c) { return !isspace(c); }).base(), s.end());
        return s;
    };

    string filename = "data/" + tableName + ".tbl";
    ifstream fin(filename);
    vector<string> lines;
    if (!fin.is_open())
        return 0;

    // 从元数据文件获取字段名
    vector<string> columns;
    ifstream meta("metadata/" + tableName + ".meta");
    string line;
    while (getline(meta, line))
    {
        if (line.find("Columns:") != string::npos)
            break;
    }
    while (getline(meta, line))
    {
        if (line.empty())
            break;
        stringstream ss(line);
        string colName;
        ss >> colName;
        columns.push_back(colName);
    }

    // 找出目标列的位置索引
    int setIdx = -1, whereIdx = -1;
    for (int i = 0; i < columns.size(); ++i)
    {
        if (columns[i] == setColumn)
            setIdx = i;
        if (columns[i] == whereColumn)
            whereIdx = i;
    }
    if (setIdx == -1 || whereIdx == -1)
        return 0;

    // 处理每一行数据
    int count = 0;
    while (getline(fin, line))
    {
        if (line.empty())
        {
            lines.push_back(line);
            continue;
        }
        if (line[0] == '#')
        {
            lines.push_back(line);
            continue;
        }
        stringstream ss(line);
        string field;
        vector<string> row;
        while (getline(ss, field, ','))
            row.push_back(field);
        if (row.size() > whereIdx && trim(row[whereIdx]) == trim(whereValue))
        {
            if (row.size() > setIdx)
            {
                row[setIdx] = setValue;
                count++;
            }
        }
        // 重新拼接行
        string newline;
        for (size_t i = 0; i < row.size(); ++i)
        {
            newline += row[i];
            if (i != row.size() - 1)
                newline += ",";
        }
        lines.push_back(newline);
    }
    fin.close();
    ofstream fout(filename);
    for (const auto &l : lines)
        fout << l << "\n";
    return count;
}
