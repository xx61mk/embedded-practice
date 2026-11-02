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
    sub lr, lr, #4

    // GIC 基地址
    mcr p15, 4, r1, c15, c0, 0
    add r1, r1, #0x2000
    ldr r0, [r1, #0x0C]    // interrupt num
    
    stmfd sp!, {r0-r12, lr}
    stmfd sp!, {r0, r1}
	bl system_interrupt_handler
    ldmfd sp!, {r0, r1}
    // 清除GIC中断标记位
    str r0, [r1, #0x10]             
    ldmfd sp!, {r0-r12, pc}^

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
    bic r0, r0, #(1 << 7)   // irq
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
    // 打开icache + 异常向量表重映射
    bl _enable_icache
    // 重映射异常向量表基地址
    bl _set_vbar

    b main

_enable_icache:
    mrc p15, 0, r0, c1, c0, 0   
    bic r0, r0, #(1 << 13)
    orr r0, r0, #(1 << 12)
    mcr p15, 0, r0, c1, c0, 0
    bx lr

_set_vbar:
    ldr r0, =0x87800000
    mcr p15, 0, r0, c12, c0, 0
    bx lr

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
