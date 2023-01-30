#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

struct node
{
    int data;
    node *next;
};

class linked_list
{
protected:
    node *head, *tail; // head: first node, tail: last node
    int size = 0;

public:
    linked_list()
    {
        head = nullptr;
        tail = nullptr;
    }
    ~linked_list()
    {
        delete_list();
    } //소멸자

    // 삽입연산들
    void push_front(int n);              // 노드를 맨 앞에 한 개 추가합니다.
    void push_back(int n);               // 노드를 끝에 한 개 추가합니다.
    void insert_node_at(int idx, int n); // 정해진 위치에 추가합니다.

    // 삭제연산들
    int pop_front();          // 맨 앞 노드를 지웁니다.
    int pop_back();           // 끝 노드를 지웁니다.
    int pop_node_at(int idx); // idx 위치의 노드를 지웁니다.
    void remove_node(int n);  // 값과 같은 노드를 한 개 지웁니다.
    void delete_list();       // linked list 전체를 지웁니다.

    // 대입
    void replace_value_at(int idx, int n);

    // 검색 및 표시
    int Size();
    void display_contents(); // linked list의 내용을 보여줍니다.
    int search_value(int n); // linked list의 내용을 보여줍니다.

    //week4 과제 관련 함수
    node *GetHead();
    node *GetTail();
};

// 삽입연산들
void linked_list::push_front(int n)
{                         // 리스트의 맨 앞에 노드를 추가합니다.
    node *ptr = new node; // 새 노드의 메모리를 할당해주고
    ptr->next = head;
    ptr->data = n;
    head = ptr;
    if (ptr->next == nullptr)
    {
        tail = ptr;
    }
    size++;
}

void linked_list::push_back(int n)
{                         // 리스트의 끝에 노드를 추가합니다.
    node *tmp = new node; // 새 노드의 메모리를 할당해주고
    tmp->data = n;
    tmp->next = nullptr;
    if (head == nullptr)
    { // head가 없다면 당연히 tail도 없겠죠.
        head = tmp;
        tail = tmp;
    }
    else
    {
        tail->next = tmp;
        tail = tail->next;
    }
    size++;
}

void linked_list::insert_node_at(int idx, int n)
{ // index에 있는 값을 지웁니다.
    if (size < idx || 0 > idx)
    {
        std::cout << "해당 인덱스는 존재하지 않습니다\n";
    }
    else if (idx == 0)
    { // idx == 0은 push_front(n)과 같다.
        push_front(n);
    }
    else if (idx == size)
    { // idx == size은 push_back(n)과 같다.
        push_back(n);
    }
    else
    {
        node *ptr = head;
        node *tmp = ptr;
        node *new_node = new node;
        new_node->data = n;
        new_node->next = nullptr; // 새 node를 일단 초기화해줍니다.
        for (int i = 0; i < idx; i++)
        {
            tmp = ptr;       // 들어갈 노드의 직전 노드 위치
            ptr = ptr->next; // 들어갈 노드의 위치
        }
        tmp->next = new_node;
        new_node->next = ptr;
        size++; // 노드를 삽입합니다.
    }
}

// 삭제연산
int linked_list::pop_front()
{ // 리스트의 맨 앞 노드를 지웁니다.
    if (head == nullptr)
    {
        std::cout << "linked list가 비어있습니다\n";
        exit(1);
    }
    node *ptr = head;
    int value = head->data;
    head = ptr->next;
    if (head == nullptr)
    {
        tail == nullptr;
    }
    delete ptr;
    size--;
    return value;
}

int linked_list::pop_back()
{
    if (size == 0)
    { // 리스트가 비었을때 pop_back()을 시도한 경우
        std::cout << "linked list가 비어있습니다\n";
        exit(1);
    }
    node *ptr = head;
    node *tmp = new node;
    while (ptr->next != nullptr)
    {
        tmp = ptr;
        ptr = ptr->next;
    }
    int value = ptr->data;
    if (ptr == head)
    { // pop_back() 으로인해 리스트가 빌 때
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        tail = tmp;
        tail->next = nullptr;
    }
    delete ptr;
    size--;
    return value;
}

int linked_list::pop_node_at(int idx)
{ // index에 있는 값을 지웁니다.
    int value = -1;
    if (size <= idx || 0 > idx)
    { // size < idx 에서 size <= idx 로 수정
        std::cout << "해당 인덱스는 존재하지 않습니다\n";
    }
    else if (size == 0)
    { // 리스트가 비었을때 pop_node_at()을 시도한 경우
        std::cout << "linked list가 비어있습니다\n";
        exit(1);
    }
    else if (idx == 0)
    { // ixd == 0 은 pop_front()와 같다.
        value = pop_front();
    }
    else if (idx == size - 1)
    { // ixd == size-1 은 pop_back()와 같다.
        value = pop_back();
    }
    else
    {
        node *ptr = head;
        node *tmp = ptr;
        for (int i = 0; i < idx; i++)
        {
            tmp = ptr;       // 현재 노드를 tmp로 두고
            ptr = ptr->next; // 다음 노드로 타겟을 바꿉니다.
        }
        //std::cout << ptr->data << "값의 노드를 하나 지웁니다\n";
        value = ptr->data;
        tmp->next = ptr->next; // 삭제할 노드를 빼고 연결하고
        if (ptr == tail)
        {
            tail = tmp;
        }
        size--; // 노드를 삽입합니다.
        delete ptr;
        ptr = nullptr;
    }
    return value;
}

