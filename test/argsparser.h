#ifndef __ARGSPARSER_H__
#define __ARGSPARSER_H__

#include <stdio.h>
#include <cstdlib>

#include <cassert>
#include <string>
#include <vector>

typedef std::string string;

class ArgsParser {
private:
    int num_args_;
    std::vector<string> args_;
    bool verbose_;
private:
    const int GetArgPos(const string& arg) const;
public:
    ArgsParser(const int args, char** argv): num_args_(args), verbose_(false) {
        for (int i=0; i<args; i++) args_.push_back(argv[i]);
        if(args == 2 && (args_[1] == "-h" || args_[1] == "--help"))
            verbose_ = true;
        if(verbose_) {
            printf("usage: %s\n", GetCmd().c_str());
        }
    }
    virtual ~ArgsParser() {}

    const string GetCmd() const;

    const int GetIntArg(const string& arg, const int default_value,
                        const string& help) const;

    const string GetStrArg(const string& arg, const string& default_value,
                           const string& help) const;

    // don't worry about returning a vector, as a move operation will be
    // used under c++11.
    std::vector<int> GetIntVecArg(const string& arg,
                                  const std::vector<int>& default_value,
                                  const string& help) const;

    std::vector<string> GetStrVecArg(const string& arg,
                                     const std::vector<string>& default_value,
                                     const string& help) const;

    const bool IsEnd() const { return verbose_; }
};



#endif /* __ARGSPARSER_H__ */
