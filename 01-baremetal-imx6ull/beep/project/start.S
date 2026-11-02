    .global _start

    _start:
    ldr pc, =_reset_handler
	ldr pc, =_undefine_handler
	ldr pc, =_software_handler
	ldr pc, =_prefetch_abort_handler
	ldr pc, =_data_abort_handler
	ldr pc, =_reserved_handler
	ldr pc, =_irq_handler
	ldr pc, =_fiq_handler

_fiq_handler:
	ldr pc, =_fiq_handler

_irq_handler:
	ldr pc, =_irq_handler

_reserved_handler:
	ldr pc, =_reserved_handler

_data_abort_handler:
	ldr pc, =_data_abort_handler

_prefetch_abort_handler:
	ldr pc, =_prefetch_abort_handler
  
_software_handler:
    ldr pc, =_software_handler

_undefine_handler:
	ldr pc, =_undefine_handler

_reset_handler:
    // 设置irq模式
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x12  
    msr cpsr, r0
    // 设置irq模式下的栈(32M)
    ldr sp, =0x86000000

    // 设置system模式
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x1F
    msr cpsr, r0
    // 设置system模式下的栈
    ldr sp, =0x84000000
    // 清空bss段
    bl _bss_clear

    b main

_bss_clear:
    ldr r0, =__bss_start
    ldr r1, =__bss_end

loop:
    mov r2, #0
    str r2, [r0]
    add r0, r0, #4
    cmp r0, r1
    blt loop
    bx lr

finished:
    b finished