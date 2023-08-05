#pragma once

#include <string>

namespace CONSTANTS
{
    const std::string kProgramName = "DoveCompiler";

    const std::string kPrebuiltDecl = R"C(
    int getint(){}
    int getarray(int a[]) {}
    int getch() {}
    void putint(int n) {}
    void putarray(int n, int a[]) {}
    void putch(int c) {}
    )C";
}