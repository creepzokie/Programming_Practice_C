/**
 * Copyright (c) 2003-2018 by Hyoung Bok Min, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab : Linked List
 *
 * This is the main program provided to students who registered
 * in "EE Programming Lab" at School of Electrical Engineering,
 * Sungkyunkwan University.
 *
 * ================================================================
 * You must use function "mymalloc()" for memory allocation, and
 * you have to use function "myfree()" for disposing memory.
 * DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 *
 *    void *mymalloc(unsigned int byte_count);
 *    void myfree(void *ptr);
 * ================================================================
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define  _MEMCHECK_H_SIZE_OVER 5000
#include "memcheck.h"

/**
 *    NODE data structure used in this program
 */
typedef struct list_node {
    struct list_node *link;
    int data;
} NODE;


int headInsert(NODE **head, NODE **tail, int data);
int headDelete(NODE **head, NODE **tail, int *data);
int tailInsert(NODE **head, NODE **tail, int data);
int secondDelete(NODE **head, NODE **tail, int *data);
void deleteList(NODE **head, NODE **tail);
static void printList(NODE *head);


int main(void) {
    NODE *head = 0;    // head pointer of linked list
    NODE *tail = 0;    // tail pointer of linked list
    int  data0[] = { 99, 89, 76, 65, 51, 43, 23, 18, 9 };
    int  data1[] = { 512, 415, 328, 256, 128 };
    int  *data_used;   // either data0 or data1
    int  num;          // number of data in data_used
    int  loop;         // loop variable
    int  i_data;       // integer data of linked list
    int  phase;        // we will perform same job twice in phase 0 and 1

    for (phase = 0 ; phase < 2 ; phase++) {
        if (phase) {
            data_used = data1;      // This data set is used in phase 1
            num = sizeof(data1)/sizeof(int);
        } else {
            data_used = data0;      // This data set is used in phase 0
            num = sizeof(data0)/sizeof(int);
        }

        /**
         *    [1] Create a list with headInsert
         */
        printf("\nPerform headInsert at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            if (!headInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E1] Head insertion failed(1).\n");
                return 1;
            }
            printf("  a node added to head :");
            printList(head);
            printf("\n");
        }
        if (tail->link) {
            fprintf(stderr, "[E2] Head insertion failed(2).\n");
            return 2;
        }
        printf("  headInsert expected  :");
        for (loop = num-1 ; loop >= 0 ; loop--)
            printf(" %d", data_used[loop]);
        printf("\n");
#ifdef PRINT_RESULT
        /*
         *    This block is for grading purpose only. Leave this block as was.
         */
        printf("  headInsert performed :");
        printList(head);
        printf("\n");
#endif

        /**
         *    [2] Delete head nodes
         */
        printf("\nPerform headDelete at phase %d\n", phase);
        for (loop = num-1 ; loop >= 0 ; loop--) {
            printf("  deleting a head node :");
            printList(head);
            printf("\n");
            if (!headDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "[E3] Head deletion failed (1).\n");
                return 3;
            }
            if (i_data != data_used[loop]) {
                fprintf(stderr, "[E4] Head deletion failed (2).\n");
                return 4;
            }
        }
        if (headDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "[E5] Head deletion failed (3).\n");
            return 5;
        }
        printf("  headDelete performed okay.\n");

        /**
         *    [3] Create a list with tailInsert
         */
        printf("\nPerform tailInsert at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            if (!tailInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E6] Tail insertion failed(1).\n");
                return 6;
            }
            printf("  a node added to tail :");
            printList(head);
            printf("\n");
        }
        if (tail->link) {
            fprintf(stderr, "[E7] Tail insertion failed(2).\n");
            return 7;
        }
        printf("  tailInsert expected  :");
        for (loop = 0 ; loop < num ; loop++)
            printf(" %d", data_used[loop]);
        printf("\n");
#ifdef PRINT_RESULT
        /*
         *    This block is for grading purpose only. Leave this block as was.
         */
        printf("  tailInsert performed :");
        printList(head);
        printf("\n");
#endif

        /**
         *    [4] Delete next to head nodes by using secondDelete()
         */
        printf("\nPerform secondDelete at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            int expected_index = loop+1;
            if (loop == (num-1))
                expected_index = 0;
            printf("  deleting the next-to-head node :");
            printList(head);
            printf("\n");
            if (!secondDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "[E8] Second deletion failed (1).\n");
                return 8;
            }
            if (i_data != data_used[expected_index]) {
                fprintf(stderr, "[E9] Second deletion failed (2).\n");
                return 9;
            }
        }
        if (secondDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "[E10] Second deletion failed (3).\n");
            return 10;
        }
        printf("  secondDelete performed okay.\n");

        /**
         *    [5] Test deleteList()
         */
        // Build a linked list for deletion
        for (loop = 0 ; loop < num ; loop++) {
            if (!headInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E11] Head insertion failed(3).\n");
                return 11;
            }
        }
        if (tail->link) {
            fprintf(stderr, "[E12] Head insertion failed(4).\n");
            return 12;
        }
        // Delete the list
        deleteList(&head, &tail);

        /**
         *    [6] Check the list, and perform the same operation once more.
         */
        if (head || tail) {
            fprintf(stderr, "[E13] List is not properly deleted.\n");
            return 13;
        }
        printf("End of this phase.\n");
    }

    /**
     *    [7] Epilog : check memory used by this program.
     *        WARNING: DO NOT REMOVE THE FOLLOWING FUNCTION CALL.
     */
    check_memory();
    return 0;
}


/**
 *    Print a list from head to tail
 */
static void printList(NODE *head) {
    NODE *node = head;

    while (node) {
        printf(" %d", node->data);
        node = node->link;
    }
}


