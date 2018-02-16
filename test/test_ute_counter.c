/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#include <platform.h>
#include <but.h>
#include <ute_counter.h>
#include <ute_driver.h>
#include <string.h>


struct ute_counter_data
{
    ute_context    *ctx;
    ute_counter     ctr;
};
typedef struct ute_counter_data ute_counter_data;


// Test setup and teardown methods
INTERNAL_FUNCTION utk_result setup_counter(void *data);
INTERNAL_FUNCTION void teardown_counter(void *data);


// The names of our test cases
#define TEST_NAME_INIT  "Initialize Counter"
#define TEST_NAME_GET_CONTEXT "Get Context"
#define TEST_NAME_INCREMENT_COUNT_THROW "Increment Count Throw"
#define TEST_NAME_THROW_TRY "Throw Try"
#define TEST_NAME_THROW_DISABLE_1 "Throw Disable 1"
#define TEST_NAME_THROW_DISABLE_2 "Throw Disable 2"
#define TEST_NAME_THROW_ENABLE_1 "Throw Enable 1"
#define TEST_NAME_THROW_ENABLE_2 "Throw Enable 2"
#define TEST_NAME_THROW_DISABLED_INITIALLY "Throw Initially Disabled"
#define TEST_NAME_THROW_IS_ENABLED "Throw Is Enabled"
#define TEST_NAME_THROW_ENABLED_AFTER_THROW "Throw Enabled After Throw"


// The name of the test suite and a forward reference to it.
#define UTE_TS_NAME "UTE Counter"
GLOBAL_VARIABLE utk_test_suite ute_ts;
GLOBAL_VARIABLE ute_counter_data test_data;


// Test methods
INTERNAL_FUNCTION utk_result test_initialization(void *data);
INTERNAL_FUNCTION utk_result test_get_context(void *data);
INTERNAL_FUNCTION utk_result test_increment_count_throw(void *data);
INTERNAL_FUNCTION utk_result test_throw_try(void *data);
INTERNAL_FUNCTION utk_result test_throw_disable_1(void *data);
INTERNAL_FUNCTION utk_result test_throw_disable_2(void *data);
INTERNAL_FUNCTION utk_result test_throw_enable_1(void *data);
INTERNAL_FUNCTION utk_result test_throw_enable_2(void *data);
INTERNAL_FUNCTION utk_result test_throw_is_enabled(void *data);
INTERNAL_FUNCTION utk_result test_throw_disabled_initially(void *data);
INTERNAL_FUNCTION utk_result test_throw_enabled_after_throw(void *data);


/** @brief test cases for the UTE Counter implementation */

