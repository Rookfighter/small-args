/*
 * sample_cb.cpp
 *
 *  Created on: 14 May 2017
 *      Author: Fabian Meyer
 *
 * Example on how to access options using callbacks.
 */

#include <smallargs.hpp>
#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>

#define UNUSED(x) ((void)x)
#define FILENAME_LEN 256

static sarg::Root root("sarg_sample_cpp");
static std::string filename;
static bool gotFile = false;

static int helpCB(const sarg::result *res)
{
    UNUSED(res);
    root.printHelp();
    std::exit(0);
}

static int verboseCB(const sarg::result *res)
{
    std::cout << "verbosity set to " << res->count << std::endl;
    return SARG_ERR_SUCCESS;
}

static int countCB(const sarg::result *res)
{
    int i;
    std::cout << "counting: ";
    for(i = 0; i < res->int_val; ++i) {
        std::cout << i + 1;
        if(i + 1 != res->int_val)
            std::cout << ",";
    }
    std::cout << std::endl;
    return SARG_ERR_SUCCESS;
}

static int rootCB(const sarg::result *res)
{
    std::cout << std::sqrt(res->double_val) << std::endl;
    return SARG_ERR_SUCCESS;
}

static int sayCB(const sarg::result *res)
{
    std::cout << "you say:" << res->str_val << std::endl;
    return SARG_ERR_SUCCESS;
}

static int fileCB(const sarg::result *res)
{
    gotFile = true;
    filename = res->str_val;
    return SARG_ERR_SUCCESS;
}

int main(int argc, const char **argv)
{
    try {
        root.add("h", "help", "show help text", BOOL, helpCB)
            .add("v", "verbose", "increase verbosity", BOOL, verboseCB)
            .add("c", "count", "count up to this number", INT, countCB)
            .add(NULL, "root", "calculate square root of this number", DOUBLE, rootCB)
            .add(NULL, "say", "print the given text", STRING, sayCB)
            .add("f", "file", "file to read arguments from", STRING, fileCB)
            .init();

        root.parse(argv, argc);
    } catch (sarg::Error &e) {
        std::cout << "Parsing failed" << std::endl;
        root.printHelp();
        return -1;
    }

    if(gotFile) {
        try {
            root.fromFile(filename);
        } catch (sarg::Error &e) {
            std::cout << "Error: Parsing file failed" << std::endl <<
                    e.what() << std::endl;
            return -1;
        }
    }

    return 0;
}
