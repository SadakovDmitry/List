#include "lib.h"
#include "main_func.h"
#include "help_func.h"


int main()
{
    struct List list = {};
    List_Ctor(&list);
    List_Dump(&list);

    for(int i = 1; i < 12; i++)
    {
        List_Insert_Before(&list, i, i);
        List_Dump(&list);
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
