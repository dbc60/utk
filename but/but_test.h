#ifndef BUT_TEST_H_INCLUDED
#define BUT_TEST_H_INCLUDED

#include <but.h>

// Test the include guards
#ifndef BUT_H_INCLUDED
#error BUT_H_INCLUDED must be defined
#endif

extern but_test_case tc1;
extern but_test_case tc2;

#endif
