#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <random>
#include <functional>
#include <vector>
#include <stack>


struct someObject
{
private:

    int field_1;
    char field_2;
    bool owned;
public:
    someObject(int f1, char f2) : field_1(f1), field_2(f2), owned(true) {}
    someObject() : field_1(10), field_2('z'), owned(true) {}

    int getField1() const
    {
        return field_1;
    }
    char getField2() const
    {
        return field_2;
    }
    void setField1(int field)
    {
        field_1 = field;
    }
    void setField2(char field)
    {
        field_2 = field;
    }
    ~someObject() {
        if (owned) {}
    }
    void setOwned(bool own)
    {
        owned = own;
    }
    bool getOwned()
    {
        return owned;
    }
    bool operator<(const someObject& temp) const {
        return field_1 < temp.field_1;
    }
    bool operator>(const someObject& temp) const {
        return field_1 > temp.field_1;
    }
};
template <typename T>
struct node
{
    T data;
    node<T>* parent;
    node<T>* leftChild;
    node<T>* rightChild;
    bool color; // 1 - red, 0 - black
    int index;

    node(T data, int index) : data(data), parent(nullptr), leftChild(nullptr), rightChild(nullptr), index(index), color(1) {}
};
template <typename T>
struct arr
{
    int currentSize;
    int maxSize;
    T* array;

    arr() : currentSize(0), maxSize(10), array(new T[10]) {}
    ~arr() {
        deleteAll();
        delete[] array;
    }

    void addTail(const T& temp)
    {
        if (currentSize >= maxSize)
        {
            resize();
        }
        array[currentSize++] = temp;
    }

    void resize()
    {
        maxSize *= 2;
        T* temp = new T[maxSize];
        for (int i = 0; i < currentSize; ++i)
        {
            temp[i] = array[i];
        }
        delete[] array;
        array = temp;
    }
    
