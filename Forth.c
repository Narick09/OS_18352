//4. Список строк
//Напишите программу, которая вставляет строки, введенные с клавиатуры, в список. Память под узлы списка выделяйте динамически с использованием malloc(3). Ввод завершается,
//когда в начале строки вводится точка (.). Затем все строки из списка выводятся на экран.
//Подсказка: Объявите массив символов размера, достаточного чтобы вместить самую длинную введенную строку. Используйте fgets(3), чтобы прочитать строку, и strlen(3), чтобы определить ее длину.
//Помните, что strlen(3) не считает нулевой символ, завершающий строку. После определения длины строки, выделите блок памяти нужного размера и внесите новый указатель в список.

//#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFF_SIZE = 200;
int tmp_counter = 0;

typedef struct List {
	char* string;
	// size_t stringSize;
	struct List *next;
} List;

int add_to_list(List** head, char* str) {
	List* tmp = (List*)malloc(sizeof(List));
	if (!tmp) {
		perror("No memory allocated. Unable to add to list");
		return 1;
	}
	tmp->string = str;
	// tmp->stringSize = strlen(str);
	tmp->next = NULL;
	(*head)->next = tmp;
	return 0;
}

void print_list(List* head) {
	List* tmp = head;
	while (tmp)
	{
		printf("%s", tmp->string);
		tmp = tmp->next;
	}
}

//void delete_list(List** head){
// List* next;
//
// while(*head)//->next) //change
// {
// tmp_counter++;
// next = (*head)->next;
// free((*head)->string);
// free(*head);
// *head = next;
// }
//}

void delete_list(List** head) {
	List* next;

	while ((*head)->next) //change
	{
		tmp_counter++;
		next = (*head)->next;
		free((*head)->string);
		free(*head);
		*head = next;
	}
	free((*head)->string);
	free(*head);
}
//void delete_list2(List* head){
// List* next;
//
// while(head)
// {
// tmp_counter++;
// next = head->next;
// free(head->string);
// free(head);
// head = next;
// }
//}

int main() {
	char tmpStr[BUFF_SIZE];

	char* first_string = fgets(tmpStr, BUFF_SIZE, stdin);
	if (!first_string) {
		perror("Reading error!");
		return -1;
	}
	char* string_to_add = strdup(first_string);
	if (!string_to_add)
	{
		perror("Memrore allocated");
		return -1;
	}
	//free(first_string);

	List* first = (List*)malloc(sizeof(List));
	first->next = NULL;
	first->string = string_to_add;
	//first->string = tmpStr;

	List** tmp = &first;
	while (1) {
		char* tmp_string = fgets(tmpStr, BUFF_SIZE, stdin);
		if (!tmp_string) {
			perror("Reading error!");
			tmp = NULL;
			break;
		}

		char* string_to_add2 = strdup(tmp_string);
		if (!string_to_add2)
		{
			perror("Memrore allocated");
			tmp = NULL;
			break;
		}

		if (!add_to_list(tmp, string_to_add2)) {
			tmp = &(*tmp)->next;
			// printf("\n");
			// free(tmp_string);
		}
		else {
			tmp = NULL;
			// free(tmp_string);
			break;
		}
		if (string_to_add2[0] == '.') {
			break;
		}
	}
	if (tmp) {
		print_list(first);
	}
	//printf("%d\n", tmp_counter);
	if (first != NULL)
		delete_list(&first);
	printf("%s", first_string);
	//printf("%d\n", tmp_counter);
	exit(0);
}
