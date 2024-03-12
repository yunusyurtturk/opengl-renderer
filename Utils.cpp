#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#ifndef VULKAN
#include <GL/glew.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

#include "Utils.h"

bool ReadFile(const char* pFileName, string& outFile)
{
    ifstream f(pFileName);

    bool ret = false;

    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();

        ret = true;
    }

    return ret;
}