    T findByIndex(int index)
    {
        if (index >= currentSize || index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return array[index];
    }
    
    void replaceByIndex(int index, const T& temp)
    {
        if (index >= currentSize || index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        array[index] = temp;
    }
    
    void deleteAll()
    {
        if constexpr (std::is_pointer<T>::value) {

            for (int i = 0; i < currentSize; ++i) {
                if (array[i] != nullptr && array[i]->getOwned())
                    delete array[i];
            }
        }
        currentSize = 0;
    }

    void bubbleSort() {
        for (int i = 0; i < currentSize - 1; ++i) {
            for (int j = 0; j < currentSize - i - 1; ++j) {
                bool swap = false;
                if constexpr (std::is_pointer<T>::value)
                {
                    swap = *array[j] > *array[j + 1];
                }
                else {
                    swap = array[j] > array[j + 1];
                }
                if (swap) {
                    std::swap(array[j], array[j + 1]);
                }
            }
        }
    }
    
    std::string toString()
    {
        int max = 5;
        std::ostringstream output;
        if (currentSize < 5)
        {
            for (int i = 0; i < max && i < currentSize; ++i)
            {
                if constexpr (std::is_arithmetic<T>::value) {
                    output << "(" << array[i] << ") ";
                }
                else if constexpr (std::is_pointer<T>::value)
                {
                    output << "(" << array[i]->getField1() << " , " << array[i]->getField2() << ") ";

                }
                else {
                    output << "(" << array[i].getField1() << " , " << array[i].getField2() << ") ";
                }
            }
        }
        if (currentSize > 10)
        {
            output << "... ";
        }
        if (currentSize >= 5)
        {
            for (int i = currentSize - max; i < currentSize; ++i)
            {
                if constexpr (std::is_arithmetic<T>::value) {
                    output << "(" << array[i] << ") ";
                }
                else if constexpr (std::is_pointer<T>::value)
                {
                    output << "(" << array[i]->getField1() << " , " << array[i]->getField2() << ") ";

                }
                else {
                    output << "(" << array[i].getField1() << " , " << array[i].getField2() << ") ";
                }
            }
        }
        return output.str();
    }
};
template <typename T>
struct tree
{
    node<T>* root;
    int size;
    static int index;
    tree() : root(nullptr), size(0) {}
    ~tree()
    {
        clear(root);
    }
    void clear(node<T>*& root)
    {
        if (root == nullptr)
        {
            return;
        }
        clear(root->leftChild);
        clear(root->rightChild);
        if constexpr (std::is_pointer<T>::value) {
            if (root->data && root->data->getOwned())
            {
                delete root->data;
                root->data = nullptr;
            }
        }
        delete root;
        root = nullptr;
        size = 0;
        index = 0;
    }
    void inOrder(node<T>* root, arr<T>* output)
    {
        if (root == nullptr) {
            return;
        }
       
        inOrder(root->leftChild, output);
        if constexpr (std::is_pointer<T>::value) {
            if (root->data != nullptr) {
                output->addTail(root->data);
            }
        }
        else {
            output->addTail(root->data);
        }

        inOrder(root->rightChild, output);
    }
    void preOrder(node<T>* root, arr<T>* output)
    {
        if (root == nullptr) {
            return;
        }
       
        if constexpr (std::is_pointer<T>::value) {
            if (root->data != nullptr) {
                output->addTail(root->data);
            }
        }
        else {
            output->addTail(root->data);
        }

        preOrder(root->leftChild, output);
        preOrder(root->rightChild, output);
    }
    int determineHeight(node<T>* root)
    {
        if (root == nullptr) {
            return 0;
        }
        int l = determineHeight(root->leftChild);
        int r = determineHeight(root->rightChild);
        return std::max(l, r) + 1;
    }
    node<T>* search(node<T>* root, const T& newData, std::function<bool(const T&, const T&)> comparator)
    {
        if (root == nullptr) {
            return nullptr;
        }
        if (comparator(newData, root->data) == false && comparator(root->data, newData) == false)
        {
            return root;
        }
        
        if (comparator(newData, root->data))
        {
            return search(root->leftChild, newData, comparator);
        }
        else {
            return search(root->rightChild, newData, comparator);
        }

    }
    void add(T newData, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* toAdd = new node<T>(newData, index++);
        toAdd->leftChild = nullptr;
        toAdd->rightChild = nullptr;
        if (root == nullptr) {
            root = toAdd;
            root->color = 0;
            size = 1;
            return;
        }

        node<T>* temp = root;
        node<T>* parent = nullptr;
        while (temp != nullptr)
        {
            parent = temp;
           
            if (comparator(newData, temp->data)) {
                temp = temp->leftChild;
            }
            else {
                temp = temp->rightChild;
            }
        }
        
        if (comparator(newData, parent->data)) {
            parent->leftChild = toAdd;
            size++;
        }
        else {
            parent->rightChild = toAdd;
            size++;
        }
        toAdd->parent = parent;
        adjust(toAdd);
    }

    void adjust(node<T>* temp)
    {
       
        while (temp != root && temp->parent->color == 1) {
            if (temp->parent == temp->parent->parent->leftChild)
            {
                node<T>* uncle = temp->parent->parent->rightChild;
                if (uncle != nullptr && uncle->color == 1)
                {
                    temp->parent->color = 0;
                    uncle->color = 0;
                    temp->parent->parent->color = 1;
                    temp = temp->parent->parent;
                }
                else { 
                    if (temp == temp->parent->rightChild)
                    {
                        temp = temp->parent;
                        rotateLeft(temp);
                    }
                    temp->parent->color = 0;
                    temp->parent->parent->color = 1;
                    rotateRight(temp->parent->parent);
                }
            }
            
            else {
                node<T>* uncle = temp->parent->parent->leftChild;
                
                if (uncle != nullptr && uncle->color == 1)
                {
                  
                    temp->parent->color = 0;
                    uncle->color = 0;
                    temp->parent->parent->color = 1;
                    temp = temp->parent->parent;
                }
                else { 
                    if (temp == temp->parent->leftChild)
                    {
                        temp = temp->parent;

                        rotateRight(temp);
                    }
                    temp->parent->color = 0;
                    temp->parent->parent->color = 1;
                    rotateLeft(temp->parent->parent);
                }
            }
        }
        
        root->color = 0;
    }
    //              x                    y
    //                \                 / \
    //                  y              x    b
    //                 / \              \
    //                a    b             a
    //
    //
    void rotateLeft(node<T>* temp)
    {
        node<T>* rChild = temp->rightChild;
        temp->rightChild = rChild->leftChild;
        if (rChild->leftChild != nullptr)
        {
            rChild->leftChild->parent = temp;
        }
        rChild->parent = temp->parent;
        if (temp->parent != nullptr)
        {
            if (temp == temp->parent->leftChild) {
                temp->parent->leftChild = rChild;
            }
            else {
                temp->parent->rightChild = rChild;
            }
        }
        else {
            root = rChild;
        }
        rChild->leftChild = temp;
        temp->parent = rChild;
    }
    
    void rotateRight(node<T>* temp)
    {
        node<T>* lChild = temp->leftChild;
        temp->leftChild = lChild->rightChild;
        if (lChild->rightChild != nullptr)
        {
            lChild->rightChild->parent = temp;
        }
        lChild->parent = temp->parent;
        if (temp->parent != nullptr)
        {
            if (temp == temp->parent->leftChild) {
                temp->parent->leftChild = lChild;
            }
            else {
                temp->parent->rightChild = lChild;
            }
        } 
        else {
            root = lChild;
        }
        lChild->rightChild = temp;
        temp->parent = lChild;
    }
    std::string toString(node<T>* root, int currentH = 0, int maxH = -1)
    {
        if (root == nullptr)
        {
            return "";
        }

        if (maxH == -1) {
            maxH = determineHeight(root);
        }

        std::ostringstream output;
        bool includeLayer = (currentH < 3) || (currentH >= maxH - 1);
        if (includeLayer) {
           
            if constexpr (std::is_arithmetic<T>::value) {
                output << root->index << " value: " << root->data
                    << " [c: " << (root->color ? "red" : "black")
                    << " p: " << (root->parent ? root->parent->index : -1)
                    << " l: " << (root->leftChild ? root->leftChild->index : -1)
                    << " r: " << (root->rightChild ? root->rightChild->index : -1) << "]\n";
            }
            else if constexpr (std::is_pointer<T>::value) {
                output << root->index << " value: " << root->data->getField1() << " " << root->data->getField2()
                    << " [c: " << (root->color ? "red" : "black")
                    << " p: " << (root->parent ? root->parent->index : -1)
                    << " l: " << (root->leftChild ? root->leftChild->index : -1)
                    << " r: " << (root->rightChild ? root->rightChild->index : -1) << "]\n";
            }
            else {
                output << root->index << " value: " << root->data.getField1() << " " << root->data.getField2()
                    << " [c: " << (root->color ? "red" : "black")
                    << " p: " << (root->parent ? root->parent->index : -1)
                    << " l: " << (root->leftChild ? root->leftChild->index : -1)
                    << " r: " << (root->rightChild ? root->rightChild->index : -1) << "]\n";
            }
        }

        output << toString(root->leftChild, currentH + 1, maxH);
        output << toString(root->rightChild, currentH + 1, maxH);

        return output.str();
    }

};

template <typename T>
int tree<T>::index = 0;


template <typename T>
std::function<bool(const T&, const T&)> create() {
    if constexpr (std::is_arithmetic<T>::value)
    {
        return [](const T& a, const T& b)
            {
                return a < b;
            };
    }
    else if constexpr (std::is_pointer<T>::value)
    {
        return [](const T& a, const T& b)
            {
                return a->getField1() < b->getField1();
            };
    }
    else
    {
        return [](const T& a, const T& b)
            {
                return a.getField1() < b.getField1();
            };
    }
}

int main()
{
    auto ptrComp = create<someObject*>();
    srand(time(0));
    const int MAX_ORDER = 1;
    tree<someObject*>* rbt = new tree<someObject*>();
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);
        clock_t t1 = clock();

        for (int i = 0; i < n; i++)
        {
            int x = ((rand() << 15) + rand()) % 1000000000;
            char y = letters[rand() % 26];
            someObject* temp = new someObject(x, y);
            rbt->add(temp, ptrComp);
        }
        clock_t t2 = clock();
        double t3 = (t2 - t1) / (double)CLOCKS_PER_SEC;
        std::ostringstream out;
        std::cout << rbt->toString(rbt->root) << std::endl;
        std::cout << "Current tree height " << rbt->determineHeight(rbt->root) << std::endl;
        std::cout << "Current tree size: " << rbt->size << std::endl;
        std::cout << "Total add time: " << rbt->size << " elements: " << t3 << "s" << std::endl;
        //std::cout << std::fixed << std::setprecision(10);
        std::cout << "Average add time: " << t3 / n * 1000000 << "us" << std::endl;
        std::cout << "Tree height to data size ratio: " << rbt->determineHeight(rbt->root) / rbt->size << std::endl;
        std::cout << "Log2 of data size: " << log2(rbt->size) << std::endl;
        std::cout << "Tree height to log of data size ratio: " << rbt->determineHeight(rbt->root) / log2(rbt->size) << std::endl;

        const int m = pow(10, 4);
        int hits = 0;
        t1 = clock();
        for (int i = 0; i < m; i++)
        {
            int x = rand() % (10000) + 1;
            char y = letters[rand() % 26];
            someObject* temp = new someObject(x, y);
            node<someObject*>* result = rbt->search(rbt->root, temp, ptrComp);
            if (result != NULL)
            {
                hits++;
            }
            delete temp;
        }
        t2 = clock();
        t3 = (t2 - t1) / (double)CLOCKS_PER_SEC;
        std::cout << "Search time" << m << " elements: " << t3 << "s" << std::endl;
        std::cout << "Hits: " << hits << std::endl;
        std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;

        rbt->clear(rbt->root);
    }
    delete rbt;
    return 0;
}