void linked_list::remove_node(int n)
{ // 값을 찾아서 한 개를 지웁니다.
    node *ptr = head;
    node *tmp = ptr;
    while (ptr != nullptr)
    {
        if (ptr->data == n)
        {
            break;
        }
        else
        {
            tmp = ptr;       // 현재 노드를 tmp로 두고
            ptr = ptr->next; // 다음 노드로 타겟을 바꿉니다.
        }
    }
    if (ptr == nullptr)
    {
        std::cout << "값의 노드는 존재하지 않습니다\n";
    }
    else if (size == 1)
    { // size == 1 일 때 값을 제거
        head == nullptr;
        tail == nullptr;
    }
    else
    {
        size--;
        std::cout << ptr->data << "값의 노드를 하나 지웁니다\n";
        tmp->next = ptr->next; // 삭제할 노드를 빼고 연결하고
        if (ptr == tail)
        {
            tail = tmp;
        }
        delete ptr; // 동적 할당을 해제합니다
    }
}

void linked_list::delete_list()
{
    node *ptr = head;
    while (ptr != nullptr)
    {
        head = ptr->next;
        delete ptr;
        ptr = head;
    }
    size = 0;
    std::cout << "리스트가 삭제되었습니다 \n";
}

// 대입
void linked_list::replace_value_at(int idx, int n)
{
    node *ptr = head;
    if (size == 0)
    { // 리스트가 비어있을때
        std::cout << "linked list가 비어있습니다\n";
        exit(1);
    }
    else if (size <= idx || 0 > idx)
    { // size < idx 에서 size <= idx 로 수정
        std::cout << "해당 인덱스는 존재하지 않습니다\n";
    }
    else
    {
        for (int i = 0; i < idx; i++)
        {
            ptr = ptr->next; // 들어갈 노드의 위치
        }
        ptr->data = n;
    }
}

// 검색 및 표시
int linked_list::Size()
{
    return size;
}

void linked_list::display_contents()
{
    node *ptr = head;
    while (ptr != nullptr)
    {
        std::cout << ptr->data << "->";
        ptr = ptr->next;
    }
    std::cout << "nullptr"
              << "\n";
}

int linked_list::search_value(int n)
{
    node *ptr = head;
    int idx = 0;
    bool isFind = false;
    while (ptr != nullptr)
    {
        if (ptr->data == n)
        {
            std::cout << n << " 은 " << idx << " 에 있습니다.\n";
            isFind = true;
            break;
        }
        ptr = ptr->next;
        idx++;
    }
    if (isFind == false)
    {
        std::cout << n << " 은 리스트 안에 없습니다.\n";
        return -1;
    }
    else
    {
        return idx;
    }
}

////////////////////
////과제 관련 함수////
////////////////////

linked_list* GenerateList(int n)   //인자로 숫자 n을 받는다.
{
    linked_list *linkedlist;
    linkedlist = new linked_list;

    if(linkedlist->GetHead() != NULL)  //리스트가 비어있지 않으면 먼저 초기화 한다.
        linkedlist->delete_list();

     for (int i = 0; i < n; i++)    //push_back 함수를 통해 루프를 돌며 값을 추가한다.
    {
        linkedlist->push_back(i);
    }

    return linkedlist;
}

void CopyLS2DA(int *array, linked_list *linkedlist) //인자로 array, linkedlist를 받는다.
{
    node *point = linkedlist->GetHead();    //linkedlist의 head를 받아온다.

    for (int i = 0; i < linkedlist->Size(); i++)    //head를 받아서 linkedlist를 따라가며 array에 차례로 저장한다.
    {
        array[i] = point->data;
        point = point->next;
    }
}

node *linked_list::GetHead()
{
    return this->head;
}

node *linked_list::GetTail()
{
    return this->tail;
}

int main()
{
    linked_list *LinkedList;
    int *Array;

    LinkedList = GenerateList(10);
    Array = new int[LinkedList->Size()];
    CopyLS2DA(Array, LinkedList);

    LinkedList->display_contents();
    printf("Array = {");
    for (int i = 0; i < LinkedList->Size(); i++)
    {
        if (i + 1 == LinkedList->Size())
            printf(" %d ", Array[i]);
        else
            printf(" %d,", Array[i]);
    }
    printf("}\n");

    delete Array;
    delete LinkedList;

    return 0;
}