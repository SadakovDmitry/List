#include "lib.h"
//#include <Dotter.h>

void List_Ctor(struct List* list);
void List_Realloc(struct List* list);
int Delete(struct List* list, int position);
void Do_Graph(struct List* list);
void List_Dump(struct List* list);
int Find_Reall_Id(struct List* list, int pos);
int List_Insert_After(struct List* list, int val, int position);
void Put_Elem_to_position(struct List* list, int position);
void Put_Elem_to_position(struct List* list, int position);
void Resize_List(struct List* list, int position);
int Get_Next(struct List* list, int pos);
int Get_Prev(struct List* list, int pos);

#define GET_PREV(pos) Get_Prev(list, pos)
#define GET_NEXT(pos) Get_Next(list, pos)

const int poizon = -1111;

struct List
{
    int* data;
    int* next;
    int* prev;
    int head;
    int tail;
    int free;
    int n_elem;
    int capacity;
    int errors;
};

enum List_err : unsigned int
{
    CAPACITY_ZERO                   = 1 << 0,
    NUM_ELEM_BIGGER_THEN_CAPACITY   = 1 << 1,
    START_ELEMENT_WAS_CHANGED       = 1 << 2,
    POSITION_OUT_OF_DATA            = 1 << 3
};

void List_Ctor(struct List* list)
{
    assert(list);

    list -> capacity = 2;
    list -> free = 1;
    list -> head = 0;
    list -> tail = 0;
    list -> n_elem = 0;

    list -> data = (int*) calloc(list -> capacity + 1, sizeof(int));
    list -> next = (int*) calloc(list -> capacity + 1, sizeof(int));
    list -> prev = (int*) calloc(list -> capacity + 1, sizeof(int));

    list -> data[0] = poizon;
    (list -> next)[0] = 1;
    (list -> next)[0] = 0;
    (list -> prev)[1] = -1;

    for(int i = 1; i < list -> capacity; i++)
    {
        list -> next[i] = i + 1;
        list -> prev[i] = -1;
    }
}

void List_Dtor(struct List* list)
{
    assert(list);
    assert(list -> data);

    if (list -> data == NULL)
    {
        free(list -> data);
    }
}

void List_Verify(struct List* list)
{
    if (list -> capacity <= 0)
    {
        printf("\n\033[31mERROR\033[0m: CAPACITY ZERO !!!");
        exit(1);
    }
    if (list -> capacity < list -> n_elem)
    {
        printf("\n\033[31mERROR\033[0m: NUM ELEM BIGGER THEN CAPACITY !!!");
        exit(1);
    }
    if (list -> data[0] != -1111)
    {
        printf("\n\033[31mERROR\033[0m: START ELEMENT WAS CHANGED");
        exit(1);
    }
    if (list -> errors & POSITION_OUT_OF_DATA)
    {
        printf("\n\033[31mERROR\033[0m: POSITION OUT OF DATA");
        exit(1);
    }
}

void List_Realloc_Up(struct List* list)
{
    assert(list);
    assert(list -> data);

    list -> data = (int*) realloc(list -> data, (list -> capacity) * 2 * sizeof(int));
    list -> next = (int*) realloc(list -> next, (list -> capacity) * 2 * sizeof(int));
    list -> prev = (int*) realloc(list -> prev, (list -> capacity) * 2 * sizeof(int));

    for(int i = list -> capacity; i < list -> capacity * 2; i++)
    {
        list -> data[i] = 0;
        list -> next[i] = i + 1;
        list -> prev[i] = -1;
    }
    //list -> next[list -> capacity * 2] = 0;

    list -> capacity = list -> capacity * 2;
}

