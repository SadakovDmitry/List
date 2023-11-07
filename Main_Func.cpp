#include "lib.h"
#include "main_func.h"
#include "help_func.h"

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
    list -> next[list -> capacity - 1] = 0;
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
    if (Check_List(list) == 0)
    {
        printf("\n\033[31mERROR\033[0m: RUN_THREW_LIST_WRONG");
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
    list -> next[list -> capacity - 1] = list -> capacity;
    list -> next[list -> capacity * 2 - 1] = 0;

    list -> capacity = list -> capacity * 2;
    List_Dump(list);
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
            now_pos = GET_NEXT(now_pos);
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
        //list -> next[list -> free] = list -> free + 1;
        list -> next[list -> free] = 0;
        list -> prev[list -> free] = -1;
    }
    List_Verify(list);
    List_Dump(list);
}

int List_Insert_Before(struct List* list, int val, int position)
{
    assert(list);
    assert(list -> data);
    //fprintf(stderr, "gggg----g---g---g--g---g----g---g---g---g----g----g-----g-----g------g------");
    Resize_List(list, position);

    list -> data[list -> free] = val;
    int last_free = list -> next[list -> free];

    if (GET_PREV(position) == -1)
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
    return List_Insert_Before(list, val, GET_NEXT(position));
}

int Delete(struct List* list, int position)
{
    assert(list);
    assert(list -> data);

    list -> next[GET_PREV(position)] = GET_NEXT(position);
    list -> prev[GET_NEXT(position)] = GET_PREV(position);
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

    Beautiful_Dump();
    printf("\ncapacity = %d;\n"
             "head     = %d;\n"
             "tail     = %d;\n"
             "n_elem   = %d;\n"
             "free     = %d;\n\n",list -> capacity, list -> head, list -> tail, list -> n_elem, list -> free);

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
        printf("%5d ", GET_NEXT(i));
    }
    printf("\n");

    printf("prev : ");
    for(int i = 0; i < list -> capacity; i++)
    {
        printf("%5d ", GET_PREV(i));
    }
    printf("\n");

    List_Verify(list);
}
