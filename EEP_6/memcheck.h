/**
 *    Copyright (c) 2012-2018 by Hyoung Bok Min, All rights reserved.
 *    For copyright and license information, please visit
 *    http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    File name : memcheck.h, revision c181
 *
 *    =================================================================
 *    There are three functions which are written to make sure that
 *    your memory management is good enough.
 *
 *        void *mymalloc(unsigned int byte_count);
 *               Use this function as a substitute for "malloc()".
 *
 *        void myfree(void *ptr);
 *               Use this function as a substitute for "free()".
 *
 *        void check_memory(void);
 *               You have to call this function immediately before "return"
 *               statements in your "main()" function. If you do not honor
 *               this rule, your grade will be 0, absolutely.
 *
 *    The following functions are used as subsitutes of fopen() and fclose()
 *    to make sure that you're using those functions correctly.
 *
 *        FILE *myfopen(const char *path, const char *mode);
 *               Use this function as a substitute for "fopen()".
 *
 *        int  myfclose(FILE *stream);
 *               Use this function as a substitute for "fclose()".
 *
 *    PLEASE DO NOT MODIFY ANY PROGRAM CODES IN THIS FILE.
 *    =================================================================
 */

#ifndef _MEMCHECK_H
#define _MEMCHECK_H

#ifdef _MEMCHECK_H_EXTERN
#ifdef _MEMCHECK_H_SRCFILE
extern void *mymymalloc(unsigned int byte_count, char *srcfile, int srcline);
extern void mymyfree(void *ptr, char *srcfile, int srcline);
#else
extern void *mymalloc(unsigned int byte_count);
extern void myfree(void *ptr);
#endif
extern void check_memory(void);
extern void *_memcheck_h_do_not_use(char *);
#ifndef O_NOT_USE_MEMCHECK_H_FOPEN
extern FILE *myfopen(const char *path, const char *mode);
extern int  myfclose(FILE *stream);
#endif
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef UINT_PTR_T
#error Please change the string UINT_PTR_T to another in memcheck.h.
#endif

#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define UINT_PTR_T unsigned long long int
    static UINT_PTR_T _mem_masks[] = { 0xffffffffff000000, 0xffffffff00000000,
                   0xffffff0000000000, 0xffff000000000000, 0xff00000000000000};
  #else
    #define UINT_PTR_T unsigned long int
    static UINT_PTR_T _mem_masks[] = {0xffffff00, 0xffff0000, 0xff000000};
  #endif
#else
  #if _WIN32
    #define UINT_PTR_T unsigned long int
    static UINT_PTR_T _mem_masks[] = {0xffffff00, 0xffff0000, 0xff000000};
  #else
    #define UINT_PTR_T unsigned long long int
    static UINT_PTR_T _mem_masks[] = { 0xffffffffff000000, 0xffffffff00000000,
                   0xffffff0000000000, 0xffff000000000000, 0xff00000000000000};
  #endif
#endif  /* __GNUC__ */

typedef struct _mem_chunk {
    struct _mem_chunk *next;
    void *ptr;
    UINT_PTR_T *pcode;
    UINT_PTR_T code;
    UINT_PTR_T mask;
    UINT_PTR_T size;
#ifdef _MEMCHECK_H_SRCFILE
    char *srcfile;
    int  srcline;
#endif
} _MEM_CHUNK;

static int _peak_mems = 0;
static int _peak_num_mems = 0;
static int _largest_mems = 0;
static int _current_mems = 0;
static int _size_mems = 0;
static int _num_mems = 0;
static void *_mems_used = 0;
static _MEM_CHUNK **_mems_table = 0;


#ifndef O_NOT_USE_MEMCHECK_H_FOPEN
typedef struct _mem_files_chunk {
    struct _mem_files_chunk *next;
    FILE *fp;
    char *path;
    char *mode;
} _MEM_FILES_CHUNK;

#define _MEM_FILES_MAX         256
#define _MEM_FILES_MODE_MAX    3
static int _mem_num_files = 0;
static int _mem_num_peak_files = 0;
static _MEM_FILES_CHUNK *_mem_files_head = 0;
#endif


/**
 * Get a mask of leading 1's in which the number of bits of 1's are equal to
 * the number of leading zero's at the given 'number'.
 */