void List_Realloc_Down(struct List* list)
{
    assert(list);
    assert(list -> data);

    char str[10] = "";

    printf("\n\033[31mWorning!!!\033[0m To long process! Do you wont press list?\n");
    scanf("%s", str);

    if (strcasecmp("yes", str) == 0)
    {
        int* new_data = (int*) calloc(list -> n_elem + 2, sizeof(int));
        int* new_next = (int*) calloc(list -> n_elem + 2, sizeof(int));
        int* new_prev = (int*) calloc(list -> n_elem + 2, sizeof(int));

        int now_pos = list -> head;
        new_data[0] = -1111;
        new_next[0] = 1;
        new_prev[0] = list -> n_elem;
        //new_prev[0] = 0;                                            // zamena na prev

        for (int i = 1; i <= list -> n_elem; i++)
        {
            new_data[i] = list -> data[now_pos];
            new_next[i] = i + 1;
            new_prev[i] = i - 1;
            now_pos = list -> next[now_pos];
        }

        new_next[list -> n_elem] = 0;

        free(list -> data);
        free(list -> next);
        free(list -> prev);

        list -> data = new_data;
        list -> next = new_next;
        list -> prev = new_prev;
        list -> capacity = list -> n_elem + 2;
        list -> free = list -> n_elem + 1;
        list -> next[list -> free] = list -> free + 1;
        list -> prev[list -> free] = -1;
    }
    List_Verify(list);
    List_Dump(list);
}

void Put_Elem_to_end(struct List* list, int position)
{
    list -> next[list -> free] = 0;
    list -> prev[list -> free] = list -> prev[0];
    list -> next[list -> prev[0]] = list -> free;

    list -> prev[0] = list -> free;
}

void Put_Elem_to_position(struct List* list, int position)
{
    list -> next[list -> free] = list -> next[list -> prev[position]];
    list -> prev[list -> free] = list -> prev[position];
    list -> next[list -> prev[position]] = list -> free;
    list -> prev[position] = list -> free;
}

void Resize_List(struct List* list, int position)
{
    if (list -> n_elem < list -> capacity / 4)
    {
        List_Realloc_Down(list);
    }
    while (list -> capacity <= position || list -> capacity <= list -> n_elem || list -> next[list -> free] == 0)
    {
        List_Realloc_Up(list);
    }
}

void Sort_list(struct List* list)
{
    List_Realloc_Down(list);
}

int List_Insert_Before(struct List* list, int val, int position)
{
    assert(list);
    assert(list -> data);

    Resize_List(list, position);

    list -> data[list -> free] = val;
    int last_free = list -> next[list -> free];

    if (list -> prev[position] == -1)
    {
        Put_Elem_to_end(list, position);
    }
    else
    {
        Put_Elem_to_position(list, position);
    }

    int ret = list -> free;
    list -> free = last_free;
    (list -> n_elem)++;

    return ret;
}

int List_Insert_After(struct List* list, int val, int position)
{
    return List_Insert_Before(list, val, list -> next[position]);
}

int Delete(struct List* list, int position)
{
    assert(list);
    assert(list -> data);

    list -> next[list -> prev[position]] = list -> next[position];
    list -> prev[list -> next[position]] = list -> prev[position];
    list -> data[position] = 0;

    list -> next[position] = list -> free;
    list -> prev[position] = -1;
    list -> free = position;
    list -> n_elem--;

    return position;
}

void List_Dump(struct List* list)
{
    assert(list);
    assert(list -> data);

    printf("\ncapacity = %d;\nhead = %d; \ntail = %d; \nn_elem = %d;\nfree = %d;\n;\n\n",list -> capacity, list -> head, list -> tail, list -> n_elem, list -> free);

    printf("index: ");
    for(int i = 0; i < list -> capacity; i++)
    {
        printf("%5d ", i);
    }
    printf("\n");
    printf("-----------------------------------------------------------------------------------------------------------------------\n");

    printf("data : ");
    for(int i = 0; i < list -> capacity; i++)
    {
        printf("%5d ", list -> data[i]);
    }
    printf("\n");

    printf("next : ");
    for(int i = 0; i < list -> capacity; i++)
    {
        printf("%5d ", list -> next[i]);
    }
    printf("\n");

    printf("prev : ");
    for(int i = 0; i < list -> capacity; i++)
    {
        printf("%5d ", list -> prev[i]);
    }
    printf("\n");

    List_Verify(list);
}

