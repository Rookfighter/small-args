/*
 * test_smallargs.c
 *
 *  Created on: 8 Oct 2016
 *      Author: fabian
 */

#include <smallargs.h>
#include <ctest.h>

CTEST(root, init_root)
{
    sarg_argument args[] = {
            {"n", "count", "some count variable", INT},
            {"f", "file", "out file", STRING},
            {"q", "quiet", "enable quiet mode", BOOL},
    };
    sarg_root root;

    root.args = NULL;
    root.arg_len = -1;
    root.results = NULL;
    root.res_len = -1;

    int ret = sarg_init(&root, args, 3);
    ASSERT_EQUAL(SARG_ERR_SUCCESS, ret);

    ASSERT_NOT_NULL(root.args);
    ASSERT_NOT_NULL(root.results);
    ASSERT_EQUAL(3, root.arg_len);
    ASSERT_EQUAL(3, root.res_len);

    sarg_destroy(&root);
}
