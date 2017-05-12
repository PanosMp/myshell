# myshell

# supports

simple commands: ls

commands with parameters: ls -l

commands with stdio redirections: ls -l > out.txt

pipes: ls | sort -u | wc -l > count.txt

# compiling

gcc -Wall myshX.c mysh-common.c -o bin/myshX

# execute

./myshX
