.. ========================================================================
   (C) Copyright 2015,2017 Douglas B. Cuthbertson

.. This library is free software; you can redistribute it and/or modify
   it under the terms of the MIT license. See LICENSE for details.
   ========================================================================

###################
Planning Cycle Zero
###################

There are several things I'd like to accomplish. Here is where I'm going to sort out which goals and features to purue next.

I think BUT is good enough to use on its own for now, so I don't plan on updating it beyond its current version, v0.1.0. I will port it to Linux, FreeBSD, and Mac OS X so I can use it to test the other components I need for the heirarchical test library that will become UTK.

************
Major Tasks
************

* Port BUT to Linux
* Port BUT to FreeBSD
* Port BUT to Mac OS X
* Create the following modules

  * Exception Handling. This is needed for the memory management and test module.
  * Mutex. Needed the Memory Region module, and probably for the Threads module.
  * Thread Local Storage. Needed for the Threads module.
  * Threads. Handle multiple threads.
  * Memory. Track memory usage through :code:`malloc`, :code:`calloc`, :code:`free` and other standard C memory allocation functions.
  * Memory Region module. Needed for the Memory module.
  * Design an API for UTK

**********
API Design
**********

I like the style of the `test and assertion macros <https://github.com/google/googletest/blob/master/googletest/docs/Primer.md>`_ in the `Google st framework <https://github.com/google/googletest>`_. While Google Test is a C++ test amework, I bet I can write a similar interface in C.

:code:`ASSERT` macros are fatal while :code:`EXPECT` are not. That implies the :code:`ASSERT` macros might have to rely on an exception handling mechanism to "throw" a fatal error. That thrown error would then be caught and recorded by the test driver.

These record the :code:`condition`. In case of a failure, the :code:`condition` is reported.

* :code:`ASSERT_TRUE(condition);`. :code:`EXPECT_TRUE(condition);` verifies :code:`condition` is true.
* :code:`ASSERT_FALSE(condition);`. :code:`EXPECT_FALSE(condition);` verifies :code:`condition` is false.

These macros will record both :code:`val1` and :code:`val2`. In the event of a failure, both are  displayed.

* :code:`ASSERT_EQ(val1, val2);`, :code:`EXPECT_EQ(val1, val2);` verifies :code:`val1 == val2`
* :code:`ASSERT_NE(val1, val2);`, :code:`EXPECT_NE(val1, val2);` verifies :code:`val1 != val2`
* :code:`ASSERT_LT(val1, val2);`, :code:`EXPECT_LT(val1, val2);` verifies :code:`val1 < val2`
* :code:`ASSERT_LE(val1, val2);`, :code:`EXPECT_LE(val1, val2);` verifies :code:`val1 <= val2`
* :code:`ASSERT_GT(val1, val2);`, :code:`EXPECT_GT(val1, val2);` verifies :code:`val1 > val2`
* :code:`ASSERT_GE(val1, val2);`, :code:`EXPECT_GE(val1, val2);` verifies :code:`val1 >= val2`

C-string comparisons. I'm using different names than Google Test for the case-insensitive macros. Using ``ASSERT__STRCASEEQ``, for example, feet like the "case" of the strings matter - which is the opposite of the intended meaning. Instead I'm using ``ASSERT_STRIEQ`` and ``ASSERT_STRINE`` to stand for "comparisons that are *Insensitive* to the case of the strings". The ``EXPECT_*`` macros follow the same pattern:

* :code:`ASSERT_STREQ(str1, str2);`,  :code:`EXPECT_STREQ(str1, str2);` verifies the two strings have the same content.
* :code:`ASSERT_STRNE(str1, str2);`,  :code:`EXPECT_STRNE(str1, str2);` verifies the two strings have different content.
* :code:`ASSERT_STRIEQ(str1, str2);`, :code:`EXPECT_STRIEQ(str1, str2);` verifies the two strings have the same content, insensitive to case.
* :code:`ASSERT_STRINE(str1, str2);`, :code:`EXPECT_STRINE(str1, str2);` verifies the two strings have different content, insensitive to case.

Google Test also has a macro that is used to name a test suite, name a test case, and define the body of the test: :code:`TEST(test_suite_name, test_case_name)`. Just use a code block following the macro to define the test:

.. code-block:: c

    // Test the factorial of 0
    TEST(FactorialTest, HandlesZeroInput) {
      EXPECT_EQ(1, factorial(0));
    }

    // Test the factorial of a few positive numbers
    TEST(FactorialTest, HandlesPositiveInput) {
        EXPECT_EQ(1, Factorial(1));
        EXPECT_EQ(2, Factorial(2));
        EXPECT_EQ(6, Factorial(3));
        EXPECT_EQ(40320, Factorial(8));
    }

Both the tests above are part of the :code:`FactorialTest` test suite.

Google Test also has a macro to name a test fixture and a test suite (same name?), and the name of the test case: :code:`TEST_F(test_fixture_name, test_case_name)`. As for :code:`TEST()`, the body of the test is defined in a code block following the macro.

There are a few rules when using :code:`TEST_F()`:

* The test fixture is defined by deriving a class from :code:`testing::Test`. It's body must start with :code:`protected` or :code:`public`.

  * If objects are needed for tests, then define a default constructor/destructor, or the functions :code:`SetUp` and :code`TearDown`.
  * Define any functions that your tests need to share.

* The test fixture must be defined before using the macro.
* The first argument for :code:`TEST_F()` is the name of the test fixture.

Queue Example
=============

Here's the queue example from the Google Test primer. This is the :code:`Queue` interface:

.. code-block:: c++

    template <typename E> // E is the element type.
    class Queue {
    public:
      Queue();
      void Enqueue(const E& element);
      E* Dequeue(); // Returns NULL if the queue is empty.
      size_t size() const;
      //...
    };

Here's the test fixture:

.. code-block:: c++

    class QueueTest : public ::testing::Test {
    protected:
      virtual void SetUp() {
        q1_.Enqueue(1);
        q2_.Enqueue(2);
        q2_.Enqueue(3);
      }

      // virtual void TearDown() {}

      Queue<int> q0_;
      Queue<int> q1_;
      Queue<int> q2_;
    };

Here are a couple of tests using the fixture above:

.. code-block:: c++

    TEST_F(QueueTest, IsEmptyInitially) {
      EXPECT_EQ(0, q0_.size());
    }

    TEST_F(QueueTest, DequeueWorks) {
      int* n = q0_.Dequeue();
      EXPECT_EQ(NULL, n);

      n = q1_.Dequeue();
      ASSERT_TRUE(n != NULL);
      EXPECT_EQ(1, *n);
      EXPECT_EQ(0, q1_.size());
      delete n;

      n = q2_.Dequeue();
      ASSERT_TRUE(n != NULL);
      EXPECT_EQ(2, *n);
      EXPECT_EQ(1, q2_.size());
      delete n;
    }

Known Limitations
=================

These limitations are instructive:

    Google Test is designed to be thread-safe. The implementation is thread-safe on systems where the ``pthreads`` library is available. It is currently unsafe to use Google Test assertions from two threads concurrently on other systems (e.g. Windows). In most tests this is not an issue as usually the assertions are done in the main thread. If you want to help, you can volunteer to implement the necessary synchronization primitives in ``gtest-port.h`` for your platform.
