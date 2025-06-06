ENTRY(_start)

PHDRS
{
  text PT_LOAD FLAGS(5); /* R_X */
  data PT_LOAD FLAGS(6); /* R_W */
}

SECTIONS
{
  . = 0x80000;
  .text : { *(.text*) } :text
  .data : { *(.data*) } :data
  .bss  : { *(.bss*)  } :data
}