static UINT_PTR_T _get_leading_zeros(UINT_PTR_T number) {
    UINT_PTR_T mask;
    int i, k, num_mem_masks = sizeof(_mem_masks) / sizeof(UINT_PTR_T);
    for (i = 0 ; i < num_mem_masks ; i++) {
        if ((number & ~(_mem_masks[i])) == number) {
            if (i == 0)
                return _mem_masks[i];
            mask = _mem_masks[i-1] << 1;
            for (k = 0 ; k < 8 ; k++) {
                if ((number & ~mask) == number)
                    return mask;
                mask <<= 1;
            }
        }
    }
    mask = _mem_masks[num_mem_masks-1] << 1;
    for (k = 0 ; k < 8 ; k++) {
         if ((number & ~mask) == number)
             return mask;
         mask <<= 1;
    }
    return 0;
}

/**
 * hash function, courtesy of Bob Jenkins from
 * http://burtleburtle.net/bob/hash/integer.html
 * This function is declared in public domain at the above web page.
 * You may also find good hash functions at github,
 * https://gist.github.com/badboy/6267743
 */
static int _mem_hash(UINT_PTR_T val, UINT_PTR_T size) {
    val = val ^ (val >> 4);
    val = (val ^ 0xdeadbeef) + (val << 5);
    val = val ^ (val >> 11);
    val = val % size;
    return (int)val;
}

/* Extract item of key 'ptr' from hash table. */
static _MEM_CHUNK *_mem_extract(void *ptr) {
    _MEM_CHUNK *item, *prev = (_MEM_CHUNK *)0;
    int hash = _mem_hash((UINT_PTR_T)ptr, (UINT_PTR_T)_size_mems);
    assert(hash >= 0 && hash < _size_mems);
    item = _mems_table[hash];
    while (item) {
        if (item->ptr == ptr) {
            if (prev)
                prev->next = item->next;
            else
                _mems_table[hash] = item->next;
            return item;
        }
        prev = item;
        item = item->next;
    }
    return (_MEM_CHUNK *)0;
}

/* Insert an item into hash table. */
static void _mem_insert(_MEM_CHUNK *item) {
    int hash = _mem_hash((UINT_PTR_T)(item->ptr), (UINT_PTR_T)_size_mems);
    assert(hash >= 0 && hash < _size_mems);
    item->next = _mems_table[hash];
    _mems_table[hash] = item;
}

/* Get a new hash table size. */
static int _mem_new_size(int size) {
    int num, index;
    int primes[7] = { 3, 5, 7, 11, 13, 17, 19};
    int begin = size + size + 1;
    if (begin < 0)
        return size;
    for (num = begin ; num > 0 ; num += 2) {
        for (index = 0 ; index < 7 ; index++) {
            if (num % primes[index] == 0)
                break;
        }
        if (index >= 7)
            return num;
    }
    return size;
}

/* Create a new hash table with size of twice. */
static void _mem_rehash(void) {
    int i, old_size = _size_mems;
    _MEM_CHUNK **old_table = _mems_table;

    _size_mems = _mem_new_size(old_size);
    if (_size_mems == old_size)
        return;
    _mems_table = (_MEM_CHUNK **)malloc(_size_mems*sizeof(_MEM_CHUNK *));
    if (!_mems_table) {
        _mems_table = old_table;
        _size_mems = old_size;
        return;
    }
    for (i = 0 ; i < _size_mems ; i++)
        _mems_table[i] = (_MEM_CHUNK *)0;
    i = 0;
    for ( ; i < old_size ; i++) {
        _MEM_CHUNK *item = old_table[i];
        while (item) {
             _MEM_CHUNK *nextitem = item->next;
             _mem_insert(item);
             item = nextitem;
        }
    }
    free(old_table);
}

