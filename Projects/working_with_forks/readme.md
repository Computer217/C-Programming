# checkof

The program checkof prints valid file desciptors form 0 to 20 to stderr, and
then copies a file, if specified on the command line, or stdin to stdout until
EOF is seen at stdin.

This program is also a demo fo redircting stdin for a process.

```
./checkof 
./checkof  checkof.c
```

# runseq

```
./runseq echo 'Hello, world!' -- echo '==before ls' -- ls Makefile -- echo '==after ls'

# in shell
{echo 'Hello, world!';  echo '==before ls'; ls Makefile;  echo '==after ls'; } >shOutput

# compare the results
diff seqOutput shOutput
```

```
./runseq seq 1 20 -- ./checkof Makefile -- sort Makefile 

# in shell
{ seq 1 20; ./checkof Makefile ls Makefile; sort Makefile; } >shOutput

# compare the results
diff seqOutput shOutput
```

# runpipeline

Some commands for testing runpipeline are listed below.  

## Compare output

```
./runpipeline seq 1 20 -- cat -- ./checkof -- sort -r >pipeOutput

# in shell
seq 1 20 | cat | ./checkof | sort -r >shOutput
 
# compare the results
diff pipeOutput shOutput
```

```
./runpipeline cat whitman.txt -- tr -s [:space:] '\n' -- tr -d [:punct:] -- tr A-Z a-z -- \sort -- uniq -c -- sort -nr > counts.txt

# in shell
cat whitman.txt | tr -s [:space:] '\n' | tr -d [:punct:] | tr A-Z a-z | \sort | uniq -c | sort -nr > counts-sh.txt
 
# compare the results
diff counts.txt counts-sh.txt
```

# check file descriptors

Each process should report only 0, 1, and 2 are valid.
Also, the file descriptors of runpipeline should be checked with lsof command
(see the lsof section below).

```
./runpipeline ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof -- ./checkof 

# in shell
./checkof | ./checkof | ./checkof | ./checkof | ./checkof | ./checkof | ./checkof | ./checkof | ./checkof | ./checkof 
```

# lsof

lsof is a tool that lists the open file discriptors. Some examples are shown
below. See the manual page for details.

```
# list open files by PIDs
$lsof -p 3753,3754,3755,3756,3757
$lsof -p 3753

# list open files for processes whose name starts with runp, cat, wc, or tr
lsof -c runp -c cat -c wc -c tr

# use -u option to specify a user name. -a indicates combine conditions with 'and'
lsof -c runp -u netid -a

# Use awk to filter out less interesting rows
lsof -c runp -c cat | awk '$4 ~ /^[0-9]/ { print }'
```

The rows that are interesting in this problem are the ones that have a number in
the FD column. Most of the commands have only three open files 0, 1, and 2.
Some commands, for example, tee, may have additional open files.

