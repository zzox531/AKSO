; Author : Ignacy Kozakiewicz
; Grade : 9.2 / 10 
section .data
    rbuffer times 4096 db 0     ; Buffer of size 4KB used to read file
    wbuffer times 4098 db 0     ; Buffer of size 4098B used to write
    SYS_WRITE equ 1
    SYS_OPEN equ 2
    SYS_CLOSE equ 3
    SYS_EXIT equ 60
    ; r8  - Loop counter iterating through the buffer
    ; r9  - Computed byte of read buffer
    ; r10 - Descriptor byte
    ; r12 - In file descriptor
    ; r13 - Out file descriptor
    ; r14 - Write buffer counter
    ; r15 - Counter of bytes != S or s
    ; rsp - Stack pointer
section .text
global _start
_start: 
    mov rdi, [rsp]  ; Put into rdi the number of given parameters
    cmp rdi, 3      ; If !=3, exit with code 1
    jne exit
    mov rdi, [rsp + 16]     ; Put into rdi the name of in_file

    ; Open in_file
    mov rax, SYS_OPEN       ; Syscall no. 2 (sys_open)
    xor rsi, rsi    ; Flags (read_only)
    syscall

    ; Check if file was opened properly
    cmp rax, 0
    jl exit     ; If returned value is < 0, exit with code 1
    mov r12, rax    ; move descriptor value of in_file
    
    mov rdi, [rsp + 24]     ; Put into rdi the name of out_file
    ; Create out_file
    mov rax, SYS_OPEN       ; sys_open 
    mov rsi, 0xc1   ; O_WRONLY | O_CREAT | O_EXCL - flags necesary to create the file
    mov rdx, 0644o  ; -rw-r--r-- 
    syscall

    cmp rax, 0      ; If returned value is < 0, exit with code 1
    jl close_in
    
    mov r13, rax    ; Move descriptor value of in_file
    xor r15, r15    ; Byte counter = 0
read_loop:
    xor rax, rax            ; syscall no. 0 (sys_read)
    mov rdi, r12            ; Pointer to file descriptor
    lea rsi, [rel rbuffer]  ; Pointer to buffer
    mov rdx, 4096           ; Buffer size - 4096 B = 4KB
    syscall

    cmp rax, 0              
    je end_loop             ; If 0 bytes read, finish reading
    jl close_out            ; If sys_read returned with an error - exit code 1

    mov r8, rax             ; Number of bytes to compute
    lea r10, [rel rbuffer]  ; Move pointer of the buffer to r10
process_loop:
    mov r9b, byte [r10]     ; Move byte from buffer
    cmp r9b, 83             ; If equal to 83 (s)
    je reset
    cmp r9b, 115            ; Or 115 (S)
    je reset                ; Jump to the label resetting the sequence
    jmp inc
reset:
    cmp r15, 0                      ; If sequence has length == 0
    je no_add_seq                   ; Don't add it to the buffer
    mov [wbuffer + r14], r15w   ; Else, add it to the buffer
    add r14, 2
    xor r15, r15    ; Set sequence length to 0 
    cmp r14, 4096   ; If buffer isn't filled, jump to label               
    jl no_add_seq   ; Adding letter S to the buffer 

    mov rax, SYS_WRITE  ; else - Write buffer to the file
    mov rdi, r13        
    mov rsi, wbuffer
    mov rdx, r14
    syscall

    cmp rax, 0          ; If SYS_WRITE was not successful
    jl close_out        ; Close files

    cmp rax, r14        ; If SYS_WRITE wrote less than r14 bytes
    jne close_out       ; Close files
    xor r14, r14        ; Set counter of write_buffer to 0
no_add_seq:
    mov [wbuffer + r14], r9b    ; Add s or S to the buffer
    inc r14             ; Increment buffer counter
    cmp r14, 4096       ; If 4096B was not exceeded
    jl no_inc           ; Skip SYS_WRITE
    mov rax, SYS_WRITE  
    mov rdi, r13
    mov rsi, wbuffer
    mov rdx, r14
    syscall

    cmp rax, 0          ; If SYS_WRITE was not successful
    jl close_out        ; Close files

    cmp rax, r14        ; If SYS_WRITE wrote less than r14 bytes
    jne close_out       ; Close files
    xor r14, r14        ; Set counter of write_buffer to 0
    jmp no_inc          
inc:
    inc r15             ; If current byte is neither s nor S, increment r15
no_inc:
    inc r10             ; Incremenet buffer address
    dec r8              ; Decrement bytes left to compute
    jnz process_loop    ; If r8 != 0, compute next bytes of the buffer
    jmp read_loop       ; If r8 = 0, read another fragment of in_file
end_loop:
    cmp r15, 0      ; If sequence length is >0, add the sequence to the buffer
    jle last_add    ; Else - jump to label writing the last buffer
    mov [wbuffer + r14], r15w   ; Write the sequence to the buffer
    add r14, 2      
last_add:
    cmp r14, 0      ; If r14 jest = 0, close files
    je close_out
    mov rax, SYS_WRITE  ; Else - write buffer to out_file
    mov rdi, r13
    mov rsi, wbuffer
    mov rdx, r14
    syscall

    cmp rax, 0      ; If SYS_WRITE was not successful
    jl close_out    ; Close files

    cmp rax, r14    ; If less than r14 bytes were written
    jne close_out   ; Close files
    xor rax, rax    ; Before closing files, set rax to 0
close_out:
    xor r14, r14    
    or r14, rax     
    mov rax, SYS_CLOSE  ; Close out
    mov rdi, r13        ; exit code 1
    syscall
close_in:
    or r14, rax
    mov rax, SYS_CLOSE  ; Close in_file
    mov rdi, r12
    syscall
exit:
    or r14, rax
    xor rdi, rdi
    cmp r14, 0      ; If any syscall returned value != 0
    je no_err       
    mov rdi, 1      ; Set exit_code to 0
no_err:
    mov rax, SYS_EXIT   ; sys_exit
    syscall