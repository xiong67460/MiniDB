/**
 * MiniDB - 一个简单的数据库管理系统
 * 作者: YGX
 * 功能: 支持基本的SQL操作，包括CREATE TABLE、INSERT、SELECT、DELETE
 */

#include "parser/parser.h"
#include "catalog/catalog_manager.h"
#include "record/record_manager.h"
#include <vector>
#include <memory>
using namespace std;
#include <iostream>
#include <algorithm>
#include <fstream>

/**
 * 清理SQL语句的辅助函数
 * 去除首尾空格和末尾分号
 * @param sql 要清理的SQL语句
 * @return 清理后的SQL语句
 */
string cleanSQL(string sql)
{
    // 去除前导空格
    sql.erase(sql.begin(), find_if(sql.begin(), sql.end(), [](char c)
                                   { return !isspace(c); }));
    // 去除尾部空格和分号
    sql.erase(find_if(sql.rbegin(), sql.rend(), [](char c)
                      { return !isspace(c) && c != ';'; }).base(),
              sql.end());
    return sql;
}

/**
 * 主函数 - 数据库系统的入口点
 * 提供交互式SQL命令行界面
 */
int main()
{
    string sql; // 存储用户输入的SQL语句
    
    // 只在程序启动时显示一次欢迎信息
    cout << "hello, welcome to MiniDB by YGX\n";
    cout << "Type 'exit' to quit\n\n";
    
    // 主循环：持续接收用户输入直到退出
    while (true)
    {
        cout << "SQL> "; // 提示符
        getline(cin, sql); // 读取用户输入的SQL语句
        
        // 清理SQL语句，去除前导空格和尾部空格、分号
        sql = cleanSQL(sql);
        
        // 检查退出命令
        if (sql == "exit")
            break;

        // 跳过空输入
        if (sql.empty())
            continue;

        // 解析SQL语句，生成命令对象
        auto cmd = Parser::parse(sql);
        
        // 根据命令类型执行相应的操作
        if (cmd->type == CommandType::CREATE)
        {
            // 处理CREATE TABLE命令
            auto create = static_cast<CreateCommand *>(cmd.get());
            if (CatalogManager::createTable(create->tableName, create->columns))
            {
                cout << "Table '" << create->tableName << "' created successfully with " 
                     << create->columns.size() << " columns.\n";
            }
            else
            {
                cout << "Failed to create table '" << create->tableName << "'. "
                     << "Please check if the table already exists or you have write permissions.\n";
            }
        }
        else if (cmd->type == CommandType::INSERT)
        {
            // 处理INSERT命令
            auto insert = static_cast<InsertCommand *>(cmd.get());
            if (RecordManager::insertRecord(insert->tableName, insert->values))
            {
                cout << "Successfully inserted " << insert->values.size() 
                     << " values into table '" << insert->tableName << "'.\n";
            }
            else
            {
                cout << "Failed to insert data into table '" << insert->tableName << "'. "
                     << "Please check if the table exists and the data format is correct.\n";
            }
        }
        else if (cmd->type == CommandType::SELECT)
        {
            // 处理SELECT命令
            auto select = static_cast<SelectCommand *>(cmd.get());
            auto trim = [](string s) -> string {
                s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c) { return !isspace(c); }));
                s.erase(find_if(s.rbegin(), s.rend(), [](char c) { return !isspace(c); }).base(), s.end());
                return s;
            };
            if (select->condition.empty())
            {
                // 无条件查询：返回所有记录
                auto result = RecordManager::selectAll(select->tableName);
                if (result.empty())
                {
                    cout << "No records found in table '" << select->tableName << "'.\n";
                }
                else
                {
                    cout << "Found " << result.size() << " record(s) in table '" << select->tableName << "':\n";
                    cout << "----------------------------------------\n";
                    for (const auto &row : result)
                    {
                        for (const auto &f : row)
                            cout << f << "\t";
                        cout << "\n";
                    }
                    cout << "----------------------------------------\n";
                }
            }
            else
            {
                // 条件查询：根据WHERE条件筛选记录
                size_t eq = select->condition.find('=');
                string col = trim(select->condition.substr(0, eq));
                string val = trim(select->condition.substr(eq + 1));
                auto result = RecordManager::selectWhere(select->tableName, col, val);
                if (result.empty())
                {
                    cout << "No records found in table '" << select->tableName 
                         << "' where " << col << " = " << val << ".\n";
                }
                else
                {
                    cout << "Found " << result.size() << " record(s) in table '" << select->tableName 
                         << "' where " << col << " = " << val << ":\n";
                    cout << "----------------------------------------\n";
                    for (const auto &row : result)
                    {
                        for (const auto &f : row)
                            cout << f << "\t";
                        cout << "\n";
                    }
                    cout << "----------------------------------------\n";
                }
            }
        }
        else if (cmd->type == CommandType::DELETE)
        {
            // 处理DELETE命令
            auto del = static_cast<DeleteCommand *>(cmd.get());
            auto trim = [](string s) -> string {
                s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c) { return !isspace(c); }));
                s.erase(find_if(s.rbegin(), s.rend(), [](char c) { return !isspace(c); }).base(), s.end());
                return s;
            };
            size_t eq = del->condition.find('=');
            string col = trim(del->condition.substr(0, eq));
            string val = trim(del->condition.substr(eq + 1));
            int count = RecordManager::deleteWhere(del->tableName, col, val);
            if (count > 0)
            {
                cout << "Successfully deleted " << count << " record(s) from table '" 
                     << del->tableName << "' where " << col << " = " << val << ".\n";
            }
            else
            {
                cout << "No records found in table '" << del->tableName 
                     << "' where " << col << " = " << val << " to delete.\n";
            }
        }
        else
        {
            // 未知命令类型
            cout << "Unrecognized SQL command. Supported commands:\n";
            cout << "  - CREATE TABLE <table_name> (<column_definitions>)\n";
            cout << "  - INSERT INTO <table_name> VALUES (<values>)\n";
            cout << "  - SELECT * FROM <table_name> [WHERE <condition>]\n";
            cout << "  - DELETE FROM <table_name> WHERE <condition>\n";
        }
    }
    
    cout << "\nThank you for using MiniDB. Goodbye!\n";
    return 0;
}
