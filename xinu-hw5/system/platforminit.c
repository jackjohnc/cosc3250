/**
 * @file platforminit.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

#define MAX 256

/** 
 * Physical memory address at which the bootloader placed the ARM boot tags.
 * This is set by the code in start.S.  Here, initialize it to a dummy value to
 * prevent it from being placed in .bss.
 */
const struct atag *atags_ptr = (void *)-1;

/* End of kernel (used for sanity check)  */
extern void *_end;

/* mask value, and shift right by count of trailing zeros in mask */
static inline ulong mask_and_shift(ulong value, ulong mask)
{
    return ((value & mask) >> __builtin_ctzl(mask));
}

/**
 * Parse a base-ten string passed as kernel commandline arguments
 * from the bootloader, expecting form "descriptor=12345".
 * Returns integer representation of number following '=', or -1.
 */
static ulong parse_decimal(char *s)
{
    ulong result = 0;
    char *search = NULL;
    search = strchr(s, '=');
    if (!search)
        return -1;
    search++;
    while (*search)
    {
        if (isdigit(*search))
            result = result * 10 + (*search - '0');
        search++;
    }
    return result;
}

/**
 * Parse a hex string passed as kernel commandline arguments
 * from the bootloader, expecting form "descriptor=0xF123".
 * Returns integer representation of number following '=', or -1.
 */
static ulong parse_hex(char *s)
{
    ulong result = 0;
    char *search = NULL;
    search = strchr(s, '=');
    if (!search)
        return -1;
    search++;
    if (*search++ != '0')
        return -1;
    if (*search++ != 'x')
        return -1;
    while (*search)
    {
        if (isdigit(*search))
            result = result * 16 + (*search - '0');
        else if (isxdigit(*search))
            result = result * 16 + toupper(*search) - 'A' + 10;
        search++;
    }
    return result;
}

/**
 * New-type RasPi board revision codes indicate:
 * manufacturer, model, revision, and RAM memory installed,
 * among other useful bits of knowledge.
 * These are parsed and stored in global platform struct.
 */
