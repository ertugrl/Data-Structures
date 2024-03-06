/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 08.01.2024 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cstdlib>

using namespace std;

const int MIN_INT = -2147483648; // -(2^31)
const int MAX_INT = 2147483647; // 2^31 - 1

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d){
            id = i;
            salary = s;
            department = d;
        }
        ~Employee(){}

        int get_id(){
            return id;
        }
        int get_salary(){
            return salary;
        }
        void set_salary(int salary){
            this->salary = salary;
        }
        int get_department(){
            return department;
        }
        void set_department(int department){
            this->department = department;
        }
};

class DoublySkipList_Node{
    private:
        Employee *data;
        DoublySkipList_Node *next;
        DoublySkipList_Node *below;
    public:
        DoublySkipList_Node(Employee *data){
            this->data = data;
            this->next = NULL;
            this->below = NULL;
        }
        void set_data(Employee* data){
            this->data = data;
        }
        Employee* get_data(){
            return this->data;
        }
        void set_next(DoublySkipList_Node* next){
            this->next = next;
        }
        void set_below(DoublySkipList_Node* below){
            this->below = below;
        }
        DoublySkipList_Node* get_next(){
            return this->next;
        }
        DoublySkipList_Node* get_below(){
            return this->below;
        }
};

class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in);
        ~DoublySkipList();
        void insert(Employee* data);
        void update(Employee* data);
        void remove(int remove_id);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);
        int flipCoin();
};

DoublySkipList::DoublySkipList(int height_in){
    // head is set to display minus infinity data and tail is set to display plus infinity data
    this->height = height_in;
    DoublySkipList_Node* tail;
    Employee* head_data = new Employee(MIN_INT, 0, 0);
    head = new DoublySkipList_Node(head_data);
    Employee* tail_data = new Employee(MAX_INT, 0, 0);
    tail = new DoublySkipList_Node(tail_data);
    head->set_next(tail);
    DoublySkipList_Node* parent;
    DoublySkipList_Node* new_head = head;
    DoublySkipList_Node* new_tail = tail;
    for (int i = 0; i < height_in - 1; i++){
        // construct head nodes
        parent = new_head;
        new_head = new DoublySkipList_Node(head_data);
        parent->set_below(new_head);

        // construct tail nodes
        parent = new_tail;
        new_tail = new DoublySkipList_Node(tail_data);
        parent->set_below(new_tail);

        // connect each level
        new_head->set_next(new_tail);
    }

}

DoublySkipList::~DoublySkipList(){
    // delete all nodes level by level
    DoublySkipList_Node* headBelow = head->get_below();
    DoublySkipList_Node* curr = head;

    while (head != NULL){
        while (curr != NULL){
            curr = curr->get_next();
            if (headBelow == NULL){
                // If the lowest level is reached, delete the data as well
                delete head->get_data();
            }
            delete head;
            head = curr;
        }
        head = headBelow;
        curr = head;
        if (headBelow != NULL){
            headBelow = headBelow->get_below();
        }
    }
}

void DoublySkipList::insert(Employee* data){
    // The nodes to be added will have only 1 employee* pointer
    DoublySkipList_Node* temp = head;
    DoublySkipList_Node* lastTemp;
    int level = flipCoin();
    int count = height;

    while (temp != NULL){
        while (temp->get_next()->get_data()->get_id() < data->get_id()){
            temp = temp->get_next();
        }
        if (count <= level){
            DoublySkipList_Node* newNode = new DoublySkipList_Node(data);
            newNode->set_next(temp->get_next());
            temp->set_next(newNode);
            if (count < level){
                lastTemp->get_next()->set_below(newNode);
            }
        }
        count--;
        lastTemp = temp;
        temp = temp->get_below();
    }
}

void DoublySkipList::update(Employee* data){
    Employee* emp = search(data->get_id());
    // if id of node to be updated does not match any node in the skip list
    if (emp == NULL){
        cout << "ERROR: An invalid ID to update\n";
        return;
    } else { // if there is a valid id
        emp->set_salary(data->get_salary());
        emp->set_department(data->get_department());
    }
}

