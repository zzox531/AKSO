; Author - Ignacy Kozakiewicz
; Grade - 8.8/10
global sum

; r10 - register containing the index of the array, currently beeing computed
; r11 - register containing information about the index i, which will be updated
; r9 - register containing the value of the element of the array on index r10
; r8 - number of 'carry' added to index r11 + 2 after executing all calculations

section .text  
sum:
    cmp rsi, 1 ; if n = 1, return first element of the array 
    je finish
    xor r11, r11 
    mov r10, 1
    mov r9, [rdi + 8 * r10] ; put value of x[1] into r9
    cmp qword [rdi], 0
    jge greater_eq
    mov qword [rdi + 8 * r10], -1 ; if x[0] < 0, set x[1] to -1 
    mov r8, -1
    jmp iterate_over_elements
greater_eq: ; if x[0] >= 0, set x[1] to 0
    mov qword [rdi + 8 * r10], 0
    xor r8, r8
iterate_over_elements: ; main loop iterating over the elements of array x
    mov rax, r10
    mul r10
    shl rax, 6
    div rsi 
    mov rcx, 64 ; rax holds the index of the array affected by x[r10]
    xor rdx, rdx
    div rcx ; rdx contains the remainder
    cmp rax, r11
    je skip_fill
fill: ; label adding carry to certain indexes of the array
    inc r11 ; instructions are executed when rax is changed, untill r11 < rax
    add qword [rdi + 8 * r11 + 8], r8 ; add carry to the index
    xor r8, r8
    cmp qword [rdi + 8 * r11 + 8], 0
    jge no_fill_dec ; if result greater than 0 -> carry = 0, else carry = 1
    dec r8
no_fill_dec:
    cmp r11, rax
    jl fill
skip_fill: ; label adding x[r10] on range x[r11] and x[r11+1]
    test r9, r9 ; if array element <0 , lower carry count in r8
    jns no_add_dec
    dec r8
no_add_dec:
    mov rax, r9
    mov rcx, rdx
    cmp cl, 64 ; in this architecture, "shl rax, n" asm makes shl by n%64
    jne perform_shl ; edgecase n=64 taken care of
    xor rax, rax ; for this case, rax = 0
perform_shl:
    shl rax, cl
    add qword [rdi + 8 * r11], rax
    adc qword [rdi + 8 * r11 + 8], 0
    adc r8, 0 ; increase r8 by the carry value on x[r11+2]
    neg rcx
    add rcx, 64 ; operations in two upper lines set rcx to (64 - rdx)
    cmp cl, 64
    jne perform_sar ; analogical case as "shl rax, 64"
    sar r9, 32 ; n = 64 -> 2x sar rax, 32, else -> sar rax, cl
    sar r9, 32
perform_sar:
    sar r9, cl
    add qword [rdi + 8 * r11 + 8], r9
    adc r8, 0 ; similarly, increase r8 by carry value on x[r11+2]
    inc r10
    cmp r10, rsi
    je finish ; if r10 = rsi -> finish program execution
    mov r9, [rdi + 8 * r10]
    mov qword [rdi + 8 * r10], 0
    jmp iterate_over_elements
finish: 
    ret