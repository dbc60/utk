/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include <dlist.h>
#include <memory.h> // declare memset
#include <stdlib.h> // define NULL

// The name of the exported test suite
#define TS_NAME_DLIST "DLIST"

// The names of our test cases
#define TC_NAME_INIT "Init"
#define TC_NAME_INSERT_HEAD_ONE "Insert Head One"
#define TC_NAME_INSERT_TAIL_ONE "Insert Tail One"
#define TC_NAME_INSERT_HEAD_TWO "Insert Head Two"
#define TC_NAME_INSERT_TAIL_TWO "Insert Tail Two"
#define TC_NAME_REMOVE_NODE "Remove Node"
#define TC_NAME_REMOVE_LONG "Remove Long"

// Test setup and teardown methods
INTERNAL_FUNCTION utk_result setup_dlist(void *data);
INTERNAL_FUNCTION utk_result setup_dlist_nodes(void *data);
INTERNAL_FUNCTION void    teardown_dlist(void *data);

// Test methods
INTERNAL_FUNCTION utk_result test_dlist_init(void *data);
INTERNAL_FUNCTION utk_result test_dlist_insert_head_one(void *data);
INTERNAL_FUNCTION utk_result test_dlist_insert_tail_one(void *data);
INTERNAL_FUNCTION utk_result test_dlist_insert_head_two(void *data);
INTERNAL_FUNCTION utk_result test_dlist_insert_tail_two(void *data);
INTERNAL_FUNCTION utk_result test_dlist_remove_node(void *data);
INTERNAL_FUNCTION utk_result test_dlist_remove_long(void *data);

// Test Data
LOCAL_VARIABLE dlist dl1, dl2, dl3, dl4;

LOCAL_VARIABLE dlist *init_data[] = {&dl1, NULL};
LOCAL_VARIABLE dlist *two_nodes[] = {&dl1, &dl2, NULL};
LOCAL_VARIABLE dlist *three_nodes[] = {&dl1, &dl2, &dl3, NULL};
LOCAL_VARIABLE dlist *four_nodes[] = {&dl1, &dl2, &dl3, &dl4, NULL};

LOCAL_VARIABLE utk_test_case test_case_init = 
{
    TC_NAME_INIT,
    setup_dlist, 
    test_dlist_init, 
    teardown_dlist, 
    init_data
};

LOCAL_VARIABLE utk_test_case test_case_insert_head_one =
{
    TC_NAME_INSERT_HEAD_ONE,
    setup_dlist,
    test_dlist_insert_head_one,
    teardown_dlist,
    two_nodes
};

LOCAL_VARIABLE utk_test_case test_case_insert_tail_one = 
{
    TC_NAME_INSERT_TAIL_ONE,
    setup_dlist,
    test_dlist_insert_tail_one,
    teardown_dlist,
    two_nodes
};

LOCAL_VARIABLE utk_test_case test_case_insert_head_two = 
{
    TC_NAME_INSERT_HEAD_TWO,
    setup_dlist,
    test_dlist_insert_head_two,
    teardown_dlist,
    three_nodes
};

LOCAL_VARIABLE utk_test_case test_case_insert_tail_two =
{
    TC_NAME_INSERT_TAIL_TWO,
    setup_dlist,
    test_dlist_insert_tail_two,
    teardown_dlist,
    three_nodes
};

LOCAL_VARIABLE utk_test_case test_case_remove_node = 
{
    TC_NAME_REMOVE_NODE,
    setup_dlist,
    test_dlist_remove_node,
    teardown_dlist,
    two_nodes
};

LOCAL_VARIABLE utk_test_case test_case_remove_long = 
{
    TC_NAME_REMOVE_LONG,
    setup_dlist_nodes,
    test_dlist_remove_long,
    teardown_dlist,
    four_nodes
};


utk_result
setup_dlist(void *data)
{
    dlist **dl = (dlist **)data;

    while (*dl)
    {
        dlist_init(*dl);
        ++dl;
    }

    return 0;
}


utk_result
setup_dlist_nodes(void *data)
{
    dlist    *list = ((dlist**)data)[0];
    dlist    *first = ((dlist**)data)[1];
    dlist    *second = ((dlist**)data)[2];
    dlist    *third = ((dlist**)data)[3];

    setup_dlist(data);
    dlist_insert_tail(list, first);
    dlist_insert_tail(list, second);
    dlist_insert_tail(list, third);

    return 0;
}


