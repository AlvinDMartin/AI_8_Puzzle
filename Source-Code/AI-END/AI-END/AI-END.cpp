// MainCode_8Puzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<stdio.h>
#include <iostream>
#include<malloc.h>
#include<string.h>
#include<ctime>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
#define SIDE 3 // Length of the board 

//Khai Bao
enum class Actions {
    left = 0,
    right = 1,
    up = 2,
    down = 3
};
string action_names[] = { "left","right","up","down" };
typedef struct Node_array//should be tile
{
    int state[SIDE][SIDE];
    Actions action;
    struct Node_array* parent; // tree search

    int path_cost;
    int depth;
    struct Node_array* nextNode; // list


} Node;
typedef struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    Node* head;            //pointer to the first node in the list
    Node* tail;            //pointer to the last node in the list
};
typedef struct Vitri
{
    int index_i = 0;
    int index_j = 0;
};
typedef struct VitriXemDuoc
{
    int ii = 0;
    int jj = 0;
}XemDuoc;
int CountNode = 0;


//Khởi tạo dánh sách liên kết để lưu giá trị
struct LinkedList {
    int data;
    struct LinkedList* next;
};
typedef struct LinkedList* list;

//Function
Vitri timkiemRong(Node* node)
{
    Vitri A;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {
                A.index_i = i;
                A.index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    return A;
}
//danh sach liên kết
list InitHead() {
    list head;
    head = NULL;
    return head;
}
list CreateNode(int value) {
    list temp;
    temp = (list)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    temp->data = value;
    return temp;
}
list AddHead(list head, int value) {
    list temp = CreateNode(value);
    if (head == NULL) {
        head = temp;
    }
    else {
        temp->next = head;
        head = temp;
    }
    return head;
}
void Traverser(list head) {
    cout << endl;
    for (list p = head; p != NULL; p = p->next) {
        cout << p->data << " ";
    }
}
bool CheckExistedInList(list head, int num)
{
    for (list p = head; p != NULL; p = p->next)
    {
        if (num == p->data)
            return true;
    }
    return false;
}
bool Search(list head, int value) {
    int position = 0;
    for (list p = head; p != NULL; p = p->next) {
        if (p->data == value) {
            return true;
        }
        ++position;
    }
    return false;
}

void set_zero(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = 0;
}
void copy_array(int a[][SIDE], int b[][SIDE])//copy b to a
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = b[i][j];
}
int is_equal(int a[][SIDE], int b[][SIDE])
{
    int i, j, flag = 1;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            if (a[i][j] != b[i][j])
                flag = 0;
    return flag;
}
void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}
bool isLegalAction(Node* node, Actions action) {

    Vitri A = timkiemRong(node);

    if (action == Actions::left && A.index_j == 0)  return false;
    if (action == Actions::right && A.index_j == 2)  return false;

    switch (A.index_i)
    {
    case 0:
        if (action == Actions::up) return false;
        return true;
    case 2:
        if (action == Actions::down) return false;
        return true;
    default:
        return true;
    }
    return true;
}

//in gia tri
void print_array(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            printf("%d  | ", a[i][j]);
        cout << endl;
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        cout << endl;
    }
    cout << endl;
}
void print_array2(int a[][SIDE], int x, int y)
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
        {
            if ((i == x && j == y) || a[i][j] == 0)
            {
                printf("%d  | ", a[i][j]);
            }
            else
            {
                cout << "X  | ";
            }
        }
        cout << endl;
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        cout << endl;
    }
    cout << endl;
}
void print_array3(int a[][SIDE], list l)
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
        {
            if (!Search(l, a[i][j]))
            {
                printf("%d  | ", a[i][j]);
            }
            else
            {
                cout << "X  | ";
            }
        }
        cout << endl;
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        cout << endl;
    }
    cout << endl;
}


