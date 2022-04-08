/**
 * @file dispatch.c
 *
 * This file contains code that interfaces with the interrupt controller of the
 * BCM2835 (and later) SoC used in the Raspberry Pi.
 *
 * This "interrupt controller" is, more specifically, the interrupt controller
 * used by the ARM processor on the BCM2835.  That is, this interrupt controller
 * controls which IRQs the ARM receives.  The VideoCore co-processor likely has
 * its own (undocumented) way to control IRQs to itself.
 *
 * See http://xinu-os.org/BCM2835_Interrupt_Controller for additional
 * information.
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include <xinu.h>

/** Layout of the BCM2835 interrupt controller's registers. */
struct bcm2835_interrupt_registers
{
    uint IRQ_basic_pending;
    uint IRQ_pending_1;
    uint IRQ_pending_2;
    uint FIQ_control;
    uint Enable_IRQs_1;
    uint Enable_IRQs_2;
    uint Enable_Basic_IRQs;
    uint Disable_IRQs_1;
    uint Disable_IRQs_2;
    uint Disable_Basic_IRQs;
};

static volatile struct bcm2835_interrupt_registers *const irq_regs =
    (volatile struct bcm2835_interrupt_registers *)INTERRUPT_REGS_BASE;

/* Number of IRQs shared between the GPU and ARM. These correspond to the IRQs
 * that show up in the IRQ_pending_1 and IRQ_pending_2 registers.  */
#define BCM2835_NUM_GPU_SHARED_IRQS     64

/* Number of ARM-specific IRQs. These correspond to IRQs that show up in the
 * first 8 bits of IRQ_basic_pending.  */
#define BCM2835_NUM_ARM_SPECIFIC_IRQS   8

/* Total number of IRQs on this hardware.  */
#define BCM2835_NUM_IRQS (BCM2835_NUM_GPU_SHARED_IRQS + BCM2835_NUM_ARM_SPECIFIC_IRQS)

/** Table of Xinu's interrupt handler functions.  This is an array mapping IRQ
 * numbers to handler functions.  */
interrupt_handler_t interruptVector[BCM2835_NUM_IRQS];

/** Three Bitwise tables of IRQs that have been enabled on the ARM. */
#define ARM_ENABLED_TABLE_MAX 3
static uint arm_enabled_irqs[ARM_ENABLED_TABLE_MAX];

/* Check if the pending bit for an IRQ line is set; 
 * if so, call the corresponding handler function.
 */
static void handle_irq(uchar irq_num)
{
    bool handle = FALSE;        //RMv7 reference manual in section B1.8.1 
    interrupt_handler_t handler = NULL;

    /* Check the appropriate hardware register, depending on the IRQ number.  */
    /* Xinu assigns IRQs 64-95 to IRQ_basic_pending register. */
    if (irq_num >= 64)
    {
        if (irq_regs->IRQ_basic_pending & (1 << (irq_num - 64)))
        {
            handle = TRUE;
        }
    }
    /* IRQs 32-63 are in IRQ_pending_2 register. */
    else if (irq_num >= 32)
    {
        if (irq_regs->IRQ_pending_2 & (1 << (irq_num - 32)))
        {
            handle = TRUE;
        }
    }
    /* IRQs 0-31 are in IRQ_pending_1 register. */
    else
    {
        if (irq_regs->IRQ_pending_1 & (1 << irq_num))
        {
            handle = TRUE;
        }
    }
    /* Hardware does not report IRQ pending for requested irq_num */
    if (!handle)
        return;

    /* The pending bit should have been cleared in a device-specific way by
     * the handler function.  As far as we can tell, it cannot be cleared
     * directly through the interrupt controller.  */
    handler = interruptVector[irq_num];

    /* If there is no Xinu handler registered for this IRQ */
    if (!handler)
    {
        kprintf("ERROR: No handler registered for interrupt %u\r\n",
                irq_num);

        extern void halt(void);
        halt();
    }

    /* Call the correct interrupt handler function */
    (*handler) ();
}

/**
 * Processes all pending interrupt requests.
 *
 * On the BCM2837 (Raspberry Pi3), this done by iterating through all registered
 * interrupts on the ARM and checking whether each one is pending.  This is not
 * necessarily the fastest way to do it, but this should minimize problems with
 * the poorly-documented hardware and conflicts with the GPU.
 */
void dispatch(void)
{
    uint table, bit, mask;

    for (table = 0; table < ARM_ENABLED_TABLE_MAX; table++)
    {
        mask = arm_enabled_irqs[table];
        while (mask != 0)
        {
            /* Count Leading Zeros to find leftmost masked bit */
            bit = 31 - __builtin_clz(mask);
            /* XOR off the leftmost masked bit */
            mask ^= (1 << bit);
            /* Check and handle if that masked on bit is pending */
            handle_irq(bit + (table << 5));
        }
    }

}

/**
 * Enable an interrupt request line.
 * This function decodes 96 possible Xinu IRQs across three 32-bit
 * ARM IRQ enable registers.
 * @param irq_num
 *      index of the interrupt to enable, which must be valid on the current
 *      platform.
 */
void enable_irq(irqmask irq_num)
{
    if (irq_num < 32)
    {
        irq_regs->Enable_IRQs_1 = 1 << irq_num;
        arm_enabled_irqs[0] |= 1 << irq_num;
    }
    else if (irq_num < 64)
    {
        irq_regs->Enable_IRQs_2 = 1 << (irq_num - 32);
        arm_enabled_irqs[1] |= 1 << (irq_num - 32);
    }
    else
    {
        irq_regs->Enable_Basic_IRQs = 1 << (irq_num - 64);
        arm_enabled_irqs[2] |= 1 << (irq_num - 64);
    }
}

/**
 * Disable an interrupt request line.
 * This function decodes 96 possible Xinu IRQs across three 32-bit
 * ARM IRQ disable registers.
 * @param irq_num
 *      index of the interrupt to disable, which must be valid on the current
 *      platform.
 */
void disable_irq(irqmask irq_num)
{
    if (irq_num < 32)
    {
        irq_regs->Disable_IRQs_1 = 1 << irq_num;
        arm_enabled_irqs[0] &= ~(1 << irq_num);
    }
    else if (irq_num < 64)
    {
        irq_regs->Disable_IRQs_2 = 1 << (irq_num - 32);
        arm_enabled_irqs[1] &= ~(1 << (irq_num - 32));
    }
    else
    {
        irq_regs->Disable_Basic_IRQs = 1 << (irq_num - 64);
        arm_enabled_irqs[2] &= ~(1 << (irq_num - 64));
    }
}
