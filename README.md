# myshell

# supports

simple commands: ls

commands with parameters: ls -l

commands with stdio redirections: ls -l > out.txt

pipes: ls | sort -u | wc -l > count.txt

# compiling

gcc -Wall mysh<X>.c mysh-common.c -o bin/mysh<X>

# execute

./mysh<X>
