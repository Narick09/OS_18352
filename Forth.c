//4. Список строк
//Напишите программу, которая вставляет строки, введенные с клавиатуры, в список. Память под узлы списка выделяйте динамически с использованием malloc(3). Ввод завершается,
//когда в начале строки вводится точка (.). Затем все строки из списка выводятся на экран.
//Подсказка: Объявите массив символов размера, достаточного чтобы вместить самую длинную введенную строку. Используйте fgets(3), чтобы прочитать строку, и strlen(3), чтобы определить ее длину.
//Помните, что strlen(3) не считает нулевой символ, завершающий строку. После определения длины строки, выделите блок памяти нужного размера и внесите новый указатель в список.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFF_SIZE = 200;

typedef struct List {
    char* string;
    struct List *next;
} List;

int add_to_list(List** head, char* str){
    List* tmp = (List*) malloc(sizeof(List));
    if(!tmp){
        perror("No memory allocated. Unable to add to list");
        return 1;
    }
    tmp->string = str;
    tmp->next = NULL;
    (*head)->next = tmp;
    return 0;
}

void print_list(List* head){
    List* tmp = head;
    while(tmp)
    {
        printf("%s\n", tmp->string);
        tmp = tmp->next;
    }
}

void delete_list(List** head){
    List* next;

    while((*head)->next)
    {
        next = (*head)->next;
        free((*head)->string);
        free(*head);
        *head = next;
    }
    free((*head)->string);
    free(*head);
}

int main() {
    char tmpStr[BUFF_SIZE];

    List* first = (List*) malloc(sizeof(List));
    if(!first){
        perror("Memrore allocated");
        return -1;
    }
    if(!fgets(tmpStr, BUFF_SIZE, stdin)){
        perror("Reading error!");
        return -1;
    }
    char* first_string = strndup(tmpStr, strlen(tmpStr) - 1);
    if(!first_string)
    {
        perror("Memrore allocated");
        return -1;
    }

    first->next = NULL;
    first->string = first_string;

    List** tmp = &first;
    while(1){
        if(!fgets(tmpStr, BUFF_SIZE, stdin)){
            perror("Reading error!");
            tmp = NULL;
            break;
        }
        char* string_to_add = strndup(tmpStr, strlen(tmpStr) - 1);
        if(!string_to_add)
        {
            perror("Memrore allocated");
            tmp = NULL;
            break;
        }

       if(!add_to_list(tmp, string_to_add)){
           tmp = &(*tmp)->next;
       } else {
           tmp = NULL;
           break;
       }
        if(string_to_add[0] == '.'){
            break;
        }
    }

    if(tmp){
        print_list(first);
    }
    if(first!= NULL)
        delete_list(&first);

    exit(0);
}