static void parse_boardrev(char *s)
{
    int revnumber = 0;
#ifdef DEBUG
    kprintf("boardrev = %s\r\n", s);
#endif                          /* DEBUG */
    revnumber = parse_hex(s);
#ifdef DEBUG
    kprintf("boardrev = 0x%08X\r\n", revnumber);
#endif                          /* DEBUG */
    platform.serial_high = revnumber;

    /* Detect new-style RasPi boardrev code */
    if (!(RPI_BOARDREV_MASK_NEW & revnumber))
        return;
#ifdef DEBUG
    kprintf("New-style BoardRev code found.\r\n");
#endif                          /* DEBUG */

    /* Detect manufacturer */
    switch (mask_and_shift(revnumber, RPI_BOARDREV_MASK_MANU))
    {
    case 0:
        strlcpy(platform.manufacturer, "Sony UK", PLT_STRMAX);
        break;
    case 1:
        strlcpy(platform.manufacturer, "Egoman", PLT_STRMAX);
        break;
    case 2:
    case 4:
        strlcpy(platform.manufacturer, "Embest", PLT_STRMAX);
        break;
    case 3:
        strlcpy(platform.manufacturer, "Sony Japan", PLT_STRMAX);
        break;
    case 5:
        strlcpy(platform.manufacturer, "Stadium", PLT_STRMAX);
        break;
    default:
        strlcpy(platform.manufacturer, "UNKNOWN", PLT_STRMAX);
        break;
    }
#ifdef DEBUG
    kprintf("Manufacturer %s.\r\n", platform.manufacturer);
#endif                          /* DEBUG */

    /* Detect SoC processor family */
    switch (mask_and_shift(revnumber, RPI_BOARDREV_MASK_PROC))
    {
    case 0:
        strlcpy(platform.family, "BCM2835", PLT_STRMAX);
        break;
    case 1:
        strlcpy(platform.family, "BCM2836", PLT_STRMAX);
        break;
    case 2:
        strlcpy(platform.family, "BCM2837", PLT_STRMAX);
        break;
    case 3:
        strlcpy(platform.family, "BCM2711", PLT_STRMAX);
        break;
    default:
        strlcpy(platform.family, "UNKNOWN", PLT_STRMAX);
        break;
    }
#ifdef DEBUG
    kprintf("Family %s.\r\n", platform.family);
#endif                          /* DEBUG A */

    /* Detect Platform Name/Type */
    switch (mask_and_shift(revnumber, RPI_BOARDREV_MASK_TYPE))
    {
    case 0:
        strlcpy(platform.type, "RasPi A", PLT_STRMAX);
        break;
    case 1:
        strlcpy(platform.type, "RasPi B", PLT_STRMAX);
        break;
    case 2:
        strlcpy(platform.type, "RasPi A+", PLT_STRMAX);
        break;
    case 3:
        strlcpy(platform.type, "RasPi B+", PLT_STRMAX);
        break;
    case 4:
        strlcpy(platform.type, "RasPi 2B", PLT_STRMAX);
        break;
    case 5:
        strlcpy(platform.type, "RasPi Alpha", PLT_STRMAX);
        break;
    case 6:
        strlcpy(platform.type, "RasPi CM1", PLT_STRMAX);
        break;
    case 8:
        strlcpy(platform.type, "RasPi 3B", PLT_STRMAX);
        break;
    case 9:
        strlcpy(platform.type, "RasPi Zero", PLT_STRMAX);
        break;
    case 0xA:
        strlcpy(platform.type, "RasPi CM3", PLT_STRMAX);
        break;
    case 0xC:
        strlcpy(platform.type, "RasPi Zero W", PLT_STRMAX);
        break;
    case 0xD:
        strlcpy(platform.type, "RasPi 3B+", PLT_STRMAX);
        break;
    case 0xE:
        strlcpy(platform.type, "RasPi 3A+", PLT_STRMAX);
        break;
    case 0xF:
        strlcpy(platform.type, "RasPi forbidden", PLT_STRMAX);
        break;
    case 0x10:
        strlcpy(platform.type, "RasPi CM3+", PLT_STRMAX);
        break;
    case 0x11:
        strlcpy(platform.type, "RasPi 4B", PLT_STRMAX);
        break;
    default:
        strlcpy(platform.type, "RasPi UNKNOWN", PLT_STRMAX);
        break;
    }
#ifdef DEBUG
    kprintf("Type %s.\r\n", platform.type);
#endif                          /* DEBUG */
    platform.revision = mask_and_shift(revnumber, RPI_BOARDREV_MASK_REV);
#ifdef DEBUG
    kprintf("Revision %d.\r\n", platform.revision);
#endif                          /* DEBUG */


    /* Detect RAM size */
    switch (revnumber & RPI_BOARDREV_MASK_MEM)
    {
    case RPI_BOARDREV_MEM_256M:
        platform.maxaddr = (void *)0x10000000UL;
        break;

    case RPI_BOARDREV_MEM_512M:
        platform.maxaddr = (void *)0x20000000UL;
        break;

    case RPI_BOARDREV_MEM_1G:
        platform.maxaddr = (void *)0x40000000UL;
        break;

    case RPI_BOARDREV_MEM_2G:
        platform.maxaddr = (void *)0x80000000UL;
        break;

    case RPI_BOARDREV_MEM_4G:
    case RPI_BOARDREV_MEM_8G:
        platform.maxaddr = (void *)0xFFFFFFFFUL;
        kprintf("WARNING: platform.maxaddr out or range!\r\n");
        break;

    default:
        platform.maxaddr = (void *)0x10000000UL;
        kprintf("WARNING: RAM size UNKNOWN!\r\n");
        break;
    }
#ifdef DEBUG
    kprintf("Detected RAM is %d (0x%08X).\r\n",
            platform.maxaddr, platform.maxaddr);
#endif                          /* DEBUG */

}

/**
 * Parse the Linux-style kernel commandline strings passed by the
 * bootloader.  Strings consist of a space-delimited sequence of
 * name/value pairs connected with an '=' symbol.
 * This function uses the delimiters to look through the list,
 * checking for known strings of interest to Xinu platform
 * initialization.
 */