Node* Child_node(Node* node, Actions action) {
    Node* child = (Node*)malloc(sizeof(Node));

    Vitri A = timkiemRong(node);
    copy_array(child->state, node->state);
    child->action = action;
    child->parent = node;
    child->path_cost = node->path_cost + 1;
    if (action == Actions::left) {
        swap(child->state[A.index_i][A.index_j - 1], child->state[A.index_i][A.index_j]);
    }
    else if (action == Actions::right) {
        swap(child->state[A.index_i][A.index_j + 1], child->state[A.index_i][A.index_j]);
    }
    else if (action == Actions::up) {
        swap(child->state[A.index_i - 1][A.index_j], child->state[A.index_i][A.index_j]);
    }
    else if (action == Actions::down) {
        swap(child->state[A.index_i + 1][A.index_j], child->state[A.index_i][A.index_j]);
    }

    return child;

    //can't free memory
}
NodeList* FIFO_initial() {
    NodeList* list;
    list = (NodeList*)malloc(sizeof(NodeList));
    list->nodeCount = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
    //can't free memory
}
void FIFO_add(NodeList* list, Node* node) {
    if (list->nodeCount <= 0) {
        list->head = node;
        list->tail = node;
        list->nodeCount += 1;
        return;
    }
    list->tail->nextNode = node;
    list->tail = node;
    list->nodeCount += 1;
}
Node* FIFO_pop(NodeList* list) {
    if (list->nodeCount <= 0) {
        return NULL;
    }
    Node* temp = list->head;
    list->nodeCount -= 1;
    if (list->nodeCount <= 0) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        list->head = temp->nextNode;
    }
    return temp;
}

void LIFO_add(NodeList* list, Node* node)
{
    if (list->nodeCount <= 0)
    {
        list->head = node;
        list->tail = node;
        list->nodeCount += 1;
        return;
    }
    node->nextNode = list->head;
    list->head = node;
    list->nodeCount += 1;
}
Node* LIFO_pop(NodeList* list)
{
    if (list->nodeCount <= 0)
    {
        return NULL;
    }
    Node* temp = list->head;
    if (list->nodeCount <= 0)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = temp->nextNode;
    }
    list->nodeCount -= 1;
    return temp;
}

bool Goal_test(Node* node, Node* goal)
{
    return is_equal(node->state, goal->state);
}

bool checkExist(NodeList* list, Node* node) {
    Node* _node;
    int i = 0;
    _node = list->head;
    while (i < list->nodeCount) {
        if (Goal_test(node, _node)) {
            return true;
        }
        _node = _node->nextNode;
        i++;
    }
    return false;
}

void Solution(Node* node) {
    cout << "=======================" << endl;
    printf("\n(^-^).Solution.(^-^)\n");
    while (node->parent != NULL) {

        print_array(node->state);
        printf("\n-----------------^^^-----\n");
        cout << "\naction: " << action_names[int(node->action)] << endl;
        cout << "-----------------" << endl;
        node = node->parent;
    }
    print_array(node->state);

}


bool Win = false;
void DFSChild(NodeList* explorer, Node* node, Node* goal)
{
    int action;
    for (action = 0; action < 4; action++) {
        if (isLegalAction(node, (Actions)action)) {
            Node* child = (Child_node(node, (Actions)action));
            if (checkExist(explorer, child) == false)
            {
                if (Goal_test(child, goal))
                {
                    Solution(child);
                    Win = true;
                    return;
                }
                FIFO_add(explorer, child);

                DFSChild(explorer, child, goal);
            }
            else
            {
                free(child);
            }

        }
        if (Win) break;
    }
}
void depthFirstSearch(Node* root, Node* goal) {
    Node* node = root;
    int Path_cost = 0;
    if (Goal_test(node, goal)) {
        Solution(node);
        return;
    }
    NodeList* frontier;
    NodeList* explorer;
    //frontier = FIFO_initial();
    explorer = FIFO_initial();
    //FIFO_add(frontier, node);
   // do {
        //node = FIFO_pop(frontier);
    FIFO_add(explorer, node);
    //action
    //for (action = 0; action < 4; action++) {
    DFSChild(explorer, node, goal);
    //}


//} while (frontier->nodeCount > 0);
}



