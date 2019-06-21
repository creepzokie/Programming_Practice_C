/**
 *  Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 *  Copyright (C) 2018 by Hae Jin Jung, All rights reserved.
 *  For license information, please refer to
 *      http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  complier used : GNU GCC Compiler 7.2
 *
 *  Quiz #2 of C Programming : Doubly Linked List
 *
 *  Summary of this lab :
 *  Write 5 functions
 *  • int headInsert(NODE **head, NODE **tail, inti_data);
 *  • int headDelete(NODE **head, NODE **tail, int*i_data);
 *  • int tailInsert(NODE **head, NODE **tail, inti_data);
 *  • int tailDelete(NODE **head, NODE **tail, int*i_data);
 *  • void deleteList(NODE **head, NODE **tail);
 *  NOTE: The above 4 functions return 1 if data is actually
 *        inserted or deleted, and returns 0 if data cannot be
 *        inserted or delete at any reason.
 *  Two functions are given
 *  • printListfromHead(NODE *head);
 *  • printListfromTail(NODE *tail);
 *
 *  Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 *  File name : 2017311583.JungHaeJin.Q2.c
 *
 *  Written on September 01, 2011
 *  Modification History :
 *     1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *     2. modified by Hae Jin Jung on June 11, 2018
 *        (a) Add codes for reading and writing text file (homeworks).
 *
 *  This is the main program provided to students who registered
 *  in "Programming Lab" at School of Information and Communication,
 *  Sungkyunkwan University.
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

int headInsert(NODE **head_pointer, NODE **tail_pointer, int i_data) {
    // memory allocation of saving input data which is composed of an integer
    // and address of the next data chuck.
    NODE *itemHEAD = (NODE *)mymalloc(sizeof(NODE));
    // alert error if memory allocation fails.
    if (!itemHEAD) {
        fprintf(stderr, "[ERROR] Out of memory.\n");
        return 0;
    }
    // input the integer
    itemHEAD->data = i_data;
    // input the address of pointer
    itemHEAD->right = *head_pointer;
    itemHEAD->left = NULL;
    // if data chunk is one, tail pointer points to first data chuck
    if (!(*head_pointer)) {
        *tail_pointer = itemHEAD;
    } else {
    // link data chunks
        (*head_pointer)->left = itemHEAD;
    }
    *head_pointer = itemHEAD;
    return 1;
}

int headDelete(NODE **head_pointer, NODE **tail_pointer, int *p_i_data) {
    // set head data to ready to delete data
    NODE *itemHEAD = *head_pointer;
    if (!itemHEAD) {
        return 0;
    }
    // cut a link of two adjacent data chunks
    *p_i_data = itemHEAD->data;
    // link a data chunk and subsequent data chunk
    *head_pointer = itemHEAD->right;
    // if data chunk is one, set tail pointer also 0
    if (!(*head_pointer)) {
        *tail_pointer = 0;
    } else {
        (*head_pointer)->left = NULL;
    }
    // free the memory chunk
    myfree(itemHEAD);
    return 1;
}

int tailInsert(NODE **head_pointer, NODE **tail_pointer, int i_data) {
    // memory allocation of saving input data which is composed of an integer
    // and address of the next data chuck.
    NODE *itemTAIL = (NODE *)mymalloc(sizeof(NODE));
    // alert error if memory allocation fails.
    if (!itemTAIL) {
        fprintf(stderr, "[ERROR] Out of memory.\n");
        return 0;
    }
    // set tail pointer 0 to link to new data chunk
    itemTAIL->right = NULL;
    // input the integer
    itemTAIL->data = i_data;
    if (!(*head_pointer)) {
    // if data chunk is one, head pointer points to first data chuck
        itemTAIL->left = NULL;
        *head_pointer = itemTAIL;
    } else {
    // the second data chunk from behind points to new data chuck
        (*tail_pointer)->right = itemTAIL;
        itemTAIL->left = *tail_pointer;
    }
    *tail_pointer = itemTAIL;
    return 1;
}

int tailDelete(NODE **head_pointer, NODE **tail_pointer, int *p_i_data) {
    // set head data to ready to delete data
    NODE *itemTAIL = *tail_pointer;
    if (!itemTAIL) {
        return 0;
    }
    // cut a link of two adjacent data chunks
    *p_i_data = itemTAIL->data;
    // link a data chunk and subsequent data chunk
    *tail_pointer = itemTAIL->left;
    // if data chunk is one, set tail pointer also 0
    if (!(*tail_pointer)) {
        *head_pointer = 0;
    } else {
        (*tail_pointer)->right = NULL;
    }
    // free the memory chunk
    myfree(itemTAIL);
    return 1;
}

void deleteList(NODE **head_pointer, NODE **tail_pointer) {
    // repeat 'headDelete' function
    NODE *itemHEAD = *head_pointer;
    if (itemHEAD) {
        while (*head_pointer) {
            *head_pointer = itemHEAD->right;
            if (*head_pointer) {
                (*head_pointer)->left = NULL;
                myfree(itemHEAD);
            } else {
                myfree(itemHEAD);
            }
            itemHEAD = *head_pointer;
        }
        *head_pointer = *tail_pointer = NULL;
    }
}
