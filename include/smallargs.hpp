/*
 * smallargs.hpp
 *
 *  Created on: 2 Dec 2016
 *      Author: Fabian Meyer
 *
 * LICENSE
 * =======
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Fabian Meyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INCLUDE_SMALLARGS_HPP_
#define INCLUDE_SMALLARGS_HPP_

#include <cassert>
#include <string>
#include <stdexcept>
#include <sstream>

extern "C" {
#include "smallargs.h"
}

namespace sarg
{
    typedef sarg_opt opt;
    typedef sarg_result result;

    class Error : public std::exception
    {
    private:
        int errVal_;
        std::string errMsg_;

    public:
        Error(const int errVal) throw()
        : errVal_(errVal), errMsg_()
        {
            std::stringstream ss;
            ss << errVal_ << ": " << sarg_strerror(errVal);
            errMsg_ = ss.str();
        }

        ~Error() throw()
        {

        }

        const char* what() const throw()
        {
            return errMsg_.c_str();
        }

        int errval()
        {
            return errVal_;
        }
    };

    class Root
    {
    private:
        sarg_root root_;

        Root() {}
    public:
        Root(const opt *opts, const std::string& name)
        {
            int ret;

            ret = sarg_init(&root_, opts, name.c_str());
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);
        }

        Root(const Root& root)
        {
            //TODO
            _SARG_UNUSED(root);
            assert(false);
        }

        ~Root()
        {
            sarg_destroy(&root_);
        }

        const result& operator[](const std::string &key)
        {
            int ret;
            result *res;

            ret = sarg_get(&root_, key.c_str(), &res);
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);

            return *res;
        }

        void parse(const char **argv, const int argc)
        {
            int ret;

            ret = sarg_parse(&root_, argv, argc);
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);
        }

#ifndef SARG_NO_PRINT
        std::string getHelp()
        {
            std::string result;
            char *text;
            int ret;

            ret = sarg_help_text(&root_, &text);
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);

            result = std::string(text);
            free(text);

            return result;
        }

        void printHelp()
        {
            int ret;

            ret = sarg_help_print(&root_);
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);
        }
#endif

#ifndef SARG_NO_FILE
        void fromFile(const std::string &filename)
        {
            int ret;

            ret = sarg_parse_file(&root_, filename.c_str());
            if(ret != SARG_ERR_SUCCESS)
                throw Error(ret);
        }
#endif
    };
}

#endif
