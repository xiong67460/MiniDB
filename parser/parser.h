//parser.h - SQL解析器头文件

#pragma once
#include "../common/command.h"
#include <memory>
#include <string>
using namespace std;

// SQL解析器类,将SQL字符串解析为相应的命令对象
class Parser
{
public:
   
    static unique_ptr<Command> parse(const string &sql);
};
