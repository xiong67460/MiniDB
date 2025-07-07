/**
 * parser.cpp - SQL解析器实现
 * 实现了SQL语句的解析逻辑，支持基本的SQL语法
 */

#include "parser.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <string>
#include <memory>
using namespace std;

/**
 * 清理字符串的辅助函数
 * 去除首尾空格和末尾分号
 * @param s 要清理的字符串
 * @return 清理后的字符串
 */
string clean(string s)
{
    // 去除前导空格
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](char c)
                                     { return !isspace(c); }));
    // 去除尾部空格和分号
    s.erase(find_if(s.rbegin(), s.rend(), [](char c)
                          { return !isspace(c) && c != ';'; }).base(),
             s.end());
    return s;
}

/**
 * 解析SQL语句的主函数
 * 根据SQL语句的类型创建相应的命令对象
 * @param sql 要解析的SQL字符串
 * @return 解析后的命令对象指针
 */
unique_ptr<Command> Parser::parse(const string &sql)
{
    // 转换为小写以便进行关键字匹配
    string lower = sql;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // 解析CREATE TABLE语句
    if (lower.find("create table") == 0)
    {
        auto cmd = make_unique<CreateCommand>();
        cmd->type = CommandType::CREATE;

        // 提取表名
        size_t start = lower.find("table") + 6;
        size_t paren = sql.find('(', start);
        string tableName = sql.substr(start, paren - start);
        cmd->tableName = clean(tableName);

        // 解析列定义
        size_t endParen = sql.find(')', paren);
        string fields = sql.substr(paren + 1, endParen - paren - 1);
        stringstream ss(fields);
        string segment;
        while (getline(ss, segment, ','))
        {
            stringstream part(segment);
            string colName, colType;
            part >> colName >> colType;
            cmd->columns.emplace_back(colName, colType);
        }
        return cmd;
    }

    // 解析INSERT INTO语句
    if (lower.find("insert into") == 0)
    {
        auto cmd = make_unique<InsertCommand>();
        cmd->type = CommandType::INSERT;

        // 提取表名
        size_t intoPos = lower.find("into") + 4;
        size_t valPos = lower.find("values", intoPos);
        string tableName = sql.substr(intoPos, valPos - intoPos);
        cmd->tableName = clean(tableName);

        // 解析值列表
        size_t lParen = sql.find('(', valPos);
        size_t rParen = sql.find(')', lParen);
        string valuesStr = sql.substr(lParen + 1, rParen - lParen - 1);

        stringstream ss(valuesStr);
        string val;
        while (getline(ss, val, ','))
        {
            cmd->values.push_back(clean(val));
        }
        return cmd;
    }

    // 解析SELECT语句
    if (lower.find("select") == 0)
    {
        auto cmd = make_unique<SelectCommand>();
        cmd->type = CommandType::SELECT;

        // 提取表名和条件
        size_t fromPos = lower.find("from");
        if (fromPos == string::npos)
            return cmd;

        size_t wherePos = lower.find("where", fromPos);
        string tableName;
        if (wherePos != string::npos)
        {
            tableName = sql.substr(fromPos + 4, wherePos - fromPos - 4);
            cmd->condition = sql.substr(wherePos + 5);
        }
        else
        {
            tableName = sql.substr(fromPos + 4);
        }
        cmd->tableName = clean(tableName);
        return cmd;
    }

    // 解析DELETE FROM语句
    if (lower.find("delete from") == 0)
    {
        auto cmd = make_unique<DeleteCommand>();
        cmd->type = CommandType::DELETE;

        // 提取表名和条件
        size_t fromPos = lower.find("from") + 4;
        size_t wherePos = lower.find("where", fromPos);
        string tableName;

        if (wherePos != string::npos)
        {
            tableName = sql.substr(fromPos, wherePos - fromPos);
            cmd->condition = sql.substr(wherePos + 5);
        }
        else
        {
            tableName = sql.substr(fromPos);
        }
        cmd->tableName = clean(tableName);
        return cmd;
    }

    // 解析UPDATE语句
    if (lower.find("update") == 0)
    {
        auto cmd = make_unique<UpdateCommand>();
        cmd->type = CommandType::UPDATE;

        // 提取表名
        size_t setPos = lower.find("set");
        string tableName = sql.substr(6, setPos - 6);
        cmd->tableName = clean(tableName);

        // 提取SET子句
        size_t wherePos = lower.find("where", setPos);
        string setClause;
        if (wherePos != string::npos)
        {
            setClause = sql.substr(setPos + 3, wherePos - (setPos + 3));
            cmd->condition = sql.substr(wherePos + 5);
        }
        else
        {
            setClause = sql.substr(setPos + 3);
            cmd->condition = "";
        }
        setClause = clean(setClause);
        size_t eq = setClause.find('=');
        cmd->setColumn = clean(setClause.substr(0, eq));
        cmd->setValue = clean(setClause.substr(eq + 1));
        return cmd;
    }

    // 解析DROP TABLE语句
    if (lower.find("drop table") == 0)
    {
        auto cmd = make_unique<DropCommand>();
        cmd->type = CommandType::DROP;
        size_t start = lower.find("table") + 5;
        string tableName = sql.substr(start);
        cmd->tableName = clean(tableName);
        return cmd;
    }

    // 未知命令类型
    auto cmd = make_unique<Command>();
    cmd->type = CommandType::UNKNOWN;
    return cmd;
}