GLOBAL_VARIABLE utk_test_case test_case_counter_init =
{
    TEST_NAME_INIT,
    setup_counter,
    test_initialization,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_counter_get_context =
{
    TEST_NAME_GET_CONTEXT,
    setup_counter,
    test_get_context,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_counter_increment_count_throw = 
{
    TEST_NAME_INCREMENT_COUNT_THROW,
    setup_counter,
    test_increment_count_throw,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_try =
{
    TEST_NAME_THROW_TRY,
    setup_counter,
    test_throw_try,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_disable_1 = 
{
    TEST_NAME_THROW_DISABLE_1,
    setup_counter,
    test_throw_disable_1,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_disable_2 = 
{
    TEST_NAME_THROW_DISABLE_2,
    setup_counter,
    test_throw_disable_2,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_enable_1 = 
{
    TEST_NAME_THROW_ENABLE_1,
    setup_counter,
    test_throw_enable_1,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_enable_2 = 
{
    TEST_NAME_THROW_ENABLE_2,
    setup_counter,
    test_throw_enable_2,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_disabled_initially = 
{
    TEST_NAME_THROW_DISABLED_INITIALLY,
    setup_counter,
    test_throw_disabled_initially,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_is_enabled = 
{
    TEST_NAME_THROW_IS_ENABLED,
    setup_counter,
    test_throw_is_enabled,
    teardown_counter,
    &test_data
};

GLOBAL_VARIABLE utk_test_case test_case_throw_enabled_after_throw = 
{
    TEST_NAME_THROW_ENABLED_AFTER_THROW,
    setup_counter,
    test_throw_enabled_after_throw,
    teardown_counter,
    &test_data
};


enum test_results {
    CTR_SUCCESS = UTK_SUCCESS,
    CTR_INVALID_EXCEPTION_POINT_INITIAL,
    CTR_INVALID_COUNT_THROW_INITIAL,
    CTR_INVALID_THROW_TEST_EXCEPTION_INITIAL,
    CTR_INVALID_CONTEXT,
    CTR_INVALID_COUNT_THROW,
    CTR_FAILED_TO_THROW,
    CTR_FAILED_TO_THROW2,
    CTR_INVALID_THROW,
    CTR_FAILED_THROW_ENABLE,
    CTR_FAILED
};


utk_result
setup_counter(void *data)
{
    utk_result result = CTR_SUCCESS;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    ctr_data->ctx = ute_new(&ute_ts);

    if (NULL == ctr_data->ctx) {
        result = UTK_FAIL_SETUP;
    }

    ute_counter_init(&ctr_data->ctr, ctr_data->ctx);

    return result;
}

void
teardown_counter(void *data)
{
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    ute_context *ctx = ute_counter_get_context(&ctr_data->ctr);
    ute_delete(ctx);
}


/**
 * @brief Initialize a counter and verify the field values are correct.
 */
utk_result
test_initialization(void *data)
{
    utk_result result = CTR_SUCCESS;
    ute_counter_data *ctr_data = (ute_counter_data*)data;

    if (ctr_data->ctr.count_exception_point != 0) {
        result = CTR_INVALID_EXCEPTION_POINT_INITIAL;
    } else if (ctr_data->ctr.count_fail != 0) {
        result = CTR_INVALID_COUNT_THROW_INITIAL;
    } else if (ctr_data->ctr.throw_test_exception) {
        result = CTR_INVALID_THROW_TEST_EXCEPTION_INITIAL;
    } else if (ctr_data->ctr.context != ctr_data->ctx) {
        result = CTR_INVALID_CONTEXT;
    }

    return result;
}


utk_result
test_get_context(void *data)
{
    utk_result result = CTR_SUCCESS;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    ute_context *ctx = ute_counter_get_context(&ctr_data->ctr);

    if (ctr_data->ctx != ctx) {
        result = CTR_INVALID_CONTEXT;
    }

    return result;
}


utk_result
test_increment_count_throw(void *data)
{
    utk_result result = CTR_SUCCESS;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    u64 expected = 0;
    u64 actual = ute_get_count_fail(&ctr_data->ctr);

    if (expected != actual) {
        result = CTR_INVALID_COUNT_THROW_INITIAL;
    } else {
        ute_increment_count_fail(&ctr_data->ctr);
        ++expected;
        actual = ute_get_count_fail(&ctr_data->ctr);
        if (expected != actual) {
            result = CTR_INVALID_COUNT_THROW;
        }
    }

    return result;
}


utk_result
test_throw_try(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;

    ute_increment_count_fail(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_try(&ctr_data->ctr);
        result = CTR_FAILED_TO_THROW;
    } EHM_CATCH(exception_ute_test) {
        ute_increment_count_fail(&ctr_data->ctr);
        EHM_TRY {
            ute_throw_try(&ctr_data->ctr);
            EHM_TRY {
                ute_throw_try(&ctr_data->ctr);
                result = CTR_FAILED_TO_THROW2;
            } EHM_CATCH(exception_ute_test) {
                result = CTR_SUCCESS;
            } EHM_ENDTRY;
        } EHM_CATCH(exception_ute_test) {
            result = CTR_INVALID_THROW;
        } EHM_ENDTRY;
    } EHM_ENDTRY;

    return result;
}


utk_result
test_throw_disable_1(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;

    ute_increment_count_fail(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_disable(&ctr_data->ctr);
        ute_throw_try(&ctr_data->ctr);
        result = CTR_SUCCESS;
    } EHM_CATCH(exception_ute_test) {
        result = CTR_INVALID_THROW;
    } EHM_ENDTRY;

    return result;
}


utk_result
test_throw_disable_2(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;

    ute_increment_count_fail(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_try(&ctr_data->ctr);
        result = CTR_FAILED_TO_THROW;
    } EHM_CATCH(exception_ute_test) {
        ute_increment_count_fail(&ctr_data->ctr);
        EHM_TRY {
            ute_throw_try(&ctr_data->ctr);
            EHM_TRY {
                ute_throw_disable(&ctr_data->ctr);
                ute_throw_try(&ctr_data->ctr);
                result = CTR_SUCCESS;
            } EHM_CATCH(exception_ute_test) {
                result = CTR_INVALID_THROW;
            } EHM_ENDTRY;
        } EHM_CATCH(exception_ute_test) {
            result = CTR_INVALID_THROW;
        } EHM_ENDTRY;
    } EHM_ENDTRY;

    return result;
}


utk_result
test_throw_enable_1(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;

    ute_increment_count_fail(&ctr_data->ctr);
    ute_throw_disable(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_enable(&ctr_data->ctr);
        ute_throw_try(&ctr_data->ctr);
        result = CTR_FAILED_TO_THROW;
    } EHM_CATCH(exception_ute_test) {
        result = CTR_SUCCESS;
    } EHM_ENDTRY;

    return result;
}


utk_result
test_throw_enable_2(void *data)
{
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    utk_result result = CTR_FAILED;

    ute_increment_count_fail(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_disable(&ctr_data->ctr);
        ute_throw_try(&ctr_data->ctr);
    } EHM_CATCH(exception_ute_test) {
        result = CTR_INVALID_THROW;
    } EHM_FINALLY {
        ute_throw_enable(&ctr_data->ctr);
        EHM_TRY {
            ute_throw_try(&ctr_data->ctr);
            result = CTR_FAILED_TO_THROW;
        } EHM_CATCH(exception_ute_test) {
            result = CTR_SUCCESS;
        } EHM_ENDTRY;
    } EHM_ENDTRY;

    return result;
}


utk_result
test_throw_disabled_initially(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    
    if (!ute_throw_is_enabled(&ctr_data->ctr)) {
        result = CTR_SUCCESS;
    }

    return result;
}


utk_result
test_throw_is_enabled(void *data)
{
    utk_result result = CTR_FAILED;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    
    ute_increment_count_fail(&ctr_data->ctr);
    if (ute_throw_is_enabled(&ctr_data->ctr)) {
        result = CTR_SUCCESS;
    }

    return result;
}


utk_result
test_throw_enabled_after_throw(void *data)
{
    utk_result result = CTR_FAILED_TO_THROW;
    ute_counter_data *ctr_data = (ute_counter_data*)data;
    
    ute_increment_count_fail(&ctr_data->ctr);
    EHM_TRY {
        ute_throw_try(&ctr_data->ctr);
        result = CTR_FAILED_TO_THROW;
    } EHM_CATCH(exception_ute_test) {
        if (ute_throw_is_enabled(&ctr_data->ctr)) {
            result = CTR_SUCCESS;
        }
    } EHM_ENDTRY;

    return result;
}


// The array of test cases for the test suite
GLOBAL_VARIABLE
utk_test_case *tca[] = 
{
    // UTE driver tests
    &test_case_counter_init,
    &test_case_counter_get_context,
    &test_case_counter_increment_count_throw,
    &test_case_throw_try,
    &test_case_throw_disable_1,
    &test_case_throw_disable_2,
    &test_case_throw_enable_1,
    &test_case_throw_enable_2,
    &test_case_throw_disabled_initially,
    &test_case_throw_is_enabled,
    &test_case_throw_enabled_after_throw
};

GLOBAL_VARIABLE
utk_test_suite ute_ts = {UTE_TS_NAME, 
                         ARRAY_COUNT(tca),
                         tca};

PROJECTAPI utk_test_suite *
test_suite_load(void)
{
    return &ute_ts;
}
