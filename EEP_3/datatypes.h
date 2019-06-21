/**
 * Copyright (c) 2003-2016 by Hyoung Bok Min, All rights reserved.
 * For license information, please refer to
 * http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * File name : datatypes.h
 *
 * This header file defines BYTE, WORD, DWORD, and INT.
 *
 * This is the program provided to students who registered
 * in "Programming Lab" at School of Information and Communication,
 * Sungkyunkwan University.
 */


typedef unsigned char  BYTE;     // 8-bit unsigned
typedef unsigned short WORD;     // 16-bit unsigned
typedef unsigned int   DWORD;    // 32-bit unsigned
typedef int            INT;      // 32-bit signed
typedef int            LONG;     // 32-bit signed

#define sizeofBYTE   (sizeof(BYTE))
#define sizeofWORD   (sizeof(WORD))
#define sizeofDWORD  (sizeof(DWORD))
#define sizeofINT    (sizeof(INT))
#define sizeofLONG   (sizeof(LONG))

/*--------------  datatypes.h  --------------*/
