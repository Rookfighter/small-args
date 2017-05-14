/*
 * sample_get.cpp
 *
 *  Created on: 03 Dec 2016
 *      Author: Fabian Meyer
 *
 * Example on how to access options using the index operator.
 */

#include <smallargs.hpp>
#include <iostream>
#include <cmath>
#include <cassert>

int main(const int argc, const char **argv)
{
    sarg::Root root("sarg_sample_cpp");
    int i;

    try {
        root.add("h", "help", "show help text", BOOL, NULL)
            .add("v", "verbose", "increase verbosity", BOOL, NULL)
            .add("c", "count", "count up to this number", INT, NULL)
            .add(NULL, "root", "calculate square root of this number", DOUBLE, NULL)
            .add(NULL, "say", "print the given text", STRING, NULL)
            .add("f", "file", "file to read arguments from", STRING, NULL)
            .init();

        root.parse(argv, argc);
    } catch (sarg::Error &e) {
        std::cout << "Parsing failed" << std::endl;
        root.printHelp();
        return -1;
    }

    // check if help was set
    if(root["help"].bool_val) {
        root.printHelp();
        return 0;
    }

    // check for file
    if(root["f"].count) {
        try {
            root.fromFile(root["f"].str_val);
        } catch (sarg::Error &e) {
            std::cout << "Error: Parsing file failed" << std::endl <<
                    e.what() << std::endl;
            return -1;
        }
    }

    // check for verbosity
    std::cout << "verbosity set to " << root["v"].count << std::endl;

    // count to the given number
    if(root["count"].count > 0) {
        std::cout << "counting: ";
        for(i = 0; i < root["count"].int_val; ++i) {
            std::cout << i + 1;
            if(i + 1 != root["count"].int_val)
                std::cout << ",";
        }
        std::cout << std::endl;
    }

    // calculate square root of given number
    if(root["root"].count > 0)
        std::cout << std::sqrt(root["root"].double_val) << std::endl;

    // say something
    if(root["say"].count > 0)
        std::cout << "you say: " << root["say"].str_val << std::endl;

    return 0;
}
