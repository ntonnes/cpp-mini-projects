.data   
buffer:         .space  31                                          # Buffer to store user input (30 characters + null terminator)
buffer2:        .space  31                                          # Buffer to store shifted characters
prompt1:        .asciiz "Input a string 30 characters or less: "
prompt2:        .asciiz "Input an integer greater than 0: "
error1:         .asciiz "No input. Run again.\n"
error2:         .asciiz "Wrong input. Run again.\n"
resultPrompt:   .asciiz "Shifted string ["

.text   
main:           
    # Ask for user input string
    li      $v0,    4
    la      $a0,    prompt1
    syscall 

    # Read user input string
    li      $v0,    8
    la      $a0,    buffer
    li      $a1,    30
    syscall 

    # Check if the input string is empty
    lb      $t0,    buffer
    beq     $t0,    10,             empty_input

    # Ask for user input integer
    li      $v0,    4
    la      $a0,    prompt2
    syscall 

    # Read user input integer
    li      $v0,    5
    syscall 
    move    $t1,    $v0

    # Check if the input integer is greater than 0
    bgtz    $t1,    valid_input

    # Display error message and exit
    li      $v0,    4
    la      $a0,    error2
    syscall 
    j       exit

empty_input:    
    # Display error message and exit
    li      $v0,    4
    la      $a0,    error1
    syscall 
    j       exit

valid_input:    
    # Copy characters to buffer2 and shift
    la      $t2,    buffer2
    la      $t3,    buffer
    add     $t4,    $t3,            $t1
    move    $t5,    $t3

shift_loop:     
    lb      $t6,    0($t4)
    sb      $t6,    0($t2)
    addi    $t2,    $t2,            1
    addi    $t4,    $t4,            1
    bnez    $t6,    shift_loop

    # Copy remaining characters
    move    $t4,    $t5
copy_remaining: 
    lb      $t6,    0($t4)
    sb      $t6,    0($t2)
    addi    $t2,    $t2,            1
    addi    $t4,    $t4,            1
    bnez    $t6,    copy_remaining

    # Display result
    li      $v0,    4
    la      $a0,    resultPrompt
    syscall 

    # Display shifted string
    li      $v0,    4
    la      $a0,    buffer2
    syscall 

    # Display closing square bracket and exit
    li      $v0,    4
    la      $a0,    "]\n"
    syscall 

exit:           
    # Exit the program
    li      $v0,    10
    syscall 