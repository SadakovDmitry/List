void List_Ctor(struct List* list);
void List_Dtor(struct List* list);
void List_Realloc_Down(struct List* list);
void List_Realloc_Up(struct List* list);
 int Delete(struct List* list, int position);
void List_Dump(struct List* list);
 int List_Insert_After(struct List* list, int val, int position);
 int List_Insert_Before(struct List* list, int val, int position);
void List_Verify(struct List* list);


#define GET_PREV(pos) Get_Prev(list, pos)
#define GET_NEXT(pos) Get_Next(list, pos)
#define GET_HEAD      Get_Head(list)
#define GET_TAIL      Get_Tail(list)


const int poizon = -1111;


struct List
{
    int* data;
    int* next;
    int* prev;
    int  head;
    int  tail;
    int  free;
    int  n_elem;
    int  capacity;
    int  errors;
};


enum List_err : unsigned int
{
    CAPACITY_ZERO                   = 1 << 0,
    NUM_ELEM_BIGGER_THEN_CAPACITY   = 1 << 1,
    START_ELEMENT_WAS_CHANGED       = 1 << 2,
    POSITION_OUT_OF_DATA            = 1 << 3
};
