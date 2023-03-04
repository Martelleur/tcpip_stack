# Command Parser Notes

__Step for creating CLI with LIBCLI:__
- main1.c
- main2.c
- main3.c
- main4.c

## Compile, Link  and test 

- Compile:__ ```gcc -g -c main1.c -o main1.o```

- Link: ```gcc -g main1.o -o test_cli.exe -L ./CommandParser/ -lcli```