Node* RemoveNode(NodeList* list, Node* node)
{
    Node* n1 = list->head;
    Node* n2 = n1->nextNode;
    if (list->nodeCount <= 0)
    {
        return NULL;
    }
    for (int i = 0; i < list->nodeCount; i++)
    {
        if (is_equal(n2->state, node->state))
        {
            n1->nextNode = n2->nextNode;
        }
        n1 = n1->nextNode;
    }
}
int manhattanDist(Node* node, Node* goal)
{
    int x0, y0; 
    int x1, y1; 
    int dx, dy; 
    int sum = 0;
    for (y0 = 0; y0 < 3; ++y0) {
        for (x0 = 0; x0 < 3; ++x0) {
            for (y1 = 0; y1 < 3; ++y1) {
                for (x1 = 0; x1 < 3; ++x1) {
                    if (node->state[y0][x0] == goal->state[y1][x1]) {
                        dx = (x0 - x1 < 0) ? x1 - x0 : x0 - x1;
                        dy = (y0 - y1 < 0) ? y1 - y0 : y0 - y1;
                        sum += dx + dy;
                    }
                }
            }
        }
    }

    return sum;
}
void breadthFirstSearch(Node* root, Node* goal)
{
    Node* node = root;
    int action;
    if (Goal_test(node, goal)) 
    {
        Solution(node);     
        return;
    }
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    FIFO_add(frontier, node);
    while (frontier->nodeCount > 0)
    {
        node = FIFO_pop(frontier);
        FIFO_add(explorer, node);       
        CountNode = explorer->nodeCount;
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action)) {
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false)   
                {
                    if (checkExist(frontier, child) == false)   
                    {
                        if (Goal_test(child, goal))
                        {
                            Solution(child);
                            return;
                        }
                        FIFO_add(frontier, child);
                    }
                }
            }
        }
    }
}
void aStarSearch(Node* root, Node* goal)
{
    Node* node = root;
    int action;
    if (Goal_test(node, goal)) //kiểm tra xem state hiện tại có bằng goal srate không
    {
        Solution(node);     // nếu bằng thì in ra
        return;
    }
    // nếu chưa bằng:
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    LIFO_add(frontier, node);
    while (frontier->nodeCount > 0)
    {
        node = LIFO_pop(frontier);
        LIFO_add(explorer, node);       //Đổ từng node của frontier sang explorer
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action)) { // nếu di chuyển được
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false)   //kiểm tra có di chuyển lại bước trước đó
                {
                    if (checkExist(frontier, child) == false)   //kiểm tra có di chuyển lại bước ban đầu
                    {
                        if (Goal_test(child, goal))
                        {
                            Solution(child);
                            return;
                        }
                        FIFO_add(frontier, child);
                    }
                    else
                    {
                        int i = 0;
                        Node* temp = frontier->head;
                        while (i <= frontier->nodeCount - 1)
                        {
                            if (is_equal(temp->state, child->state) == 1 && (manhattanDist(child, goal) < manhattanDist(temp, goal)))   //hàm tính chi phí để đi đến trạng thái đích
                            {
                                LIFO_add(frontier, child);
                                RemoveNode(frontier, temp);
                            }
                            temp = temp->nextNode;
                            i++;
                        }
                    }
                }
            }
        }
    }
    while (frontier->nodeCount > 0);
    printf("No solution");
}
void FindX_Asearch(Node* root, int firstvalue, int x, int y)
{
    bool demaction = true;
    bool dem_Quaylui = true;
    Node* node = root;
    int action;
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    FIFO_add(frontier, node);
    list l = InitHead();
    l = AddHead(l, 0);
    l = AddHead(l, firstvalue);
    vector<int> label;
    if (firstvalue != 0)
    {
        label.push_back(firstvalue);
    }
    bool exit = false;
    // print
    cout << "Current map with X:  " << endl;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (node->state[i][j] == 0 || (i == x && j == y))
            {
                printf_s("%d  | ", node->state[i][j]);
            }
            else
            {
                cout << "X  | ";
            }
        }
        cout << endl;
        for (int k = 0; k < SIDE; k++)
            printf_s("---|-");
        cout << endl;
    }

    cout << "=========== Find X in Node =============" << endl;

    while (!exit)
    {

        node = FIFO_pop(frontier);
        if (node == NULL)
        {
            cout << "TAO NULL ROI" << endl;
        }
        FIFO_add(explorer, node);       //Đổ từng node của frontier sang explorer
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action) && !exit) { // nếu di chuyển được
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false && demaction == true)   //kiểm tra có di chuyển lại bước trước đó
                {
                    if (checkExist(frontier, child) == false && demaction == true && dem_Quaylui == true)   //kiểm tra có di chuyển lại bước ban đầu
                    {
                        int i, j;
                        demaction = false;
                        for (i = 0; i < SIDE; i++)
                        {
                            for (j = 0; j < SIDE; j++)
                            {
                                if ((i == x && j == y) || Search(l, child->state[i][j]) == true)///////////////////////////////////////
                                {
                                    printf_s("%d  | ", child->state[i][j]);
                                    if (child->state[i][j] != 0 && !CheckExistedInList(l, child->state[i][j]))
                                    {
                                        label.push_back(child->state[i][j]);
                                        l = AddHead(l, child->state[i][j]);

                                        if (label.size() == 8)
                                        {
                                            exit = true;
                                            copy_array(root->state, child->state);
                                        }
                                    }
                                }
                                else
                                {
                                    cout << "X  | ";
                                    if (demaction == false)
                                    {

                                    }
                                }
                            }
                            cout << endl;
                            for (j = 0; j < SIDE; j++)
                                printf_s("---|-");
                            cout << endl;
                        }
                        cout << "========================" << endl;
                        FIFO_add(frontier, child);
                        //Traverser(l);
                        cout << endl;
                    }
                    else
                    {
                        int i = 0;
                        Node* temp = frontier->head;
                        while (i <= frontier->nodeCount - 1)
                        {
                            temp = temp->nextNode;
                            i++;
                        }
                    }
                }
            }
        }
        demaction = true;
    }
}
void FindX_DFS(Node* root, int firstvalue, int x, int y)
{
    bool demaction = true;
    Node* node = root;
    int Path_cost = 0;
    //if (Goal_test(node, goal)) {
    //    Solution(node);
    //    return;
    //}
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    int action;
    FIFO_add(frontier, node);
    list l = InitHead();
    l = AddHead(l, 0);
    l = AddHead(l, firstvalue);
    vector<int> label;
    if (firstvalue != 0)
    {
        label.push_back(firstvalue);
    }
    bool exit = false;
    // print
    cout << "Current map with X:  " << endl;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (node->state[i][j] == 0 || (i == x && j == y))
            {
                printf_s("%d  | ", node->state[i][j]);
            }
            else
            {
                cout << "X  | ";
            }
        }
        cout << endl;
        for (int k = 0; k < SIDE; k++)
            printf_s("---|-");

        cout << endl;

    }
    cout << "=========== Find X in Node =============" << endl;

    do {
        node = FIFO_pop(frontier);
        FIFO_add(explorer, node);
        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action) && !exit) {
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false && demaction == true /*|| checkExist(frontier, child) == false && demaction == true*/) {
                    int i, j;
                    demaction = false;
                    for (i = 0; i < SIDE; i++)
                    {
                        for (j = 0; j < SIDE; j++)
                        {
                            if ((i == x && j == y) || Search(l, child->state[i][j]) == true)///////////////////////////////////////
                            {

                                printf_s("%d  | ", child->state[i][j]);
                                if (child->state[i][j] != 0 && !CheckExistedInList(l, child->state[i][j]))
                                {
                                    label.push_back(child->state[i][j]);
                                    l = AddHead(l, child->state[i][j]);

                                    if (label.size() == 8)
                                    {
                                        exit = true;
                                        copy_array(root->state, child->state);
                                    }
                                }
                                //cout << test << endl;
                            }
                            else
                            {
                                cout << "X  | ";
                            }
                        }
                        cout << endl;
                        for (j = 0; j < SIDE; j++)
                            printf_s("---|-");

                        cout << endl;
                    }
                    cout << "========================" << endl;
                    Traverser(l);
                    cout << endl;
                    FIFO_add(frontier, child);


                    //free(child);
                }
            }
        }
        demaction = true;
    } while (/*frontier->nodeCount > 0 &&*/ !exit);
}
void FindX(Node* root, int firstvalue, int x, int y)
{
    Node* node = root;
    int action;
    NodeList* frontier;
    NodeList* explorer;
    frontier = FIFO_initial();
    explorer = FIFO_initial();
    FIFO_add(frontier, node);
    list l = InitHead();
    l = AddHead(l, 0);
    l = AddHead(l, firstvalue);
    vector<int> label;
    if (firstvalue != 0)
    {
        label.push_back(firstvalue);
    }

    bool exit = false;
    // print
    cout << "Current map with X:  " << endl;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (node->state[i][j] == 0 || (i == x && j == y))
            {
                printf_s("%d  | ", node->state[i][j]);

            }
            else
            {
                cout << "X  | ";
            }

        }
        cout << endl;
        for (int k = 0; k < SIDE; k++)
            printf_s("---|-");

        cout << endl;

    }

    cout << "=========== Find X inn Node =============" << endl;

    while (!exit)
    {
        node = FIFO_pop(frontier);
        FIFO_add(explorer, node);       //Đổ từng node của frontier sang explorer

        //action
        for (action = 0; action < 4; action++) {
            if (isLegalAction(node, (Actions)action) && !exit) { // nếu di chuyển được
                Node* child = (Child_node(node, (Actions)action));
                if (checkExist(explorer, child) == false)   //kiểm tra có di chuyển lại bước trước đó
                {
                    if (checkExist(frontier, child) == false)   //kiểm tra có di chuyển lại bước ban đầu
                    {
                        int i, j;
                        for (i = 0; i < SIDE; i++)
                        {
                            for (j = 0; j < SIDE; j++)
                            {
                                if ((i == x && j == y) || Search(l, child->state[i][j]) == true)///////////////////////////////////////
                                {
                                    printf_s("%d  | ", child->state[i][j]);
                                    if (child->state[i][j] != 0 && !CheckExistedInList(l, child->state[i][j]))
                                    {
                                        label.push_back(child->state[i][j]);
                                        l = AddHead(l, child->state[i][j]);

                                        if (label.size() == 8)
                                        {
                                            exit = true;
                                            copy_array(root->state, child->state);

                                        }
                                    }
                                }
                                else
                                {
                                    cout << "X  | ";
                                }
                            }
                            cout << endl;
                            for (j = 0; j < SIDE; j++)
                                printf_s("---|-");

                            cout << endl;
                        }
                        cout << "========================" << endl;
                        FIFO_add(frontier, child);
                        //Traverser(l);
                        cout << endl;

                    }
                    else
                    {
                        int i = 0;
                        Node* temp = frontier->head;
                        while (i <= frontier->nodeCount - 1)
                        {
                            temp = temp->nextNode;
                            i++;
                        }
                    }
                }
            }
        }
    }

    //how to release memory here ??? (frontier, explorer)
}


