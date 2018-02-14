/* ========================================================================
 * (C) Copyright 2015,2017 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

#ifndef DLIST_H_INCLUDED
#define DLIST_H_INCLUDED

#include "platform.h"

struct dlist
{
    struct dlist *head;
    struct dlist *tail;
};
typedef struct dlist dlist;

#define dlist_init(node) do{(node)->tail = (node)->head = (node);} while(0)

// Set a node to the first and only element of a dlist
#define dlist_init_one(dlist, node)             \
    do                                          \
    {                                           \
        (dlist)->head = (dlist)->tail = (node); \
        (node)->head = (node)->tail = (dlist);  \
    } while(0)


#define dlist_insert_head(dlist, node)  \
    do                                  \
    {                                   \
        (node)->head = (dlist)->head;   \
        (node)->tail = (dlist);         \
        (node)->head->tail = (node);    \
        (dlist)->head = (node);         \
    } while(0)

// Insert a node at the end of the list
#define dlist_insert_tail(dlist, node)  \
    do                                  \
    {                                   \
        (node)->tail = (dlist)->tail;   \
        (node)->head = (dlist);         \
        (dlist)->tail->head = (node);   \
        (dlist)->tail = (node);         \
    } while(0)

// Remove node from a list
#define dlist_remove(node)                  \
    do                                      \
    {                                       \
        (node)->head->tail = (node)->tail;  \
        (node)->tail->head = (node)->head;  \
        (node)->head = (node);              \
        (node)->tail = (node);              \
    } while(0)

// Remove node from a list, but don't reset the node's links
#define dlist_remove_simple(node)           \
    do                                      \
    {                                       \
        (node)->head->tail = (node)->tail;  \
        (node)->tail->head = (node)->head;  \
    } while(0)

#define dlist_is_empty(node)    ((node)->head == (node))
#define dlist_next(node)        ((node)->head)
#define dlist_previous(node)    ((node)->tail)
#define dlist_head(node)        ((node)->head)

// The node is its own tail
#define dlist_tail(node)        ((node))


#endif  // DLIST_H_INCLUDED
