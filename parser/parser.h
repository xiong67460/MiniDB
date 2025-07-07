/**
 * parser.h - SQL解析器头文件
 * 负责将SQL语句解析为命令对象
 * 支持基本的SQL语法解析
 */

#pragma once
#include "../common/command.h"
#include <memory>
#include <string>
using namespace std;

// UpdateCommand前向声明（如有需要）
class UpdateCommand;
// DropCommand前向声明（如有需要）
class DropCommand;

/**
 * SQL解析器类
 * 提供静态方法将SQL字符串解析为相应的命令对象
 */
class Parser
{
public:
    /**
     * 解析SQL语句
     * @param sql 要解析的SQL字符串
     * @return 解析后的命令对象指针
     */
    static unique_ptr<Command> parse(const string &sql);
};
