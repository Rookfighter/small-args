/*
 * smallargs.h
 *
 *  Created on: 6 Oct 2016
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

#ifndef INCLUDE_SMALLARGS_H_
#define INCLUDE_SMALLARGS_H_

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define SARG_VERSION "0.1"

#define SARG_ERR_SUCCESS       0
#define SARG_ERR_ERRNO        -1
#define SARG_ERR_OTHER        -2
#define SARG_ERR_INVALARG     -3
#define SARG_ERR_PARSE        -4
#define SARG_ERR_NOTFOUND     -5
#define SARG_ERR_ALLOC     -5

#define _SARG_UNUSED(e) ((void) e)
#define _SARG_IS_SHORT_ARG(s) (s[0] == '-' && s[1] != '-')
#define _SARG_IS_LONG_ARG(s) (s[0] == '-' && s[1] == '-')
#define _SARG_IS_HEX_NUM(s) (s[0] == '0' && s[1] == 'x')
#define _SARG_IS_OCT_NUM(s) (s[0] == '0' && strchr("1234567", s[1]) != NULL)

//#define SARG_FOR_EACH(root,res) for(res = &root.results[0]; res->next != NULL; res = res->next)

typedef enum _sarg_type {
	INT = 0,
	UINT,
	DOUBLE,
	BOOL,
	STRING,
	COUNT
} sarg_type;

typedef struct _sarg_argument {
	char *short_name;
	char *long_name;
	char *help;
	sarg_type type;
} sarg_argument;

typedef struct _sarg_result {
    sarg_type type;
    int count;
    union {
        int int_val;
        unsigned int uint_val;
        double double_val;
        int bool_val;
        char *str_val;
    };
} sarg_result;

typedef struct _sarg_root {
	sarg_argument *args;
	int arg_len;
	sarg_result *results;
	int res_len;
} sarg_root;

void _sarg_result_init(sarg_result *res, sarg_type type)
{
    memset(res, 0, sizeof(sarg_result));
    res->type = type;
}

void _sarg_result_destroy(sarg_result *res)
{
    if (res->type == STRING && res->str_val)
    {
        free(res->str_val);
        res->str_val = NULL;
    }
}

int sarg_init(sarg_root *root, sarg_argument *arguments, const int len)
{
    int i;

    // init and copy arguments
    root->args = malloc(sizeof(sarg_argument) * len);
    if(root->args == NULL)
        return SARG_ERR_ALLOC;

    memcpy(root->args, arguments, sizeof(sarg_argument) * len);
    root->arg_len = len;

    // init parsing result array
    root->results = malloc(sizeof(sarg_result) * len);
    if(root->args == NULL) {
        free(root->args);
        return SARG_ERR_ALLOC;
    }
    root->res_len = len;

    // init results
    for (i = 0; i < root->res_len; ++i)
        _sarg_result_init(&root->results[i], root->args[i].type);

    return SARG_ERR_SUCCESS;
}

int _sarg_find_arg(sarg_root *root, const char *name)
{
    int i, is_short, is_long;
    char *namep = (char*) name;

    while(namep[0] == '-')
        ++namep;

    for(i = 0; i < root->arg_len; ++i)
    {
        is_short = root->args[i].short_name && strcmp(namep, root->args[i].short_name) == 0;
        is_long = root->args[i].long_name && strcmp(namep, root->args[i].long_name) == 0;
        if (is_short || is_long)
            return i;
    }

    return -1;
}

int _sarg_get_number_base(const char *arg)
{
    int len = strlen(arg);
    int base = 10;

    // define base for number conversion
    if(len > 2)
    {
        if (_SARG_IS_OCT_NUM(arg))
            base = 8;
        if (_SARG_IS_HEX_NUM(arg))
            base = 16;
    }

    return base;
}

int _sarg_parse_int(const char *arg, sarg_result *res)
{
    int base;
    char *endptr;
    base = _sarg_get_number_base(arg);

    // convert and check if conversion succeeded
    res->int_val = strtol(arg, &endptr, base);
    if(*endptr != '\0')
        return SARG_ERR_PARSE;

    return SARG_ERR_SUCCESS;
}

int _sarg_parse_uint(const char *arg, sarg_result *res)
{
    int base;
    char *endptr;
    base = _sarg_get_number_base(arg);

    // convert and check if conversion succeeded
    res->uint_val = strtoul(arg, &endptr, base);
    if(*endptr != '\0')
        return SARG_ERR_PARSE;

    return SARG_ERR_SUCCESS;
}

int _sarg_parse_double(const char *arg, sarg_result *res)
{
    char *endptr;

    res->double_val = strtod(arg, &endptr);
    if(*endptr != '\0')
        return SARG_ERR_PARSE;

    return SARG_ERR_SUCCESS;
}

int _sarg_parse_bool(const char *arg, sarg_result *res)
{
    _SARG_UNUSED(arg);
    ++res->bool_val;

    return SARG_ERR_SUCCESS;
}

int _sarg_parse_str(const char *arg, sarg_result *res)
{
    if(res->str_val)
        free(res->str_val);

    res->str_val = malloc(strlen(arg) + 1);
    if(!res->str_val)
        return SARG_ERR_ALLOC;

    strcpy(res->str_val, arg);

    return SARG_ERR_SUCCESS;
}

typedef int (*_sarg_parse_func)(const char*, sarg_result*);
static _sarg_parse_func _sarg_parse_funcs[COUNT] = {
        _sarg_parse_int,
        _sarg_parse_uint,
        _sarg_parse_double,
        _sarg_parse_bool,
        _sarg_parse_str,
};

int sarg_parse(sarg_root *root, const char **argv, const int argc)
{
    int i, arg_idx, len, ret;

    // reset results
    for (i = 0; i < root->res_len; ++i) {
        _sarg_result_destroy(&root->results[i]);
        _sarg_result_init(&root->results[i], root->args[i].type);
    }

    for(i = 1; i < argc; ++i)
    {
        // argument has to have at least 2 chars
        len = strlen(argv[i]);
        if(len < 2)
            return SARG_ERR_PARSE;

        if(_SARG_IS_SHORT_ARG(argv[i]) || _SARG_IS_LONG_ARG(argv[i]))
        {
            // find argument
            arg_idx = _sarg_find_arg(root, argv[i]);
            if(arg_idx < 0)
                return SARG_ERR_NOTFOUND;

            if(root->args[arg_idx].type != BOOL)
            {
                ++i;
                if(i >= argc)
                    return SARG_ERR_PARSE;
            }

            ret = _sarg_parse_funcs[root->args[arg_idx].type](
                    argv[i], &root->results[arg_idx]);
            if(ret != SARG_ERR_SUCCESS)
                return ret;

            ++root->results[arg_idx].count;
        }
    }

    return SARG_ERR_SUCCESS;
}

int sarg_get(sarg_root *root, const char *name, sarg_result **res)
{
    int arg_idx;

    arg_idx = _sarg_find_arg(root, name);
    if(arg_idx < 0)
        return SARG_ERR_NOTFOUND;

    *res = &root->results[arg_idx];

    return SARG_ERR_SUCCESS;
}

int sarg_destroy(sarg_root *root)
{
    int i;
    if(root->results) {
        for(i = 0; i < root->res_len; ++i)
            _sarg_result_destroy(&root->results[i]);
        free(root->results);
    }
    root->results = NULL;
    root->res_len = -1;

    if(root->args)
        free(root->args);
    root->args = NULL;
    root->arg_len = -1;

    return SARG_ERR_SUCCESS;
}

#endif
