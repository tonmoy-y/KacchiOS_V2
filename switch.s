    .global context_switch
context_switch:
    # stack layout (cdecl):
    # [esp+4]  = old_sp pointer (uintptr_t*)
    # [esp+8]  = new_sp (uintptr_t)
    # [esp+12] = new_pc (uintptr_t)

    movl 12(%esp), %edx      # save new_pc in edx BEFORE changing esp
    movl 4(%esp),  %eax      # eax = old_sp pointer
    testl %eax, %eax
    jz   1f
    movl %esp, (%eax)        # *old_sp = current esp
1:
    movl 8(%esp),  %esp      # esp = new_sp
    jmp  *%edx               # jump to new_pc