int main2() {
    auto ptrComp = create<someObject*>();
    arr<someObject*>* a1 = new arr<someObject*>();
    arr<someObject*>* a2 = new arr<someObject*>();
    tree<someObject*>* rbt = new tree<someObject*>();
    someObject* temp1 = new someObject(10, 'c');
    someObject* temp2 = new someObject(15, 'l');
    someObject* temp3 = new someObject(1, 'd');
    someObject* temp4 = new someObject(100, 'a');
    someObject* temp5 = new someObject(13, 'e');
    rbt->add(temp1, ptrComp);
    rbt->add(temp2, ptrComp);
    rbt->add(temp3, ptrComp);
    rbt->add(temp4, ptrComp);
    rbt->add(temp5, ptrComp);
    a1->deleteAll();
    rbt->inOrder(rbt->root, a1);
    std::cout << a1->toString() << std::endl << "////////////////////" << std::endl;
    a2->deleteAll();
    rbt->preOrder(rbt->root, a2);
    std::cout << a2->toString() << std::endl;
    //delete a1;
    delete rbt;
    std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
    auto intComp = create<int>();
    arr<int>* a3 = new arr<int>();
    arr<int>* a4 = new arr<int>();
    tree<int>* rbti = new tree<int>();

    rbti->add(10, intComp);
    rbti->add(15, intComp);
    rbti->add(1, intComp);
    rbti->add(100, intComp);
    rbti->add(13, intComp);

    a3->deleteAll();
    rbti->inOrder(rbti->root, a3);
    std::cout << a3->toString() << std::endl << "////////////////////" << std::endl;

    a4->deleteAll();
    rbti->preOrder(rbti->root, a4);
    std::cout << a4->toString() << std::endl;

    delete rbti;
}