void DoublySkipList::remove(int remove_id){
    DoublySkipList_Node* temp = head;

    // get the proper node
    while (true){
        while (temp->get_next()->get_data()->get_id() < remove_id){
            temp = temp->get_next();
        }
        if (temp->get_next()->get_data()->get_id() == remove_id){
            break;
        }
        if (temp->get_below() != NULL){
            temp = temp->get_below();
        } else {
            break;
        }
    }

    // if skip list is empty without head and tail nodes
    if (temp->get_data()->get_id() == MIN_INT && temp->get_next()->get_data()->get_id() == MAX_INT){
        cout << "ERROR: There is no Employee\n";
        return;
    }

    // if id of node to be deleted does not match any node in the skip list
    if (temp->get_next()->get_data()->get_id() != remove_id){
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    // deletion and connection process
    DoublySkipList_Node* curr;
    while (true){
        curr = temp->get_next();
        temp->set_next(curr->get_next());
        if (temp->get_below() != NULL){
            temp = temp->get_below();
            while (temp->get_next()->get_data()->get_id() != remove_id){
                temp = temp->get_next();
            }
            delete curr;
        } else {
            delete curr->get_data();
            delete curr;
            break;
        }
    }
}

Employee* DoublySkipList::search(int search_id){
    DoublySkipList_Node* temp = head;

    while (temp != NULL){
        if (temp->get_next()->get_data()->get_id() < search_id){
            temp = temp->get_next();
        } else if (temp->get_next()->get_data()->get_id() > search_id){
            temp = temp->get_below();
        } else if (temp->get_next()->get_data()->get_id() == search_id){
            // if id is found
            return temp->get_next()->get_data();
        }
    }
    // if id is not found
    return NULL;
}

void DoublySkipList::dumpToFile(ofstream& out_file){
    DoublySkipList_Node* temp = head;
    // get head's bottom node
    while (temp && temp->get_below()){
        if (temp->get_below()){
            temp = temp->get_below();
        }
    }
    temp = temp->get_next();

    out_file << "Employee_ID;Salary;Department\n";
    while (temp->get_data()->get_id() != MAX_INT){
        out_file << temp->get_data()->get_id() << ";"
                 << temp->get_data()->get_salary() << ";"
                 << temp->get_data()->get_department() << "\n";
        temp = temp->get_next();
    }
}


int DoublySkipList::flipCoin(){
    // this function determines the level of the node to be inserted
    int level = 1;
    while (rand() % 2 && level < height){
        level++;
    }
    return level;
}

int highest_id = 0;

int main(int argc, char** argv){
    int height = 20;
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator using time

    DoublySkipList* dsl = new DoublySkipList(height);

    // read csv file to add all datas to tree
    string csv_file = argv[1];

    ifstream file(csv_file); // read mode
    if (!file.is_open()){
        cout << "CSV file couldn't open";
        return 1;
    }

    string line;
    string id, salary, department;
    bool first_line = true; // to pass the first line due to titles
    while (getline(file, line)){ // navigate each line
        if(first_line){
            first_line = false;
            continue; // passed the first line
        }
        istringstream ss(line); // to render text line
        string token; // to store each values (id, salary, etc.)
        getline(ss, id, ';');
        getline(ss, salary, ';');
        getline(ss, department);
        Employee* emp = new Employee(stoi(id), stoi(salary), stoi(department));
        dsl->insert(emp); // add to new node
        highest_id++;
    }
    file.close();

    // read operations file
    string opr_fl = argv[2]; // get operation file from CLI

    ifstream opr_file(opr_fl);
    if (!opr_file.is_open()){
        cout << "Operation opr_file couldn't open";
    }
    string row; // read lines
    string command;
    while (getline(opr_file, row)){ // get line
        istringstream ss(row);
        getline(ss, command, ';'); // get operation
        if (command == "ADD"){
            getline(ss, salary, ';');
            getline(ss, department);
            // clock_t start = clock(); // start to measure
            Employee* new_emp = new Employee(++highest_id, stoi(salary), stoi(department));
            dsl->insert(new_emp);
            // clock_t end = clock(); // finish measure
            // cout << "ADD: Doubly Skip List solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            // clock_t start = clock(); // start to measure
            Employee* new_emp = new Employee(stoi(id), stoi(salary), stoi(department));
            dsl->update(new_emp);
            delete new_emp;
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: Doubly Skip List solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "DELETE"){
            getline(ss, id);
            // clock_t start = clock(); // start to measure
            dsl->remove(stoi(id));
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: Doubly Skip List solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } 
    }

    // write to file
    ofstream out_file;
    out_file.open("doubly_output.csv");
    if (!out_file.is_open()){
        cout << "failed\n";
        return 1;
    }
    dsl->dumpToFile(out_file); // print skip list to file

    // delete entire skip list with dsl's destructor
    delete dsl;
    return 0;
}