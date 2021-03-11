#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// 8 bit
#if 0
typedef bit           BOOL;
#else
typedef signed char    BOOL,   *PBOOL;
#endif
typedef signed char    CHAR, INT8,   *PINT8;
typedef unsigned char  UINT8,  *PUINT8, UCHAR;
typedef unsigned char  BYTE,   *PBYTE;

// 16 bit
typedef short          INT16,  *PINT16;
typedef unsigned short USHORT, UINT16, *PUINT16;
typedef unsigned short WORD,   *PWORD;
typedef unsigned int   UINT,   *PUINT;
typedef	signed short int	SSHORT;

// 32 bit
typedef signed long    INT32,  *PINT32;
typedef unsigned long  ULONG, UINT32, *PUINT32;
typedef unsigned long  DWORD,  *PDWORD;

typedef void           VOID,   *PVOID;

#define CHAR     char 
#define CONST    const
#define STATIC   static
#define VOLATILE volatile

#ifndef NULL
#define NULL     0
#endif
#ifndef FALSE
#define FALSE    0
#endif
#ifndef TRUE
#define TRUE     1
#endif

#define NUM_TO_STR(num) (num + '0')

#define SET_BYTE_BIT(byte, bit) byte |= (bit)
#define CLEAR_BYTE_BIT(byte, bit) byte &= ~(bit)

typedef VOID (*PCALLBACK)(PVOID pvParams);

#endif

