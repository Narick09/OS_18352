#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_STRINGS_NUMBER = 100;

int parseFile(int fd, int file_size, off_t *strings_arr){
    int count = 0;
    char buff[MAX_STRINGS_NUMBER];
    int size_readed_str = 0;
    int position = 0;

    int indikator_first = 0;
    strings_arr[0] = 0;
    while(1){
        lseek(fd,position, SEEK_SET);
        size_readed_str = read(fd, buff,MAX_STRINGS_NUMBER);
        if(size_readed_str == 0){
            break;
        }

        if(size_readed_str == -1){
            if(errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                perror("Riding Error\n");
                return -1;
            }
        }

        for(int i = 0; i < size_readed_str; i++){
            if(buff[i] == '\n'){
                if(indikator_first == 0){
                    strings_arr[0] = i;
                    indikator_first = 1;
                }
                count++;
                strings_arr[count] = position + i;
            }
        }
        if(position > file_size) {
            break;
        }
        position += size_readed_str;
    }
//    for(int i = count; i > 1; --i){
//        strings_arr[i] = strings_arr[i] - strings_arr[i - 1] - 1;//check it
//    }
//    strings_arr[1] = strings_arr[0];
    return count;
}

void printChoosenString(int fd, off_t *enters, int string_num){
    off_t number_of_bytes = enters[string_num] - enters[string_num - 1] - 1;
    off_t start_pos = enters[string_num - 1] + 1;
    if(string_num == 1){
        number_of_bytes = enters[1];
        start_pos = 0;
    }
    lseek(fd,start_pos, SEEK_SET);
    char buff[MAX_STRINGS_NUMBER];
    int tmp = 1;
    while(number_of_bytes > 0){
        tmp = read(fd, buff, MAX_STRINGS_NUMBER);
        number_of_bytes -= tmp;
        if(number_of_bytes >= 0){
            printf("%s", buff);
        } else {
            for(int i = 0; i < number_of_bytes + tmp; i++){
                printf("%c", buff[i]);
            }
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        perror("Wrong number of program parameters. You should write here only one name of your datafile. \n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        perror("File opening error. Check name of file\n");
        return 1;
    }
    off_t ENTERS[MAX_STRINGS_NUMBER];
    int file_size_on_chars = lseek(fd,0L,SEEK_END);
    lseek(fd,0L, SEEK_SET);
    int file_size_on_strings = parseFile(fd, file_size_on_chars, ENTERS);

//    for(int i = 1; i < file_size_on_strings + 1; i++){
//        printf("%d\n", ENTERS[i]);
//    }

    if(file_size_on_strings == -1)
        exit(-1);
    int string_number = 1;
    while(string_number){
        printf("Please, enter non-negative string number:\n");
        scanf("%d", &string_number);
        if(string_number == 0){
            if(close(fd) == -1){
                perror("Error of closing file");
                return -1;
            }
            break;
        }
        if(string_number < 0){
            printf("You had write negative number. Try again\n");
            continue;
        }
        if(string_number > file_size_on_strings){
            printf("You had write big number. Number of strings of your file is %d. Try again\n", file_size_on_strings);
            continue;
        }
        printChoosenString(fd, ENTERS, string_number);
    }
    exit(0);
}
