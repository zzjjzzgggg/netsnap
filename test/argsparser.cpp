
#include "argsparser.h"


const string ArgsParser::GetCmd() const {
    string cmd = args_[0];
    size_t pos = cmd.find_last_of('/');
    if (pos == string::npos) return cmd;
    else return cmd.substr(pos + 1);
}


const int ArgsParser::GetArgPos(const string& arg) const {
    int arg_pos = 0;
    for (auto&& a: args_) {
        if (a == arg) return arg_pos;
        arg_pos ++;
    }
    return -1;
}


const int ArgsParser::GetIntArg(const string& arg,
                                const int default_value,
                                const string& help) const {
    if(verbose_) {
        printf("\t%-8s %-8s %s (default: %d)\n", arg.c_str(), "[n]", help.c_str(), default_value);
    }
    int arg_pos = GetArgPos(arg);
    if (arg_pos<0 || num_args_ < arg_pos + 2)
        return default_value;
    return std::atoi(args_[arg_pos+1].c_str());
}

const string ArgsParser::GetStrArg(const string& arg, const string& default_value,
                                   const string& help) const {
    if(verbose_) {
        printf("\t%-8s %-8s %s (default: %s)\n", arg.c_str(), "[str]", help.c_str(),
               default_value.c_str());
    }
    int arg_pos = GetArgPos(arg);
    if (arg_pos<0 || num_args_ < arg_pos + 2)
        return default_value;
    return args_[arg_pos+1];
}


std::vector<int> ArgsParser::GetIntVecArg(const string& arg,
                                          const std::vector<int>& default_value,
                                          const string& help) const {
    std::vector<int> arg_ints;
    return arg_ints;
}

std::vector<string> ArgsParser::GetStrVecArg(const string& arg,
                                             const std::vector<string>& default_value,
                                             const string& help) const {
    std::vector<string> arg_strs;
    return arg_strs;
}
