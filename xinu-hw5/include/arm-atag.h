/**
 * @file arm-atag.h
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#ifndef  _ARM_ATAG_H_
#define  _ARM_ATAG_H_

/**
 * Definitions of usable ARM boot tags. ATAG list is a list of parameters
 * passed from the bootloader to the kernel. atags_ptr is passed inside
 * start.S as a parameter.
 */

enum
{
    ATAG_NONE = 0x00000000,     //Empty tag used to end list
    ATAG_CORE = 0x54410001,     //First tag used to start list
    ATAG_MEM = 0x54410002,      //Describes a physical area of memory
    ATAG_VIDEOTEXT = 0x54410003,        //Describes a VGA text display
    ATAG_RAMDISK = 0x54410004,  //Describes how the ramdisk will be used in kernel
    ATAG_INITRD2 = 0x54410005,  //Describes where the compressed ramdisk image is placed in memory
    ATAG_SERIAL = 0x54410006,   //64 bit board serial number
    ATAG_REVISION = 0x54410007, //32 bit board revision number
    ATAG_VIDEOLFB = 0x54410008, //Initial values for vesafb-type framebuffers
    ATAG_CMDLINE = 0x54410009,  //Command line to pass to kernel
};

/* Below we only define structures for tags we actually use.  */

/* ARM boot tag header.  */
struct atag_header
{
    uint size;                  /* Size of tag, in words, including the header.  */
    uint tag;                   /* One of the ATAG_* values from above.          */
};

/* Description of memory region (ATAG_MEM)  */
struct atag_mem
{
    uint size;
    uint start;
};

/* Board serial number (ATAG_SERIAL)  */
struct atag_serialnr
{
    uint low;
    uint high;
};

/* Format of ARM boot tag  */
struct atag
{
    struct atag_header hdr;
    union
    {
        struct atag_mem mem;
        struct atag_serialnr serialnr;
    };
};

/** 
 * Physical memory address at which the bootloader placed the ARM boot tags.
 * This is set by the code in start.S.  Here, initialize it to a dummy value to
 * prevent it from being placed in .bss.
 */
extern const struct atag *atags_ptr;

/* Raspberry Pi new-style revision codes:
 * https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
 * NOQu uuWu FMMM CCCC PPPP TTTT TTTT RRRR
 */

#define RPI_BOARDREV_MASK_OVER	0x80000000UL    /* Overvoltage allowed     */
#define RPI_BOARDREV_MASK_OTPP	0x40000000UL    /* OTP Programming allowed */
#define RPI_BOARDREV_MASK_OTPR	0x20000000UL    /* OPR reading allowed     */
#define RPI_BOARDREV_MASK_WARR	0x02000000UL    /* Warranty is intact      */
#define RPI_BOARDREV_MASK_NEW	0x00800000UL    /* New-style revision code */
#define RPI_BOARDREV_MASK_MEM	0x00700000UL    /* Memory size             */
#define RPI_BOARDREV_MASK_MANU	0x000F0000UL    /* Manufacturer            */
#define RPI_BOARDREV_MASK_PROC	0x0000F000UL    /* Processor SoC           */
#define RPI_BOARDREV_MASK_TYPE	0x00000FF0UL    /* Type of RasPi           */
#define RPI_BOARDREV_MASK_REV	0x0000000FUL    /* Revision number         */

#define RPI_BOARDREV_MEM_256M	0x00000000UL    /* Pi has 256 MB RAM       */
#define RPI_BOARDREV_MEM_512M	0x00100000UL    /* Pi has 512 MB RAM       */
#define RPI_BOARDREV_MEM_1G	0x00200000UL    /* Pi has 1 GB   RAM       */
#define RPI_BOARDREV_MEM_2G	0x00300000UL    /* Pi has 2 GB   RAM       */
#define RPI_BOARDREV_MEM_4G	0x00400000UL    /* Pi has 4 GB   RAM       */
#define RPI_BOARDREV_MEM_8G	0x00500000UL    /* Pi has 8 GB   RAM       */

#endif                          /* _ARM_ATAG_H_ */
