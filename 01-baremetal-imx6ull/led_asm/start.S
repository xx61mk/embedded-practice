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

    b main

finished:
    b finished
