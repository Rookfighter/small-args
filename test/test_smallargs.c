/*
 * test_smallargs.c
 *
 *  Created on: 8 Oct 2016
 *      Author: fabian
 */

#include <limits.h>
#include <smallargs.h>
#include <ctest.h>
#include <stdio.h>

#define UNUSED(e) ((void) e)

CTEST(misc, root_init)
{
    sarg_opt args[] = {
        {"n", "count", "some count variable", INT, NULL},
        {"f", "file", "out file", STRING, NULL},
        {"q", "quiet", "enable quiet mode", BOOL, NULL},
        {NULL, NULL, NULL, INT, NULL}
    };
    sarg_root root;

    root.opts = NULL;
    root.opt_len = -1;
    root.results = NULL;
    root.res_len = -1;

    int ret = sarg_init(&root, args, "test");
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);

    ASSERT_NOT_NULL(root.opts);
    ASSERT_EQUAL(3, root.opt_len);
    ASSERT_NOT_NULL(root.results);
    ASSERT_EQUAL(3, root.res_len);

    ASSERT_STR("n", root.opts[0].short_name);
    ASSERT_STR("count", root.opts[0].long_name);
    ASSERT_STR("some count variable", root.opts[0].help);
    ASSERT_EQUAL(INT, root.opts[0].type);

    ASSERT_STR("f", root.opts[1].short_name);
    ASSERT_STR("file", root.opts[1].long_name);
    ASSERT_STR("out file", root.opts[1].help);
    ASSERT_EQUAL(STRING, root.opts[1].type);

    ASSERT_STR("q", root.opts[2].short_name);
    ASSERT_STR("quiet", root.opts[2].long_name);
    ASSERT_STR("enable quiet mode", root.opts[2].help);
    ASSERT_EQUAL(BOOL, root.opts[2].type);

    sarg_destroy(&root);
}

CTEST(misc, root_destroy)
{
    sarg_opt args[] = {
        {"n", "count", "some count variable", INT, NULL},
        {"f", "file", "out file", STRING, NULL},
        {"q", "quiet", "enable quiet mode", BOOL, NULL},
        {NULL, NULL, NULL, INT, NULL}
    };
    sarg_root root;

    int ret = sarg_init(&root, args, "test");
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);

    sarg_destroy(&root);

    ASSERT_NULL(root.opts);
    ASSERT_NULL(root.results);
    ASSERT_EQUAL(-1, root.opt_len);
    ASSERT_EQUAL(-1, root.res_len);
}

CTEST(misc, result_init)
{
    sarg_result res;
    res.count = 2;
    res.type = STRING;
    res.int_val = -10;

    _sarg_result_init(&res, INT);
    ASSERT_EQUAL(INT, res.type);
    ASSERT_EQUAL(0, res.count);
    ASSERT_EQUAL(0, res.int_val);
}

CTEST(misc, result_destroy)
{
    sarg_result res;

    _sarg_result_init(&res, STRING);
    res.str_val = (char *) malloc(8);
    ASSERT_NOT_NULL(res.str_val);

    _sarg_result_destroy(&res);
    ASSERT_NULL(res.str_val);
}

CTEST(misc, get_number_base_dec)
{
    ASSERT_EQUAL(10, _sarg_get_number_base("1234"));
};

CTEST(misc, get_number_base_hex)
{
    ASSERT_EQUAL(16, _sarg_get_number_base("0xA8"));
};

CTEST(misc, get_number_base_oct)
{
    ASSERT_EQUAL(8, _sarg_get_number_base("015"));
};

CTEST(misc, get_invalid_number_base)
{
    ASSERT_EQUAL(10, _sarg_get_number_base("blabla"));
};

/* ==========================================================
 * Parsing Tests
 * ========================================================== */

sarg_opt test_args[] = {
    {"i", NULL, "foo bar", INT, NULL},
    {"n", "count", "some count variable", UINT, NULL},
    {NULL, "prob", "probability", DOUBLE, NULL},
    {"f", "file", "out file", STRING, NULL},
    {"q", "quiet", "enable quiet mode", BOOL, NULL},
    {NULL, NULL, NULL, INT, NULL}
};

CTEST_DATA(parsing)
{
    sarg_root root;
    sarg_result result;
};

CTEST_SETUP(parsing)
{
    ASSERT_EQUAL(SARG_ERR_SUCCESS, sarg_init(&data->root, test_args, "test"));
    _sarg_result_init(&data->result, INT);
}

/* ==========================================================
 * parse_INT
 * ========================================================== */