void Do_Graph(struct List* list)
{
    FILE* file_dot = fopen("Graph.dot", "w");

    assert(file_dot);

    list -> head = list -> next[0];
    list -> tail = list -> prev[0];

    fprintf(file_dot, "digraph G\n{\n\trankdir = \"LR\";\n\n\tnode[color = \"red\", fontsize = 14];\n\tedge[color = \"black\", fontcolor = \"blue\", fontsize = 12, weight = 0];\n\n\t");


    fprintf(file_dot, "List [shape = record, style = \"rounded\", label = \"capacity: %d | head: %d | tail: %d | n_elem: %d | free: %d\"];\n\t", list -> capacity, list -> head, list -> tail, list -> n_elem, list -> free);
    for (int i = 0; i < list -> capacity; i++)
    {
        fprintf(file_dot, "%d [shape = record, style = \"rounded\", label = \"ind: %d | val: %d | nxt: %d | prv: %d \"];\n\t", i, i, list -> data[i], list -> next[i], list -> prev[i]);
    }
    fprintf(file_dot, "\n\n\t");


    fprintf(file_dot, "List -> ");
    for (int i = 0; i < list -> capacity - 1; i++)
    {
        fprintf(file_dot,"%d -> ", i);
    }
    fprintf(file_dot,"%d [weight = 1000, color = \"#FFFFFF\"];\n", list -> capacity - 1);


    for (int i = list -> capacity - 2; i >= 0; i--)
    {
        fprintf(file_dot, "\t%d -> %d;\n", i, list -> next[i]);
    }

    //for (int i = list -> capacity - 2; i >= 0; i--)
    //{
    //    fprintf(file_dot, "\t%d -> %d[color = \"blue\"];\n", list -> next[i], i);
    //}

    fprintf(file_dot,"\tnode[color = \"green\", fontsize = 14];\n");
    fprintf(file_dot,"\t\"POIZON\\nELEM\" -> 0[color = \"green\"]\n");
    fprintf(file_dot,"\t\"HEAD\" -> %d[color = \"green\"]\n", list -> next[0]);
    fprintf(file_dot,"\t\"TAIL\" -> %d[color = \"green\"]\n", list -> prev[0]);
    fprintf(file_dot,"\t\"FREE\" -> %d[color = \"green\"]\n", list -> free);

    fprintf(file_dot, "\n}\n");
}

int Find_Reall_Id(struct List* list, int pos)
{
    if (pos > list -> n_elem)
    {
        list -> errors |= POSITION_OUT_OF_DATA;
        List_Verify(list);
    }

    int real_pos = GET_NEXT(0);//list -> head;

    for (int i = 1; i < pos; i++)
    {
        real_pos = GET_NEXT(real_pos);
    }
    printf("\n\033[32m*\033[0mReal_id of %d element is %d\033[32m*\033[0m\n", pos, real_pos);

    return real_pos;
}

int Get_Prev(struct List* list, int pos)
{
    return list -> prev[pos];
}

int Get_Next(struct List* list, int pos)
{
    return list -> next[pos];
}

int main()
{
    struct List list = {};
    List_Ctor(&list);

    for(int i = 1; i < 12; i++)
    {
        List_Insert_Before(&list, i, i);
    }

    //for(int i = 1; i < 12; i++)
    //{
        //Delete(&list, i);
        //List_Dump(list);
    //}

    Delete(&list, 4);
    List_Dump(&list);

    List_Insert_Before(&list, 200, 1);
    List_Insert_Before(&list, 100, 6);

    List_Dump(&list);
    Find_Reall_Id(&list, 1);

    Do_Graph(&list);
    List_Dtor(&list);
}