static void parse_kernel_commandline(char *s, int size)
{
    const char *maxbound = s + size;
    char *nextspace = NULL;
    char *start = NULL;
    char buffer[MAX];

    while (((nextspace = strchr(s, ' ')) != s)
           && (nextspace < maxbound) && ((nextspace - s) < (MAX - 1)))
    {
        /* Copy strings one at a time into buffer and null-terminate */
        strlcpy(buffer, s, nextspace - s + 1);
#ifdef DEBUG
        kprintf("kernel string: \"%s\"\r\n", buffer);
#endif                          /* DEBUG */

        /* Check for boardrev string */
        start = strstr(buffer, ".boardrev=");
        if (start)
            parse_boardrev(start);

        /* Check for serial number string */
        start = strstr(buffer, ".serial=");
        if (start)
        {
            platform.serial_low = parse_hex(start);
#ifdef DEBUG
            kprintf("serial #0x%08X\r\n", platform.serial_low);
#endif                          /* DEBUG */
        }

        /* Check for MAC Address string */
        start = strstr(buffer, ".macaddr=");
        if (start)
        {
            strlcpy(platform.macaddr, strchr(start, '=') + 1, PLT_STRMAX);
#ifdef DEBUG
            kprintf("macaddr = %s\r\n", platform.macaddr);
#endif                          /* DEBUG */
        }

        /* Check for UART Clock string */
        start = strstr(buffer, ".uart_clock=");
        if (start)
        {
            platform.uart_clock = parse_decimal(start);
#ifdef DEBUG
            kprintf("uart_clock = %d\r\n", platform.uart_clock);
#endif                          /* DEBUG */
        }

        /* Advance to next string */
        s = nextspace + 1;
    }

    return;
}

/**
 * Checks for and parses ARM-specific "ATAG" format records passed
 * to kernel by bootloader.  More recent Pi models have deprecated
 * the traditional ATAG format in favor of a kernel commandline
 * string (often passed to Linux kernel), which is encapsulated in
 * an ATAG record.
 */
static void parse_atag_list(void)
{
    extern const struct atag *atags_ptr;
    const struct atag *atag;
    ulong maxaddr = 0;

    for (atag = atags_ptr;
         atag->hdr.size > 2 && atag->hdr.tag != ATAG_NONE;
         atag =
         (const struct atag *)((const uint *)atag + atag->hdr.size))
    {
#ifdef DEBUG
        kprintf("ATAG tag 0x%08X; size %d\r\n", atag->hdr.tag,
                atag->hdr.size);
#endif                          /* DEBUG */
        switch (atag->hdr.tag)
        {
        case ATAG_MEM:
#ifdef DEBUG
            kprintf("Mem.start 0x%08X, Mem.size 0x%08X\r\n",
                    atag->mem.start, atag->mem.size);
#endif                          /* DEBUG */
            if (maxaddr == atag->mem.start && atag->mem.size != 0)
            {
                maxaddr += atag->mem.size;
            }
            break;
        case ATAG_SERIAL:
            platform.serial_low = atag->serialnr.low;
            platform.serial_high = atag->serialnr.high;
            break;
        case ATAG_CMDLINE:
#ifdef DEBUG
            kprintf("kernel_commandline: %s\r\n",
                    (char *)(((ulong *)atag) + 8));
#endif                          /* DEBUG */
            parse_kernel_commandline((char *)(((ulong *)atag) + 8),
                                     atag->hdr.size * 4);
            break;
        default:
            break;
        }
    }

    /* Set platform maximum address if calculated value is not insane.  */
    /* RasPi 3 and later actually set this through processor type in
     * kernel commandline args, not through deprecated ATAG_MEM tags.   */
    if ((maxaddr >= (ulong)&_end) && (maxaddr >= (ulong)platform.maxaddr))
    {
        platform.maxaddr = (void *)maxaddr;
    }
}

/**
 * Initializes platform specific information for the Raspberry Pi hardware.
 * @return OK
 */
int platforminit(void)
{
    /* Used only if atags are bad: */
    platform.maxaddr = (void *)PERIPHERALS_BASE;
    platform.clkfreq = 1000000;

    volatile struct pl011_uart_csreg *regptr =
        (struct pl011_uart_csreg *)PL011_REGS_BASE;
    regptr->lcrh |= PL011_LCRH_FEN;

    /* Use platform-provided ATAGs and kernel parameters to autodetect
     * platform vars like processor type and memory size. */
    parse_atag_list();

    return OK;
}
