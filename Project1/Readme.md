a)Create a .c file named simple-solution. Write source code with linked-list structure and macro function.
b)Use make command to compile the C program, then a file simple-solution.ko is created and represents the compiled kernel module with linked-list structure.
c)Use sudo dmesg -c to clear the messages. 
d)Use command: sudo inmod simple.ko to install the kernel. Use command: dmesg to check whether the messages are printed.
e)Use command: sudo rmmod simple to remove the kernel and use dmesg to check the message again.
