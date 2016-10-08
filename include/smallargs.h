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

#define SARG_VERSION "0.1"

#define SARG_ERR_SUCCESS       0
#define SARG_ERR_ERRNO        -1
#define SARG_ERR_OTHER        -2
#define SARG_ERR_INVALARG     -3

#define _SARG_IS_SHORT_ARG(s) (s[0] == '-' && s[1] != '-')
#define _SARG_IS_LONG_ARG(s) (s[0] == '-' && s[1] == '-')
#define _SARG_IS_HEX_NUM(s) (s[0] == '0' && s[1] == 'x')
#define _SARG_IS_OCT_NUM(s) (s[0] == '0' && s[1] == 'o')

typedef enum _sarg_type {
	INT = 0,
	UINT,
	DOUBLE,
	BOOL,
	STRING
} sarg_type;

typedef struct _smarg_argument {
	char *short_name;
	char *long_name;
	char *help;
	sarg_type type;
} sarg_argument;

typedef struct _sarg_result {
    sarg_type type;
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
    memset(res, 0, sizeof(res));
    res.type = type;
}

void _sarg_result_destroy(sarg_result *res)
{
    if (res->type == STRING && res->str_val)
        free(res->str_val);
}

int sarg_init(sarg_root *root, sarg_argument *arguments, const int len)
{
    int i;

    // init and copy arguments
    root->args = malloc(sizeof(sarg_argument) * len);
    if(root->args == NULL)
        return SARG_ERR_OTHER;

    memcpy(root->args, arguments, sizeof(sarg_argument) * len);
    root->arg_len = len;

    // init parsing result array
    root->results = malloc(sizeof(sarg_result) * len);
    if(root->args == NULL) {
        free(root->args);
        return SARG_ERR_OTHER;
    }
    root->res_len = len;

    // init results
    for (i = 0; i < root->res_len; ++i)
        _sarg_result_init(&root->results[i], root->args[i].type);

    return SARG_ERR_SUCCESS;
}

int _sarg_find_arg(sarg_root *root, char *name)
{
    int i;
    while(name[0] == '-')
        ++name;

    for(i = 0; i < root->arg_len; ++i)
    {
        int is_short = root.args[i].short_name && strcmp(root.args[i].short_name) == 0;
        int is_long = root.args[i].long_name && strcmp(root.args[i].long_name) == 0;
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

int sarg_parse(sarg_root *root, const char **argv, const int argc)
{
    int i, arg_idx, len, base;
    char *endptr;
    char short_arg[2];

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
            return SARG_ERR_INVALARG;

        if(_SARG_IS_SHORT_ARG(argv[i]) || _SARG_IS_LONG_ARG(argv[i]))
        {
            // find argument
            arg_idx = _sarg_find_arg(root, argv[i]);
            if(arg_idx < 0)
                return SARG_ERR_INVALARG;

            switch(root->args[arg_idx].type) {
            case INT:
                base = _sarg_get_number_base(argv[i+1]);

                // convert and check if conversion succeeded
                root->results[arg_idx].int_val = strtol(argv[i+1], &endptr, base);
                if(*endptr != '\0')
                    return SARG_ERR_INVALARG;

                break;
            case UINT:
                base = _sarg_get_number_base(argv[i+1]);

                // convert and check if conversion succeeded
                root->results[arg_idx].uint_val = strtoul(argv[i+1], &endptr, base);
                if(*endptr != '\0')
                    return SARG_ERR_INVALARG;

                break;
            case DOUBLE:
                root->results[arg_idx].double_val = strtod(argv[i+1], &endptr);
                if(*endptr != '\0')
                    return SARG_ERR_INVALARG;

                break;
            case BOOL:
                ++root->results[arg_idx].bool_val;

                break;
            case STRING:
                root->results[arg_idx].str_val = malloc(strlen(argv[i+1]) + 1);
                if(!root->results[arg_idx].str_val)
                    return SARG_ERR_OTHER;
                strcpy(root->results[arg_idx].str_val, argv[i+1]);

                break;
            default:
                return SARG_ERR_INVALARG;
            }
        }
    }

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
