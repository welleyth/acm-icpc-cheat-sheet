gcc -g -O0 -pg -fsanitize=address,undefined -fstack-protector-strong -finstrument-functions -o your_program your_program.c