//Update
Node* start_Zero(Node* root)         //ham cho 0 ve vi tri [0][0]
{
    Node* child = (Node*)malloc(sizeof(Node));
    Vitri A = timkiemRong(root);
    copy_array(child->state, root->state);

    while (A.index_j != 0)
    {
        swap(child->state[A.index_i][A.index_j - 1], child->state[A.index_i][A.index_j]);
        A.index_j--;
    }
    while (A.index_i != 0)
    {
        swap(child->state[A.index_i - 1][A.index_j], child->state[A.index_i][A.index_j]);
        A.index_i--;
    }

    return child;

}
Node* random8Puzzle(Node* root)
{
    srand((int)time(0));
    int action;
    Actions test_action = Actions::right;
    bool ktra = true;
    Node* child = (Node*)malloc(sizeof(Node));
    int Mangrandom[4];
    copy_array(child->state, root->state);
    for (int v = 0; v < 5; v++)
    {
        ktra = true;
        for (int d = 0; d < 4; d++)
        {
            Mangrandom[d] = -1;
        }
        Vitri A = timkiemRong(child);
        for (action = 0; action < 4; action++) {
            if (isLegalAction(child, (Actions)action))
            {
                Mangrandom[action] = action;
            }
        }
        while (ktra != false)
        {
            int ran = rand() % 4;
            if ((Actions)Mangrandom[ran] == Actions::left && test_action != Actions::right) {
                swap(child->state[A.index_i][A.index_j - 1], child->state[A.index_i][A.index_j]);
                ktra = false;
                test_action = Actions::left;
            }
            else if ((Actions)Mangrandom[ran] == Actions::right && test_action != Actions::left) {
                swap(child->state[A.index_i][A.index_j + 1], child->state[A.index_i][A.index_j]);
                ktra = false;
                test_action = Actions::right;
            }
            else if ((Actions)Mangrandom[ran] == Actions::up && test_action != Actions::down) {
                swap(child->state[A.index_i - 1][A.index_j], child->state[A.index_i][A.index_j]);
                ktra = false;
                test_action = Actions::up;
            }
            else if ((Actions)Mangrandom[ran] == Actions::down && test_action != Actions::up) {
                swap(child->state[A.index_i + 1][A.index_j], child->state[A.index_i][A.index_j]);
                ktra = false;
                test_action = Actions::down;
            }
        }
    }
    return child;
}
int Length(list& head) {
    int length = 0;
    for (list p = head; p != NULL; p = p->next) {
        ++length;
    }
    return length;
}
void inX(Node* node, int x, int y, list& l)
{
    bool exit = false;
    cout << "------------------V-----" << endl;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if ((i == x && j == y) || Search(l, node->state[i][j]) == true)
            {
                printf_s("%d  | ", node->state[i][j]);
                if (node->state[i][j] != 0 && !CheckExistedInList(l, node->state[i][j]))
                {
                    l = AddHead(l, node->state[i][j]);
                }
            }
            else
            {
                cout << "X  | ";
            }
        }
        cout << endl;
        for (int k = 0; k < SIDE; k++)
            printf_s("---|-");
        cout << endl;
    }
    //Traverser(l);
    cout << endl;
}
bool checkRun(list head)
{
    if (Length(head) == 9)
    {
        return true;
    }
    return false;
}
Node* DatFindX(Node* root, int x, int y)
{
    Node* child = root;
    list l = InitHead();
    l = AddHead(l, 0);
    if (!CheckExistedInList(l, child->state[x][y]))
    {
        l = AddHead(l, child->state[x][y]);
    }
    Actions test_action = Actions::right;
    int mang1[24] = { 1,3,0,2,1,3,0,2,1,1,3,3,0,0,2,2,1,1,3,3,0,0,2,2 };
    while (Length(l) < 9)
    {
        for (int i = 0; i < 24; i++)
        {
            Vitri A = timkiemRong(child);
            if ((Actions)mang1[i] == Actions::left && test_action != Actions::right) {
                swap(child->state[A.index_i][A.index_j - 1], child->state[A.index_i][A.index_j]);
                test_action = Actions::left;
                inX(child, x, y, l);
                if (checkRun(l) == true)
                {
                    break;
                }
            }
            else if ((Actions)mang1[i] == Actions::right && test_action != Actions::left) {
                swap(child->state[A.index_i][A.index_j + 1], child->state[A.index_i][A.index_j]);
                test_action = Actions::right;
                inX(child, x, y, l);
                if (checkRun(l) == true)
                {
                    break;
                }
            }
            else if ((Actions)mang1[i] == Actions::up && test_action != Actions::down) {
                swap(child->state[A.index_i - 1][A.index_j], child->state[A.index_i][A.index_j]);
                test_action = Actions::up;
                inX(child, x, y, l);
                if (checkRun(l) == true)
                {
                    break;
                }
            }
            else if ((Actions)mang1[i] == Actions::down && test_action != Actions::up) {
                swap(child->state[A.index_i + 1][A.index_j], child->state[A.index_i][A.index_j]);
                test_action = Actions::down;
                inX(child, x, y, l);
                if (checkRun(l) == true)
                {
                    break;
                }
            }
        }
    }
    return child;
}


