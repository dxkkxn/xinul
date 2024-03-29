OUTPUT_ARCH( "riscv" )

ENTRY( _start )

/*
 * Virt machine configuration:
 * The virt machine goes through the boot loader initially and 
 * then jump to the address 0x80000000 for this reason we place the kernel in that 
 * address.
 * The size of the memory that will take is 128M
 * We can take more memory space but that will not be needed in thus particular os
*/
MEMORY
{
  ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}

PHDRS
{
  text PT_LOAD;
  data PT_LOAD;
  bss PT_LOAD;
  apps PT_LOAD;
}

SECTIONS
{
  .text : {
    PROVIDE(_text_start = .);
    *(.text.init) *(.text .text.*)
    PROVIDE(_text_end = .);
  } >ram AT>ram :text

  .rodata : {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    /*
     * Include the linker script generated by the toolchain which will
     * define where userspace apps will be linked. The file is located in
     * $(OUTPUT)/kernel/ but the -L flag of ld allows us to not depends on this path
     * here, and let the Makefile manage that.
     */
    INCLUDE apps.lds
    PROVIDE(_rodata_end = .);
  } >ram AT>ram :text

  .data : {
    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  } >ram AT>ram :data

  .bss :{
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    *(scommon COMMON)
    PROVIDE(_bss_end = .);

    . = ALIGN(4096);
    PROVIDE(_bss_end_align = .);
    PROVIDE(_heap_start = _bss_end_align + 1M);
    PROVIDE(_heap_end = _heap_start + 32M);
    PROVIDE(_free_memory_start = _heap_end);
  } >ram AT>ram :bss

  PROVIDE(_memory_start = ORIGIN(ram));
  PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
}
