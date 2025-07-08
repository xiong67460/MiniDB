//record_manager.h - 记录管理器头文件

#pragma once
#include <string>
#include <vector>
using namespace std;

//记录管理器类,提供对表中数据记录的各种操作

class RecordManager
{
public:
    static bool insertRecord(const string &tableName, const vector<string> &values);
    static vector<vector<string>> selectAll(const string &tableName);
    static vector<vector<string>> selectWhere(const string &tableName, const string &column, const string &value);
    static int deleteWhere(const string &tableName, const string &column, const string &value);
    static int updateWhere(const string &tableName, const string &setColumn, const string &setValue, const string &whereColumn, const string &whereValue);
    // 导出表为CSV文件
    static bool exportToCSV(const string &tableName, const string &filePath);
};
