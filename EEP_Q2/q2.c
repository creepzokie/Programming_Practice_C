/**
 * Copyright (c) 2012-2018 by Hyoung Bok Min, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Quiz # 2 : Doubly Linked List
 *
 * This is the main program provided to students who registered
 * in "Programming Lab" at School of Information and Communication,
 * Sungkyunkwan University.
 *
 * ================================================================
 *  You must use function "mymalloc()" for memory allocation, and
 *  you have to use function "myfree()" for disposing memory.
 *  DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 * ================================================================
 *  WARNING : YOU CANNOT REMOVE OR MODIFY EXISTING CODE.
 *            YOU CAN USE NEITHER GLOBAL VARIABLES NOR FIXED SIZED ARRAYS.
 *            YOU CAN ADD PROGRAM CODE.
 *            YOU CAN ADD OR REMOVE COMMENTS FOR GOOD PROGRAM STYLE.
 * =======================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memcheck.h"

/**
 *    The following functions are given in this file.
 *    YOU MUST USE "mymalloc()" and "myfree()" for dynamic memory allocation.
 *
 *    static void *mymalloc(unsigned int);
 *    static void myfree(void *);
 */

/**
 *    NODE data structure used in this program
 */
typedef struct list_node {
    struct list_node *right;   /* link from head to tail */
    struct list_node *left;    /* link from tail to head */
    int data;
} NODE;

int headInsert(NODE **head, NODE **tail, int i_data);
int headDelete(NODE **head, NODE **tail, int *i_data);
int tailInsert(NODE **head, NODE **tail, int i_data);
int tailDelete(NODE **head, NODE **tail, int *i_data);
void deleteList(NODE **head, NODE **);
static void printListfromHead(NODE *);
static void printListfromTail(NODE *);


int main(void) {
    NODE *head = 0;
    NODE *tail = 0;
    int  data0[] = { 99, 89, 76, 65, 51, 43, 23, 18, 9 };
    int  data1[] = { 512, 415, 328, 256, 128 };
    int  *data_used;
    int  i, num, i_data, phase, num_data;

    for (phase = 0 ; phase < 2 ; phase++) {
        if (phase) {
            data_used = data1;
            num_data = sizeof(data1)/sizeof(int);
        } else {
            data_used = data0;
            num_data = sizeof(data0)/sizeof(int);
        }

        /**
         *    [1] Create a list with headInsert
         */
        printf("\nPerform headInsert at phase %d\n", phase);
        for (i = 0 ; i < num_data ; i++) {
            if (data_used[i] < 0)
                break;
            if (!headInsert(&head, &tail, data_used[i])) {
                fprintf(stderr, "ERROR: Head insertion failed.\n");
                return 1;
            }
        }
        num = i;
        printf("  headInsert expected  :");
        for (i = num-1 ; i >= 0 ; i--)
            printf(" %d", data_used[i]);
        printf("\n");
        printf("  headInsert performed :");
        printListfromHead(head);
        printf("\n");
        printf("             reversed list of the above :");
        printListfromTail(tail);
        printf("\n");

        /**
         *    [2] Delete head nodes
         */
        printf("\nPerform headDelete at phase %d\n", phase);
        for (i = num-1 ; i >= 0 ; i--) {
            printf("  deleting a head node :");
            printListfromHead(head);
            printf("\n");
            printf("             reversed list of the above :");
            printListfromTail(tail);
            printf("\n");
            if (!headDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "ERROR: Head deletion failed (1).\n");
                return 1;
            }
            if (i_data != data_used[i]) {
                fprintf(stderr, "ERROR: Head deletion failed (2).\n");
                return 1;
            }
        }
        if (headDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "ERROR: Head deletion failed (3).\n");
            return 1;
        }
        printf("  headDelete performed okay.\n");

        /**
         *    [3] Create a list with tailInsert
         */
        printf("\nPerform tailInsert at phase %d\n", phase);
        for (i = 0 ; i < num_data ; i++) {
            if (data_used[i] < 0)
                break;
            if (!tailInsert(&head, &tail, data_used[i])) {
                fprintf(stderr, "ERROR: Tail insertion failed.\n");
                return 1;
            }
        }
        num = i;
        printf("  tailInsert expected  :");
        for (i = 0 ; i < num ; i++)
            printf(" %d", data_used[i]);
        printf("\n");
        printf("  tailInsert performed :");
        printListfromHead(head);
        printf("\n");
        printf("             reversed list of the above :");
        printListfromTail(tail);
        printf("\n");

        /**
         *    [4] Delete tail nodes
         */
        printf("\nPerform tailDelete at phase %d\n", phase);
        for (i = num-1 ; i >= 0 ; i--) {
            printf("  deleting a tail node :");
            printListfromHead(head);
            printf("\n");
            printf("             reversed list of the above :");
            printListfromTail(tail);
            printf("\n");
            if (!tailDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "ERROR: Tail deletion failed (1).\n");
                return 1;
            }
            if (i_data != data_used[i]) {
                fprintf(stderr, "ERROR: Tail deletion failed (2).\n");
                return 1;
            }
        }
        if (tailDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "ERROR: Tail deletion failed (3).\n");
            return 1;
        }
        printf("  tailDelete performed okay.\n");

        /**
         *    [5] Delete the list, and perform the same operation once more.
         */
        printf("\nDeleting current list at phase %d\n", phase);

        /* delete a vacant list */
        deleteList(&head, &tail);
        if (head || tail) {
            fprintf(stderr, "ERROR: List is not deleted (1).\n");
            return 1;
        }

        /* make a new list for deletion test */
        for (i = 0 ; i < num_data ; i++) {
            if (data_used[i] < 0)
                break;
            if (!tailInsert(&head, &tail, data_used[i])) {
                fprintf(stderr, "ERROR: Tail insertion failed ");
                fprintf(stderr, "during list deletion test.\n");
                return 1;
            }
        }

        /* delete the list that is newly created above */
        deleteList(&head, &tail);
        if (head || tail) {
            fprintf(stderr, "ERROR: List is not deleted (2).\n");
            return 1;
        }
        printf("deleteList performed okay\n");
    }
    check_memory();
    return 0;
}


/**
 *    Print a list from head to tail.
 */
static void printListfromHead(NODE *head) {
    NODE *node = head;

    while (node) {
        printf(" %d", node->data);
        node = node->right;
    }
}


/**
 *    Print a list from tail to head.
 */
static void printListfromTail(NODE *tail) {
    NODE *node = tail;

    while (node) {
        printf(" %d", node->data);
        node = node->left;
    }
}


