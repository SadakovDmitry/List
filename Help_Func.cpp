#include "lib.h"
#include "main_func.h"
#include "help_func.h"

void Put_Elem_to_end(struct List* list, int position)
{
    list -> next[list -> free] = 0;
    list -> prev[list -> free] = GET_TAIL;
    list -> next[GET_TAIL] = list -> free;

    list -> prev[0] = list -> free;
}

void Put_Elem_to_position(struct List* list, int position)
{
    list -> next[list -> free] = list -> next[GET_PREV(position)];
    list -> prev[list -> free] = GET_PREV(position);
    list -> next[GET_PREV(position)] = list -> free;
    list -> prev[position] = list -> free;
}



void Resize_List(struct List* list, int position)
{
    if (list -> n_elem < list -> capacity / 4)
    {
        List_Realloc_Down(list);
    }
    while (list -> capacity <= position || list -> capacity <= list -> n_elem || GET_NEXT(list -> free) == 0)
    {
        List_Realloc_Up(list);
    }
}

void Sort_list(struct List* list)
{
    List_Realloc_Down(list);
}



int Check_List(struct List* list)
{
    int pos = GET_HEAD;

    for (int i = 1; i <= list -> n_elem; i++)
    {
        pos = GET_NEXT(pos);
    }

    if(pos == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Do_Graph(struct List* list)
{
    FILE* file_dot = fopen("Graph.dot", "w");

    assert(file_dot);

    list -> head = GET_HEAD;
    list -> tail = GET_TAIL;

    fprintf(file_dot, "digraph G\n{\n\trankdir = \"LR\";\n\n\tnode[color = \"red\", fontsize = 14];\n\tedge[color = \"black\", fontcolor = \"blue\", fontsize = 12, weight = 0];\n\n\t");


    fprintf(file_dot, "List [shape = record, style = \"rounded\", label = \"capacity: %d | head: %d | tail: %d | n_elem: %d | free: %d\"];\n\t", list -> capacity, list -> head, list -> tail, list -> n_elem, list -> free);
    for (int i = 0; i < list -> capacity; i++)
    {
        fprintf(file_dot, "%d [shape = record, style = \"rounded\", label = \"ind: %d | val: %d | nxt: %d | prv: %d \"];\n\t", i, i, list -> data[i], GET_NEXT(i), GET_PREV(i));
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
        fprintf(file_dot, "\t%d -> %d;\n", i, GET_NEXT(i));
    }

    //for (int i = list -> capacity - 2; i >= 0; i--)
    //{
    //    fprintf(file_dot, "\t%d -> %d[color = \"blue\"];\n", list -> next[i], i);
    //}

    fprintf(file_dot,"\tnode[color = \"green\", fontsize = 14];\n");
    fprintf(file_dot,"\t\"POIZON\\nELEM\" -> 0[color = \"green\"]\n");
    fprintf(file_dot,"\t\"HEAD\" -> %d[color = \"green\"]\n", GET_HEAD);
    fprintf(file_dot,"\t\"TAIL\" -> %d[color = \"green\"]\n", GET_TAIL);
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

    int real_pos = GET_HEAD;

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

int Get_Head(struct List* list)
{
    return list -> next[0];
}

int Get_Tail(struct List* list)
{
    return list -> prev[0];
}

void Beautiful_Dump()
{
    printf( "\n\n\033[36m          /＞   フ\n"
            "         |  _  _|\n"
            "        /`ミ _x 彡\n"
            "       /        |\n"
            "      /  ヽ     ﾉ\n"
            "  ／￣|     | | |\n"
            "  | (￣ヽ__ヽ_)_)\n"
            "  ＼二つ    \033[31mDUMP\033[0m ");
}

void Make_Program(struct List* list)
{
    List_Ctor(list);

    for(int i = 1; i < 12; i++)
    {
        List_Insert_Before(list, i, i);
    }

    //for(int i = 1; i < 12; i++)
    //{
        //Delete(&list, i);
    //}

    Delete(list, 4);

    List_Insert_Before(list, 200, 1);
    List_Insert_Before(list, 100, 6);

    Find_Reall_Id(list, 1);

    Do_Graph(list);
    List_Dtor(list);
}
