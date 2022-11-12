/*library implementing the linear data storing facility.
diagrams can be found in diary 2021
*/
#include<iostream>
using namespace std;
enum iterate
{
    forward = 0, backward = 1
};

template<typename T>
class List
{
public:

    List* next;//storing the address of the next record: NULL if its the first record
    List* previous;//storing the address of the previous record
    T s;//object to be stored
    List* Front;//stores the address of the first entry of the structure
    List* rear;// stores the address of the last entry of the structure

    List()//default ctor
    {

        next = previous = Front = rear = NULL;
    }
    bool add_rear(const T& ob)//returns true if it appends properly
    {
        List* node = new List;
        if (node == NULL)
        {
            cout << "\n error: memory not given";
            return false;
        }
        if (Front == NULL && rear == NULL)
        {
            Front = node;
        }
        node->next = NULL;
        node->previous = rear;
        node->s = ob;
        if (rear != NULL)
            rear->next = node;

        rear = node;
        return true;
    }
    bool add_front(const T& ob)
    {
        List* node = new List;
        if (node == NULL)
        {
            cout << "\n error: memory not given";
            return false;
        }
        if (Front == NULL && rear == NULL)
        {
            rear = node;
        }
        node->previous = NULL;
        node->next = Front;
        node->s = ob;
        if (Front != NULL)
            Front->previous = node;

        Front = node;
        return true;
    }
    //testing function
    void display(iterate where)//to display the entire structre
    {
        if (where == iterate::forward)
        {
            List* temp = Front;
            while (temp != NULL)
            {
                cout << temp->s << endl;
                temp = temp->next;
            }
            delete temp;
        }
        else if (where == iterate::backward)
        {
            List* temp = rear;
            while (temp != NULL)
            {
                cout << temp->s << endl;
                temp = temp->previous;
            }
            delete temp;
        }

    }
    int howMany()//function to tell the number of entries in the structure
    {
        List* temp = Front;
        int i = 0;
        while (temp != NULL)
        {
            temp = temp->next;
            i++;
        }
        delete temp;
        return i;
    }
    bool addAt(const T& ob, int pos)
    {

        if (pos == 0)
        {
            add_front(ob);
            return true;
        }

        else if ((howMany() + 1) == (pos + 1))
        {

            add_rear(ob);
            return true;
        }

        else if (pos > 0 && pos < howMany())
        {
            int i = 0;
            List* temp = Front;
            while (i <= (pos - 2))
            {
                temp = temp->next;
                i++;

            }
            //temp has the address of the preceding entry

            List* node = new List;
            node->next = temp->next;
            node->previous = temp;
            node->s = ob;

            temp->next = node;
            node->next->previous = node;
            //cout<<temp->next<<endl;

            temp = NULL;
            delete temp;
            //cout<<Front->next;
            return true;
        }
        else
        {

            cout << "\n error: wrong position\n value not added\n";
            return false;
        }
    }
    void del_front()//deleting elements from the front
    {
        if (Front == NULL)
        {
            cout << "\n error: nothing to delete";
        }
        List* temp = Front;
        if (howMany() == 1)
        {
            Front = rear = NULL;
        }
        else
        {
            Front = Front->next;
            Front->previous = NULL;
        }
        delete temp;


    }
    void del_rear()//deleting elements from the rear side
    {
        if (rear == NULL)
        {
            cout << "\n error:nothing to delete";

        }
        List* temp = rear;
        if (howMany() == 1)
        {
            rear = Front = NULL;
        }
        else
        {
            rear = rear->previous;
            rear->next = NULL;
        }
        delete temp;
    }
    void delAt(int pos)//position starts from zero
    {
        if (Front == NULL)
        {
            cout << "\n error: nothing to delete";
            return;
        }
        if (pos == 0)
        {
            del_front();
            return;
        }
        else if (howMany() - 1 == pos)
        {
            del_rear();
            return;
        }
        else if (pos > 0 && pos < howMany() - 1)
        {
            List* temp = Front;
            int i = 0;
            while (i <= (pos - 1))
            {
                temp = temp->next;
                i++;
            }
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
            temp = NULL;
            delete temp;
            return;
        }
        else
        {
            cout << "\n error: wrong position!";
        }

    }
    size_t sizeOf()
    {
        return howMany() * sizeof(s);
    }
    T& operator[](int n)
    {
        if (howMany() - 1 < n || n < 0)
        {
            cout << "\n error: index out of bounds";

        }
        else
        {
            List* temp = Front;
            int i = 0;
            while (i <= (n - 1))
            {
                temp = temp->next;
                i++;
            }
            return temp->s;
        }
    }
    void operator = (List<T> ob)//making a copy
    {
        for (int i = 0; i < ob.howMany(); i++)
        {
            this->add_rear(ob[i]);
        }


    }
    List(const List& ob)
    {
        Front = NULL;
        rear = NULL;
        List<T>* temp = ob.Front;
        while (temp != NULL)
        {
            add_rear(temp->s);
            temp = temp->next;
        }
    }
    ~List()
    {
        int temp = howMany();
        for (int i = 0; i < temp; i++)
        {
            del_front();
        }
        delete Front;
        delete rear;
    }
    void erase()
    {
        while (howMany() != 0)
        {
            this->del_rear();
        }
    }


};