int main()
{
    
    Node* Goal, * Start;
    int i, j, r = 0;
    int goalstate[9] = { 0,1,2,3,4,5,6,7,8 };
    XemDuoc* xd = (XemDuoc*)malloc(sizeof(XemDuoc));
    Goal = (Node*)malloc(sizeof(Node));
    Start = (Node*)malloc(sizeof(Node));


    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
        {
            Goal->state[i][j] = goalstate[r];
            Start->state[i][j] = goalstate[r];
            r++;
        }

    cout << "Goal state:" << endl;
    print_array(Goal->state);
    Goal->parent = NULL;
    Goal->depth = 0;


    cout << "Current state" << endl;
    Start = random8Puzzle(Start);
    Start = start_Zero(Start);
    Start->parent = NULL;
    Start->path_cost = 0;
    print_array(Start->state);

    cout << endl;
    cout << "============= GAME 8 PUZZLE AUTO - SECRET ============== " << endl;
    cout << "Input the location that you want to show the value: " << endl;
    do
    {
        cout << "Input row: ";
        cin >> xd->ii;
    } while (xd->ii < 0 || xd->ii > 2);

    do
    {
        cout << "Input column: ";
        cin >> xd->jj;
    } while (xd->jj < 0 || xd->jj > 2);

    int x = xd->ii;
    int y = xd->jj;

    cout << "======================= Find X =========================" << endl;
    cout << endl;
    print_array2(Start->state, x, y);
    Start = DatFindX(Start, x, y);


    cout << "========================= Swap =========================" << endl;
    clock_t start = clock();
    //depthFirstSearch(Start, Goal);
    breadthFirstSearch(Start, Goal);
    //aStarSearch(Start, Goal);
    printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    printf("The number of nodes browsed: %.2d\n", CountNode);


    // free 
    free(Goal);
    free(Start);
    free(xd);
    
    return 0;
}