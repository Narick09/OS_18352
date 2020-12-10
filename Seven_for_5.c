//7.         Таблица поиска строк в текстовом файле 2.
//Измените предыдущую программу так, чтобы использовалось отображение файла в память взамен использования read(2), lseek(2) и write(2).

#include<sys/types.h>
#include<sys/mman.h> //mmap
#include<fcntl.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_STRINGS_NUMBER = 100;

size_t parseFile(char* file, size_t file_size, size_t *strings_arr){
    size_t count = 0;
    strings_arr[0] = 0;
    for(size_t i = 0; i < file_size; i++){
        if(file[i] == '\n'){
            count++;
            strings_arr[count] = i;
        }
    }
    return count;
}

void printChoosenString(char* file, size_t *enters, int string_num){
    size_t number_of_bytes = enters[string_num] - enters[string_num - 1] - 1;
    size_t start_pos = enters[string_num - 1] + 1;
    if(string_num == 1){
        number_of_bytes = enters[1];
        start_pos = 0;
    }
    for(int i = start_pos; i < start_pos + number_of_bytes; i++){
        printf("%c", file[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        perror("Usage: /home/daniil/nsu/study/OS/numberFive.sh -c\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        perror("Usage: /home/daniil/nsu/study/OS/numberFive.sh -c\n");
        return 1;
    }
    size_t ENTERS[MAX_STRINGS_NUMBER];
    size_t file_size_on_chars = lseek(fd,0L,SEEK_END);
    lseek(fd,0L, SEEK_SET);
    char* file = mmap(0, file_size_on_chars, PROT_READ, MAP_SHARED, fd, 0);

    size_t file_size_on_strings = parseFile(file, file_size_on_chars, ENTERS);
    
    int string_number = 1;
    while(string_number){
        printf("Please, enter non-negative string number:\n");
        scanf("%d", &string_number);
        if(string_number == 0){
            break;
        }
        if(string_number < 0){
            printf("Usage: write non-negative number\n");
            continue;
        }
        if(string_number > file_size_on_strings){
            printf("Usage: write number less that %d\n", file_size_on_strings);
            continue;
        }
        printChoosenString(file, ENTERS, string_number);
    }
    if(munmap(file, file_size_on_chars) == -1){
        perror("Error: error with allocated memory.");
        exit(-1);
    }
    if(close(fd) == -1){
        perror("Error: file closed with error.");
        exit(-1);
    }
    exit(0);
}
