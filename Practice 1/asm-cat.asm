                section         .text
                global          _start
        
        
_start:
                sub             rsp, buf_size ;local var
                mov             rsi, rsp
read_again:
                xor             eax, eax
                xor             rdi, rdi
                mov             rdx, buf_size
                syscall
                
                test            rax, rax
                jz              quit
                js              read_error
                
                mov             rdx, rax
                mov             rax, 1
                mov             rdi, 1
                syscall
                jmp             read_again
            
quit:
                mov             rax, 60
                xor             rdi, rdi
                syscall
                
read_error:
                mov             eax, 1
                mov             edi, 1
                mov             rsi, read_error_msg
                mov             rdx, read_error_len
                syscall
                
                mov             rax, 60
                mov             edi, 1
                syscall

            
                section .rodata
buf_size:       equ             8192
read_error_msg: db              "Read failure", 0x0a
read_error_len: equ             $ - read_error_msg

;                section .bss
;buf_size:       equ             8192
;buf:            resb            buf_size

;nasm -f elf64 asm-cat.asm -o 1.o && ld 1.o -o asm-cat
;strace ./asm-cat.asm < kek.txt 2> syscalls.log