CTEST2(parsing, parse_int_dec)
{
    data->result.type = INT;

    int ret = _sarg_parse_int("1234", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(1234, data->result.int_val);

    ret = _sarg_parse_int("-1234", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(-1234, data->result.int_val);
}

CTEST2(parsing, parse_int_oct)
{
    data->result.type = INT;

    int ret = _sarg_parse_int("015", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(015, data->result.int_val);
}

CTEST2(parsing, parse_int_hex)
{
    data->result.type = INT;

    int ret = _sarg_parse_int("0xA8", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(0xA8, data->result.int_val);

    ret = _sarg_parse_int("0xb8", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(0xB8, data->result.int_val);
}

CTEST2(parsing, parse_int_fail)
{
    data->result.type = INT;

    int ret = _sarg_parse_int("bla", &data->result);

    ASSERT_EQUAL(SARG_ERR_PARSE, ret);
}

/* ==========================================================
 * parse_UINT
 * ========================================================== */

CTEST2(parsing, parse_uint_dec)
{
    data->result.type = UINT;

    int ret = _sarg_parse_uint("1234", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(1234, data->result.uint_val);

    ret = _sarg_parse_uint("-1", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(UINT_MAX, data->result.uint_val);
}

CTEST2(parsing, parse_uint_oct)
{
    data->result.type = UINT;

    int ret = _sarg_parse_uint("015", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(015, data->result.uint_val);
}

CTEST2(parsing, parse_uint_hex)
{
    data->result.type = UINT;

    int ret = _sarg_parse_uint("0xA8", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(0xA8, data->result.uint_val);

    ret = _sarg_parse_uint("0xb8", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(0xB8, data->result.uint_val);
}

CTEST2(parsing, parse_uint_fail)
{
    data->result.type = UINT;

    int ret = _sarg_parse_uint("bla", &data->result);

    ASSERT_EQUAL(SARG_ERR_PARSE, ret);
}

/* ==========================================================
 * parse_DOUBLE
 * ========================================================== */

CTEST2(parsing, parse_double_success)
{
    data->result.type = DOUBLE;

    int ret = _sarg_parse_double("10.12", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_DBL_NEAR_TOL(10.12, data->result.double_val, 0.001);

    ret = _sarg_parse_double("-5.34", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_DBL_NEAR_TOL(-5.34, data->result.double_val, 0.001);

    ret = _sarg_parse_double("10", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_DBL_NEAR_TOL(10, data->result.double_val, 0.1);
}

CTEST2(parsing, parse_double_fail)
{
    data->result.type = DOUBLE;

    int ret = _sarg_parse_double("bla", &data->result);

    ASSERT_EQUAL(SARG_ERR_PARSE, ret);
}

/* ==========================================================
 * parse_BOOL
 * ========================================================== */

CTEST2(parsing, parse_bool_success)
{
    data->result.type = BOOL;

    int ret = _sarg_parse_bool(NULL, &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(1, data->result.bool_val);

    ret = _sarg_parse_bool(NULL, &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(2, data->result.bool_val);
}

/* ==========================================================
 * parse_STRING
 * ========================================================== */

CTEST2(parsing, parse_string_success)
{
    data->result.type = STRING;

    int ret = _sarg_parse_str("foobar", &data->result);

    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_STR("foobar", data->result.str_val);
}

/* ==========================================================
 * find_arg
 * ========================================================== */

CTEST2(parsing, find_arg_success)
{
    int ret = _sarg_find_opt(&data->root, "prob");
    ASSERT_EQUAL(2, ret);

    ret = _sarg_find_opt(&data->root, "i");
    ASSERT_EQUAL(0, ret);

    ret = _sarg_find_opt(&data->root, "file");
    ASSERT_EQUAL(3, ret);

    ret = _sarg_find_opt(&data->root, "f");
    ASSERT_EQUAL(3, ret);
}

CTEST2(parsing, find_arg_fail)
{
    int ret = _sarg_find_opt(&data->root, "foo");
    ASSERT_EQUAL(-1, ret);

    ret = _sarg_find_opt(&data->root, "a");
    ASSERT_EQUAL(-1, ret);
}

/* ==========================================================
 * get
 * ========================================================== */

CTEST2(parsing, get_success)
{
    sarg_result *res;

    int ret = sarg_get(&data->root, "prob", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(DOUBLE, res->type);

    ret = sarg_get(&data->root, "i", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(INT, res->type);

}

CTEST2(parsing, get_fail)
{
    sarg_result *res;

    int ret = sarg_get(&data->root, "foo", &res);
    ASSERT_EQUAL(SARG_ERR_NOTFOUND, ret);
}

/* ==========================================================
 * parse
 * ========================================================== */

CTEST2(parsing, parse_success)
{
    sarg_result *res;
    char *test_argv[8] = {"myapp", "--prob", "0.1", "-f", "myfile", "--count", "10", "-q"};

    int ret = sarg_parse(&data->root, (const char **) test_argv, 8);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);

    ret = sarg_get(&data->root, "prob", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_DBL_NEAR_TOL(0.1, res->double_val, 0.01);

    ret = sarg_get(&data->root, "file", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_STR("myfile", res->str_val);

    ret = sarg_get(&data->root, "count", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(10, res->int_val);

    ret = sarg_get(&data->root, "q", &res);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);
    ASSERT_EQUAL(1, res->bool_val);
}

CTEST2(parsing, parse_fail)
{
    char *test_argv1[7] = {"myapp", "--prob", "-f", "myfile", "--count", "10", "-q"};
    char *test_argv2[8] = {"myapp", "--prob", "0.1", "-t", "myfile", "--count", "10", "-q"};

    int ret = sarg_parse(&data->root, (const char **) test_argv1, 7);
    ASSERT_EQUAL(SARG_ERR_PARSE, ret);

    ret = sarg_parse(&data->root, (const char **) test_argv2, 8);
    ASSERT_EQUAL(SARG_ERR_NOTFOUND, ret);
}

CTEST_TEARDOWN(parsing)
{
    _sarg_result_destroy(&data->result);
    sarg_destroy(&data->root);
}
