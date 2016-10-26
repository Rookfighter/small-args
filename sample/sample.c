/*
 * sample.c
 *
 *  Created on: 14 Oct 2016
 *      Author: Fabian Meyer
 */

#include <smallargs.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

static sarg_opt my_args[5] = {
        {"h", "help", "show help text", BOOL, NULL},
        {"v", "verbose", "increase verbosity", BOOL, NULL},
        {"c", "count", "count up to this number", INT, NULL},
        {NULL, "root", "calculate square root of this number", DOUBLE, NULL},
        {NULL, "say", "print the given text", STRING, NULL}
};

int main(int argc, const char **argv)
{
    sarg_root root;
    sarg_result *res;
    int i;

    int ret = sarg_init(&root, my_args, 5, "sarg_sample_c");
    assert(ret == SARG_ERR_SUCCESS);

    ret = sarg_parse(&root, argv, argc);
    if(ret != SARG_ERR_SUCCESS)
    {
        printf("Parsing failed\n");
        //sarg_print_help(&root);
        return -1;
    }


    // check if help flag was set
    ret = sarg_get(&root, "help", &res);
    // assert actually never fails,
    // if the option was initialized in sarg_init
    assert(ret == SARG_ERR_SUCCESS);

    if(res->bool_val)
    {
       sarg_help_print(&root);
       return -1;
    }

    // check for verbosity
    ret = sarg_get(&root, "v", &res);
    assert(ret == SARG_ERR_SUCCESS);

    printf("verbosity set to %d\n", res->count);

    // count to the given number
    ret = sarg_get(&root, "count", &res);
    assert(ret == SARG_ERR_SUCCESS);
    if(res->count > 0)
    {
        printf("counting: ");
        for(i = 0; i < res->int_val; ++i)
        {
            printf("%d", i+1);
            if(i+1 != res->int_val)
                printf(",");
        }
        printf("\n");
    }

    // calculate square root of given number
    ret = sarg_get(&root, "root", &res);
    assert(ret == SARG_ERR_SUCCESS);
    if(res->count > 0)
    {
        printf("%f\n", sqrt(res->double_val));
    }

    // calculate square root of given number
    ret = sarg_get(&root, "say", &res);
    assert(ret == SARG_ERR_SUCCESS);
    if(res->count > 0)
    {
        printf("you say: %s\n", res->str_val);
    }



    return 0;
}
