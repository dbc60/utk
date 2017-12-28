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

*******************************************
BUT - A Basic Unit Test Implementation in C
*******************************************

The BUT library is merely a stepping stone to a more sophisticated unit test library. It provides the simplest test framework I thought I could get away with. All you need to do is create a shared library that exports the function :code:`test_suite_load`, which returns a test suite, an instance of :code:`but_test_suite`. The test suite will contain an array of one or more test cases, :code:`but_test_case` objects. Each test case will have a test function, :code:`but_method_run` that accepts a pointer to some data, tests some condition, and returns a success or failure value, :code:`BUT_SUCCESS` or :code:`BUT_FAIL`. Each test case may optionally have a pair of methods for setting up test data, :code:`but_method_setup`, and for releasing the resources allocated by the setup method, :code:`but_method_teardown`.

************
Building BUT
************

BUT has one main component, a shared library that implements a driver called ``but_driver.dll``. All that's needed to make a BUT driver executable is a main routine to handle loading/unloading tests, and displaying their results. There's a Windows implementation of such a driver in ``src/win32_but_driver.c``. It just links to the ``but_driver`` link-library.

There is also a test suite, ``test_driver`` in the ``but`` directory, that can be loaded by a BUT driver executable to test ``but_driver.dll``.
