.. ========================================================================
   (C) Copyright 2015,2017 Douglas B. Cuthbertson

.. This library is free software; you can redistribute it and/or modify
   it under the terms of the MIT license. See LICENSE for details.
   ========================================================================

###########################################
BUT - A Basic Unit Test Implementation in C
###########################################

The BUT library is merely a stepping stone to a more sophisticated unit test library. It provides the simplest test framework I thought I could get away with. All you need to do to make a test suite is to create a shared library that exports the function :code:`test_suite_load`, which returns its test suite, an instance of :code:`but_test_suite`. The test suite will contain an array of one or more test cases, :code:`but_test_case` objects. Each test case will have a test function, whose type is :code:`but_method_run`, that accepts a pointer to some data, tests some condition, and returns a success or failure value, :code:`BUT_SUCCESS` or :code:`BUT_FAIL`. Each test case may optionally have a pair of methods for setting up test data, :code:`but_method_setup`, and releasing the resources allocated by the setup method, :code:`but_method_teardown`.

BUT includes an API for the driver that can be built into a static or shared library, and linked into an executable. It is currently built into a static library and linked into both a Windows and Linux driver: ``win32_but_driver.exe`` and ``linux_but_driver``, respectively. See ``include/but_driver.h`` for the exported functions, and either ``src/win32_but_driver.c`` or ``src/linux_but_driver.c`` for examples of how to use it.

**********
References
**********

* `Adding Exception Testing to Unit Tests <http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2001/0104/stanley/stanley.htm>`_, by Ben Stanley
* `Exception Safety in Generic Components <http://www.boost.org/community/exception_safety.html>`_, by David Abrahams