#ifdef _MEMCHECK_H_SRCFILE
void print_leaked_mems(void) {
    _MEM_CHUNK *item;
    int i, first = 1;
    if (!_mems_table)
        return;

    for (i = 0 ; i < _size_mems ; i++) {
        item = _mems_table[i];
        while (item) {
            if (first) {
                first = 0;
                fprintf(stderr, "[NOTE] The above %d ", _num_mems);
                fprintf(stderr, "chunks of memories are as follows.\n");
            }
#ifdef _WIN32
            // Windows does not support both "%lld" and "%llu".
            fprintf(stderr, "       %u bytes by mymalloc",
                            (unsigned int)(item->size));
#else
            fprintf(stderr, "       %llu bytes by mymalloc",
                            (unsigned long long int)(item->size));
#endif
            fprintf(stderr, " at line %d", item->srcline);
            fprintf(stderr, " of file '%s'\n", item->srcfile);
            item->size;
            item = item->next;
        }
    }
}
#endif


/**
 * Substitute for malloc()
 */
#ifdef _MEMCHECK_H_SRCFILE
void *mymymalloc(unsigned int byte_count, char *srcfile, int srcline) {
#else
void *mymalloc(unsigned int byte_count) {
#endif
    _MEM_CHUNK *the_mem;
    UINT_PTR_T for_code;
#ifdef _MEMCHECK_H_SRCFILE
    int length;
#endif
    unsigned int ptr_size = sizeof(char *);
    unsigned char *char_ptr, *to_user;
    int  index;

    if (byte_count <= 0) {
#ifdef _MEMCHECK_H_SIZE_OVER
        fprintf(stderr, "\n[WARNING] You're requesting memory of size 0 ");
        fprintf(stderr, "bytes. Are you sure ?\n");
#ifdef _MEMCHECK_H_SRCFILE
        fprintf(stderr, "[NOTE] The above warning is given by mymalloc");
        fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
#endif
#endif
        return (void *)0;
    }

#if _MEMCHECK_H_SIZE_OVER > 1
    if (byte_count > _MEMCHECK_H_SIZE_OVER) {
        fprintf(stderr, "\n[WARNING] You're requesting memory of size ");
        fprintf(stderr, "%d bytes. Are you sure ?\n", byte_count);
#ifdef _MEMCHECK_H_SRCFILE
        fprintf(stderr, "[NOTE] The above warning is given by mymalloc");
        fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
#endif
#ifdef _MEMCHECK_H_SIZE_OVER_NOTE
        fprintf(stderr, "[NOTE] If you want to suppress the above warning, ");
        fprintf(stderr, "remove the line which defines ");
        fprintf(stderr, "_MEMCHECK_H_SIZE_OVER in your program file.\n");
#endif
    }
#endif

    the_mem = (_MEM_CHUNK *)malloc(sizeof(_MEM_CHUNK));
    if (!the_mem)
        return (void *)0;

    char_ptr = (unsigned char *)malloc(byte_count + 2*ptr_size);
    to_user = char_ptr + ptr_size;
    the_mem->ptr = (void *)to_user;
    if (!char_ptr) {
        free(the_mem);
        return (void *)0;
    }
    the_mem->next = (_MEM_CHUNK *)0;
    the_mem->size = byte_count;
    for_code = (UINT_PTR_T)(the_mem->ptr);
    the_mem->mask = _get_leading_zeros(for_code);
    the_mem->code = the_mem->size ^ (for_code | the_mem->mask);
    the_mem->pcode = (UINT_PTR_T *)malloc(sizeof(UINT_PTR_T));
    if (!(the_mem->pcode)) {
        free(char_ptr);
        free(the_mem);
        return (void *)0;
    }
    *(the_mem->pcode) = the_mem->code;
#ifdef _MEMCHECK_H_SRCFILE
    length = strlen(srcfile);
    the_mem->srcfile = (char *)malloc((length+1)*sizeof(char));
    if (!(the_mem->srcfile)) {
        free(char_ptr);
        free(the_mem->pcode);
        free(the_mem);
        return (void *)0;
    }
    strcpy(the_mem->srcfile, srcfile);
    the_mem->srcline = srcline;
#endif
    _mems_used = the_mem;

    if (!_mems_table) {
        int i;
        _size_mems = 10091;  /* 10091, 99991 */
        _mems_table = (_MEM_CHUNK **)malloc(_size_mems*sizeof(_MEM_CHUNK *));
        assert(_mems_table);
        for (i = 0 ; i < _size_mems ; i++) {
            _mems_table[i] = (_MEM_CHUNK *)0;
        }
    } else if (_num_mems >= _size_mems) {
        _mem_rehash();
    }

    _mem_insert(the_mem);
    _num_mems++;
    if (byte_count > _largest_mems)
        _largest_mems = byte_count;
    if (_num_mems > _peak_num_mems)
        _peak_num_mems = _num_mems;
    _current_mems += byte_count;
    if (_current_mems > _peak_mems)
        _peak_mems = _current_mems;

    // 0xff for leadning and trailing memory of pointer size
    for (index = 0 ; index < ptr_size ; index++) {
        *(char_ptr+index) = 0xff;
        *(char_ptr+ptr_size+byte_count+index) = 0xff;
    }
    return (void *)(char_ptr+ptr_size);
}   /* end of mymalloc */


/**
 * Substitute for free()
 */
#ifdef _MEMCHECK_H_SRCFILE
void mymyfree(void *ptr, char *srcfile, int srcline) {
#else
void myfree(void *ptr) {
#endif
    UINT_PTR_T for_code, size;
    _MEM_CHUNK *item;
    unsigned int ptr_size = sizeof(char *);
    int  index, is_error;
    unsigned char *char_ptr;

    if (!ptr)
        return;
    assert(_mems_table);
    assert(_size_mems > 0);
    assert(_num_mems > 0);

    item = _mem_extract(ptr);
    if (!item) {
        char *msg = "\n[ERROR] Address provided to argument, which is "
                    "used to call \"myfree()\", is invalid, and it is not "
                    "elligibe for freeing a memory block. This message may "
                    "be given due to one of the following reasons: "
                    "(1) The argument may be a memory address which has "
                    "already been made free by previous myfree() call, "
                    "(2) The argument may be a memory address which has "
                    "never been allocated by mymalloc() call, "
                    "(3) The argument may be value of a pointer "
                    "variable which has never been initialized, "
                    "(4) There may be another unknown reason.";
        fprintf(stderr, "%s\n", msg);
#ifdef _MEMCHECK_H_SRCFILE
        fprintf(stderr, "[NOTE] The above error is given by myfree()");
        fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
#endif
        exit(125);
    }
    if (item->code != *(item->pcode)) {
        fprintf(stderr, "\n[ERROR] Memory corrupted (1).\n");
#ifdef _MEMCHECK_H_SRCFILE
        fprintf(stderr, "[NOTE] The above error is given by myfree");
        fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
        fprintf(stderr, "[NOTE] The above corrupted memory is allocated by");
        fprintf(stderr, " mymalloc at line %d", item->srcline);
        fprintf(stderr, " at file '%s'.\n", item->srcfile);
#endif
        exit(127);
    }

    for_code = (UINT_PTR_T)ptr;
    size = (item->code & ~(item->mask)) ^ for_code;

    if (item->size != size) {
        fprintf(stderr, "\n[ERROR] Memory corrupted (2).\n");
#ifdef _MEMCHECK_H_SRCFILE
        fprintf(stderr, "[NOTE] The above error is given by myfree");
        fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
        fprintf(stderr, "[NOTE] The above corrupted memory is allocated by");
        fprintf(stderr, " mymalloc at line %d", item->srcline);
        fprintf(stderr, " at file '%s'.\n", item->srcfile);
#endif
        exit(126);
    }

    // check overflow
    is_error = 0;
    char_ptr = (unsigned char *)ptr - ptr_size;   // actual address of memory
    for (index = 0 ; index < ptr_size ; index++) {
        if (*(char_ptr+index) != 0xff) {
            fprintf(stderr, "\n[ERROR] Memory overflow (1).\n");
#ifdef _MEMCHECK_H_SRCFILE
            fprintf(stderr, "[NOTE] The above error is given by myfree");
            fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
            fprintf(stderr, "[NOTE] The above corrupted memory is allocated ");
            fprintf(stderr, " by mymalloc at line %d", item->srcline);
            fprintf(stderr, " at file '%s'.\n", item->srcfile);
#endif
            if (is_error & 2)
                break;
            is_error |= 1;
        }
        if (*(char_ptr+ptr_size+size+index) != 0xff) {
            fprintf(stderr, "\n[ERROR] Memory overflow (2).\n");
#ifdef _MEMCHECK_H_SRCFILE
            fprintf(stderr, "[NOTE] The above error is given by myfree");
            fprintf(stderr, " at line %d at file '%s'.\n", srcline, srcfile);
            fprintf(stderr, "[NOTE] The above corrupted memory is allocated ");
            fprintf(stderr, " by mymalloc at line %d", item->srcline);
            fprintf(stderr, " at file '%s'.\n", item->srcfile);
#endif
            if (is_error & 1)
                break;
            is_error |= 2;
        }
    }

    // dispose memory
    _current_mems -= item->size;
    memset(item->ptr, -1, item->size);

    free(item->pcode);
#ifdef _MEMCHECK_H_SRCFILE
    free(item->srcfile);
#endif
    free(char_ptr);   // free(item->ptr);
    free(item);
    _num_mems--;

    if (!_num_mems) {
        free(_mems_table);
        _mems_table = (_MEM_CHUNK **)0;
        _size_mems = 0;
    }
}   /* end of myfree */


#ifndef O_NOT_USE_MEMCHECK_H_FOPEN
/* Search by path name */
static _MEM_FILES_CHUNK *_mem_files_search_by_path(const char *path) {
    _MEM_FILES_CHUNK *chunk = _mem_files_head;

    while (chunk) {
        if (!strcmp(chunk->path, path))
            return chunk;
        chunk = chunk->next;
    }
    return (_MEM_FILES_CHUNK *)0;
}

/* Search by file pointer */
static _MEM_FILES_CHUNK *_mem_files_search_by_fp(
                         FILE *fp, _MEM_FILES_CHUNK **prev) {
    _MEM_FILES_CHUNK *prev_chunk = (_MEM_FILES_CHUNK *)0;
    _MEM_FILES_CHUNK *chunk = _mem_files_head;

    while (chunk) {
        if (chunk->fp == fp) {
            *prev = prev_chunk;
            return chunk;
        }
        prev_chunk = chunk;
        chunk = chunk->next;
    }
    return (_MEM_FILES_CHUNK *)0;
}

/**
 * Substitute for fopen()
 */
FILE *myfopen(const char *path, const char *mode) {
    FILE *fp;
    _MEM_FILES_CHUNK *chunk;
    int  lenpath, lenmode;

    assert(path);
    assert(mode);
    lenpath = strlen(path);
    lenmode = strlen(mode);
    if (lenpath > _MEM_FILES_MAX) {
        fprintf(stderr, "\n[WARNING] File name is too long. ");
        fprintf(stderr, "The name is '%s'.\n", path);
    }
    if (lenmode > _MEM_FILES_MODE_MAX) {
        fprintf(stderr, "\n[ERROR] File mode is too long. ");
        fprintf(stderr, "The mode is '%s'.\n", mode);
        exit(140);
    }

    chunk = _mem_files_search_by_path(path);
    if (chunk) {
        fprintf(stderr, "\n[ERROR] File '%s' has already been opened ", path);
        fprintf(stderr, "with mode '%s'.\n", chunk->mode);
        exit(141);
    }

    fp = fopen(path, mode);
    if (!fp)
        return fp;

    chunk = (_MEM_FILES_CHUNK *)malloc(sizeof(_MEM_FILES_CHUNK));
    assert(chunk);
    chunk->fp = fp;
    chunk->path = (char *)malloc((lenpath+1)*sizeof(char));
    assert(chunk->path);
    strcpy(chunk->path, path);
    chunk->mode = (char *)malloc((lenmode+1)*sizeof(char));
    assert(chunk->mode);
    strcpy(chunk->mode, mode);
    chunk->next = _mem_files_head;
    _mem_files_head = chunk;
    _mem_num_files++;
    if (_mem_num_files > _mem_num_peak_files)
        _mem_num_peak_files = _mem_num_files;
    return fp;
}

/**
 * Substitute for fclose()
 */
int myfclose(FILE *stream) {
    _MEM_FILES_CHUNK *chunk, *prev;

    if (!stream) {
        fprintf(stderr, "\n[ERROR] File pointer to 'myfclose()' is NULL.\n");
        exit(145);
    }

    chunk = _mem_files_search_by_fp(stream, &prev);
    if (!chunk) {
        fprintf(stderr, "\n[ERROR] File given to 'myfclose()' ");
        fprintf(stderr, "has never been opened, ");
        fprintf(stderr, "or the file has already been closed.\n");
        exit(146);
    }
    if (prev)
        prev->next = chunk->next;
    else
        _mem_files_head = chunk->next;
    free(chunk->path);
    free(chunk->mode);
    free(chunk);
    _mem_num_files--;
    return fclose(stream);
}
#endif  /* O_NOT_USE_MEMCHECK_H_FOPEN */


/**
 * Check memeory management at the end of main().
 */
void check_memory(void) {
    printf("+++++ Checking memory... +++++\n");
#ifdef _MEMCHECK_H_GRADE
#if _MEMCHECK_H_GRADE > 9
#ifndef _MEMCHECK_H_SIZE_OVER
    printf("[WARNING] _MEMCHECK_H_SIZE_OVER is NOT defined.\n");
#else
#if _MEMCHECK_H_SIZE_OVER != _MEMCHECK_H_GRADE
    printf("[WARNING] Value of _MEMCHECK_H_SIZE_OVER has been modified.\n");
#endif
#endif
#endif
    printf("largest memory requested = %d bytes.\n", _largest_mems);
    printf("%d bytes in %d chunks at peak.\n", _peak_mems, _peak_num_mems);
#endif
    if (_num_mems > 0) {
        fprintf(stderr, "[ERROR] %d chunks of memories ", _num_mems);
        fprintf(stderr, "allocated by mymalloc() are not ");
        fprintf(stderr, "given back to operating system.\n");
#ifdef _MEMCHECK_H_SRCFILE
        print_leaked_mems();
#endif
        exit(124);
    }
#ifdef _MEMCHECK_H_WARNING
    if (!_mems_used) {
        fprintf(stderr, "[WARNING] You have not used mymalloc()/myfree() ");
        fprintf(stderr, "for dynamic memory allocation.\n");
    }
#endif
#ifndef O_NOT_USE_MEMCHECK_H_FOPEN
#ifdef _MEMCHECK_H_GRADE
    printf("%d files have been opened at peak.\n", _mem_num_peak_files);
#endif
    if (_mem_files_head) {
        _MEM_FILES_CHUNK *chunk = _mem_files_head;
        fprintf(stderr, "[ERROR] %d file(s) have been opened,", _mem_num_files);
        fprintf(stderr, " but they have never been closed.\n");
        while (chunk) {
             fprintf(stderr, "'%s' with mode '%s'\n", chunk->path, chunk->mode);
             chunk = chunk->next;
        }
        exit(150);
    }
#endif
    printf("+++++ Memory is okay. +++++\n");
}   /* end of check_memory */

/* To prohibit malloc/calloc/realloc/free/fopen/fclose/fdopen/freopen */
void *_memcheck_h_do_not_use(char *which) {
    fprintf(stderr, "\n");
    fprintf(stderr, "----------------------------\n");
    fprintf(stderr, "[ERROR] DO NOT USE %s().\n", which);
    fprintf(stderr, "----------------------------\n");
    exit(122);
}

#ifdef UINT_PTR_T
#undef UINT_PTR_T
#endif
#endif   /* _MEM_CHECK_H_EXTERN */

#define malloc(a)         _memcheck_h_do_not_use("malloc")
#define calloc(a, b)      _memcheck_h_do_not_use("calloc")
#define realloc(a, b)     _memcheck_h_do_not_use("realloc")
#define free(a)           _memcheck_h_do_not_use("free")

#ifndef O_NOT_USE_MEMCHECK_H_FOPEN
#define fopen(a, b)       _memcheck_h_do_not_use("fopen")
#define fdopen(a, b)      _memcheck_h_do_not_use("fdopen")
#define freopen(a, b, c)  _memcheck_h_do_not_use("freopen")
#define fclose(a)         _memcheck_h_do_not_use("fclose")
#endif  /* O_NOT_USE_MEMCHECK_H_FOPEN */

#ifdef _MEMCHECK_H_SRCFILE
#define mymalloc(x)  mymymalloc((x), __FILE__, __LINE__)
#define myfree(x)    mymyfree((x), __FILE__, __LINE__)
#endif  /* _MEMCHECK_H_SRCFILE */
#endif  /* _MEMCHECK_H */
/*-----------  end of memcheck.h  -----------*/
