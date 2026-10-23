#include "memory.h"

#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n", name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    printf("%-10s %-10s %-10s %s\n", "area", "start", "end", "size");
    row("flash", (uintptr_t)XIP_BASE, (uintptr_t)(XIP_BASE + PICO_FLASH_SIZE_BYTES));
    row("sram", (uintptr_t)SRAM_BASE, (uintptr_t)SRAM_END);
    row("rom", (uintptr_t)ROM_BASE, (uintptr_t)(ROM_BASE + 16 * 1024));
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, (uintptr_t)(XIP_BASE + PICO_FLASH_SIZE_BYTES));
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);
    row("data flash", (uintptr_t)&__etext, (uintptr_t)&__etext + (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__);
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    unsigned boot2_size = (unsigned)((uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__);
    unsigned text_size = (unsigned)((uintptr_t)&__etext - (uintptr_t)&__boot2_end__);
    unsigned data_size = (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__);
    unsigned bss_size = (unsigned)((uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__);

    unsigned flash_image = (unsigned)((uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start);
    unsigned flash_free = (unsigned)((uintptr_t)(XIP_BASE + PICO_FLASH_SIZE_BYTES) - (uintptr_t)&__flash_binary_end);
    unsigned ram_used = data_size + bss_size;
    unsigned ram_heap = (unsigned)((uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__);
    unsigned ram_stack = (unsigned)((uintptr_t)&__StackTop - (uintptr_t)&__StackBottom);

    printf("\ntotal\n");
    printf("  %-11s %8u = boot2 %u + text %u + data %u\n", "flash image", flash_image, boot2_size, text_size, data_size);
    printf("  %-11s %8u of %u\n", "flash free", flash_free, (unsigned)PICO_FLASH_SIZE_BYTES);
    printf("  %-11s %8u = data %u + bss %u\n", "ram used", ram_used, data_size, bss_size);
    printf("  %-11s %8u for heap and %u for stack\n", "ram free", ram_heap, ram_stack);
}