.. ========================================================================
   (C) Copyright 2015,2017 Douglas B. Cuthbertson

.. This library is free software; you can redistribute it and/or modify
   it under the terms of the MIT license. See LICENSE for details.
   ========================================================================

#########################
A Basic Unit Test Library
#########################

.. toctree::
    :maxdepth: 2
    :caption: Table of Contents

    doc/planning cycle 0

*******************************************
BUT - A Basic Unit Test Implementation in C
*******************************************

The BUT library is merely a stepping stone to a more sophisticated unit test library. It provides the simplest test framework I thought I could get away with. All you need to do is create a shared library that exports the function :code:`test_suite_load`, which returns a test suite, an instance of :code:`but_test_suite`. The test suite will contain an array of one or more test cases, :code:`but_test_case` objects. Each test case will have a test function, :code:`but_method_run` that accepts a pointer to some data, tests some condition, and returns a success or failure value, :code:`BUT_SUCCESS` or :code:`BUT_FAIL`. Each test case may optionally have a pair of methods for setting up test data, :code:`but_method_setup`, and for releasing the resources allocated by the setup method, :code:`but_method_teardown`.