void
teardown_dlist(void *data)
{
    dlist **dl = (dlist **)data;

    while (*dl)
    {
        memset(*dl, 0, sizeof(dlist));
        ++dl;
    }
}


utk_result
test_dlist_init(void *data)
{
    dlist **dl = (dlist**)data;
    int result;

    result = (*dl != dlist_previous(*dl) || *dl != dlist_next(*dl));

    return result;
}


/**
 * @brief unit tests
 */

utk_result
test_dlist_insert_head_one(void *data)
{
    dlist *list = ((dlist**)data)[0];
    dlist *first = ((dlist**)data)[1];
    int result;

    dlist_insert_head(list, first);
    result = (dlist_next(list) != first) | (dlist_previous(list) != first);
    result |= (dlist_next(first) != list) | (dlist_previous(first) != list);
    result |= dlist_is_empty(list);
    result |= dlist_is_empty(first);

    return result;
}


utk_result
test_dlist_insert_tail_one(void *data)
{
    dlist *list = ((dlist**)data)[0];
    dlist *first = ((dlist**)data)[1];
    int result;

    dlist_insert_tail(list, first);
    result = (dlist_next(list) != first) | (dlist_previous(list) != first);
    result |= (dlist_next(first) != list) | (dlist_previous(first) != list);
    result |= dlist_is_empty(list);
    result |= dlist_is_empty(first);

    return result;
}


utk_result
test_dlist_insert_head_two(void *data)
{
    dlist    *list = ((dlist**)data)[0];
    dlist    *first = ((dlist**)data)[1];
    dlist    *second = ((dlist**)data)[2];
    int         result;

    dlist_insert_head(list, first);
    dlist_insert_head(list, second);
    result = (dlist_next(list) != second) | (dlist_previous(list) != first);
    result |= (dlist_next(first) != list) | (dlist_previous(first) != second);
    result |= (dlist_next(second) != first) | (dlist_previous(second) != list);

    return result;
}


utk_result
test_dlist_insert_tail_two(void *data)
{
    dlist    *list = ((dlist**)data)[0];
    dlist    *first = ((dlist**)data)[1];
    dlist    *second = ((dlist**)data)[2];
    int         result;

    dlist_insert_tail(list, first);
    dlist_insert_tail(list, second);
    result = (dlist_next(list) != first) | (dlist_previous(list) != second);
    result |= (dlist_next(first) != second) | (dlist_previous(first) != list);
    result |= (dlist_next(second) != list) | (dlist_previous(second) != first);

    return result;
}


utk_result
test_dlist_remove_node(void *data)
{
    dlist    *list = ((dlist**)data)[0];
    dlist    *first = ((dlist**)data)[1];
    int         result;

    dlist_remove(first);
    result = !dlist_is_empty(list);
    result |= !dlist_is_empty(first);

    return result;
}


utk_result
test_dlist_remove_long(void *data)
{
    dlist    *list = ((dlist**)data)[0];
    dlist    *first = ((dlist**)data)[1];
    dlist    *second = ((dlist**)data)[2];
    dlist    *third = ((dlist**)data)[3];
    int         result;

    dlist_remove(second);
    result = !dlist_is_empty(second);
    result |= (dlist_next(list) != first) | (dlist_previous(list) != third);
    result |= (dlist_next(first) != third) | (dlist_previous(first) != list);
    result |= (dlist_next(third) != list) | (dlist_previous(third) != first);

    return result;
}


LOCAL_VARIABLE utk_test_case *tca[] = 
{
    &test_case_init,
    &test_case_insert_head_one,
    &test_case_insert_tail_one,
    &test_case_insert_head_two,
    &test_case_insert_tail_two,
    &test_case_remove_node,
    &test_case_remove_long
};

LOCAL_VARIABLE utk_test_suite ts_dlist =
{
    TS_NAME_DLIST,
    ARRAY_COUNT(tca),
    tca
};

PROJECTAPI utk_test_suite *
test_suite_load(void) {
    return &ts_dlist;
}
