/*
 * sample_cb.c
 *
 *  Created on: 02 Dec 2016
 *      Author: Fabian Meyer
 */

#include <smallargs.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define UNUSED(x) ((void)x)

static sarg_root root;

static int help_cb(const sarg_result *res)
{
    UNUSED(res);
    sarg_help_print(&root);
    exit(0);
}

static int verbose_cb(const sarg_result *res)
{
    printf("verbosity set to %d\n", res->count);
    return SARG_ERR_SUCCESS;
}

static int count_cb(const sarg_result *res)
{
    int i;
    printf("counting: ");
    for(i = 0; i < res->int_val; ++i) {
        printf("%d", i + 1);
        if(i + 1 != res->int_val)
            printf(",");
    }
    printf("\n");
    return SARG_ERR_SUCCESS;
}

static int root_cb(const sarg_result *res)
{
    printf("%f\n", sqrt(res->double_val));
    return SARG_ERR_SUCCESS;
}

static int say_cb(const sarg_result *res)
{
    printf("you say: %s\n", res->str_val);
    return SARG_ERR_SUCCESS;
}

static sarg_opt my_args[] = {
    {"h", "help", "show help text", BOOL, help_cb},
    {"v", "verbose", "increase verbosity", BOOL, verbose_cb},
    {"c", "count", "count up to this number", INT, count_cb},
    {NULL, "root", "calculate square root of this number", DOUBLE, root_cb},
    {NULL, "say", "print the given text", STRING, say_cb},
    {NULL, NULL, NULL, INT, NULL}
};

int main(int argc, const char **argv)
{
    int ret = sarg_init(&root, my_args, "sarg_sample_c");
    assert(ret == SARG_ERR_SUCCESS);

    ret = sarg_parse(&root, argv, argc);
    if(ret != SARG_ERR_SUCCESS) {
        printf("Error: Parsing failed\n");
        sarg_help_print(&root);
        sarg_destroy(&root);
        return -1;
    }

    sarg_destroy(&root);

    return 0;
}
