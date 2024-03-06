/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 10.11.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
        Employee* next;
    public:
        Employee(int, int, int);
        ~Employee();
        int get_id();
        int get_salary();
        void set_salary(int);
        int get_department();
        void set_department(int);
        void set_next(Employee*);
        Employee* get_next();
};

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
    this->next = NULL;
}
Employee::~Employee(){}

int Employee::get_id(){
    return this->id;
}

int Employee::get_salary(){
    return this->salary;
}

void Employee::set_salary(int salary){
    this->salary = salary;
}

int Employee::get_department(){
    return this->department;
}

void Employee::set_department(int department){
    this->department = department;
}

Employee* Employee::get_next(){
    return this->next;
}

void Employee::set_next(Employee* next){
    this->next = next;
}

// --------------------

class SingleLinkedList{
    private:
        Employee* head;
        Employee* tail;
        Employee* search(int);
    public:
        SingleLinkedList();
        ~SingleLinkedList();
        void add_emp(int, int, int);
        void update_emp(int, int, int);
        void remove_emp(int);
        bool contains(int);
        void write_to_file();
        void empty_list();
};

SingleLinkedList::SingleLinkedList(){
    head = NULL;
    tail = NULL;
}

SingleLinkedList::~SingleLinkedList(){}

bool SingleLinkedList::contains(int id){
    Employee* p = head;
    while (p != NULL && id != p->get_id()){
        p = p->get_next();
    }
    if (p == NULL){
        return false;
    } else {
        return true;
    }
}

Employee* SingleLinkedList::search(int updt_id){
    /*
    we don't need to check whether search function can find the id to be update
    because we already control this situation with the "contains" function
    */
    Employee* prev = head;
    if (updt_id == head->get_id()){ // if id matches first employee's id
        return NULL;
    } else {
        while (prev->get_next()){
            if (updt_id == prev->get_next()->get_id()){
                return prev;
            }
            prev = prev->get_next();
        }
        return prev; // conflicting with contains()'s result, this won't happen
    }
}

void SingleLinkedList::add_emp(int id, int salary, int department){
    Employee* new_emp = new Employee(id, salary, department);
    if (head == NULL){ // when the list is empty
        head = new_emp;
        tail = new_emp;
    } else { // add to tail (end)
        tail->set_next(new_emp);
        tail = new_emp;
    }
}

void SingleLinkedList::update_emp(int id, int salary, int department){
    if (!contains(id)){
        cout << "ERROR: An invalid ID to update";
        return;
    } else {
        Employee* prev = this->search(id);
        if (prev == NULL){ // related to search func's implementation
            head->set_salary(salary);
            head->set_department(department);
        } else {
            prev->get_next()->set_salary(salary);
            prev->get_next()->set_department(department);
        }
    }
}

void SingleLinkedList::remove_emp(int id){
    if (head == NULL){ // if linked list is empty
        cout << "ERROR: There is no Employee";
        return;
    } else {
        if (!contains(id)){ // if id don't match any employee's id
            cout << "ERROR: An invalid ID to delete";
            return;
        } else {
            Employee* prev = search(id);
            if (prev == NULL){ // if first employee will be deleted (head)
                prev = head->get_next();
                head->set_next(NULL);
                delete head;
                head = prev;
            } else if (prev->get_next() == tail){ // if last employee will be deleted (tail)
                prev->set_next(NULL);
                delete tail;
                tail = prev;
            } else { // if any employee between the first and last ones will be deleted
                Employee* cur = prev->get_next();
                prev->set_next(cur->get_next());
                cur->set_next(NULL);
                delete cur;
            }
        }
    }
}

void SingleLinkedList::write_to_file(){
    ofstream out_file;
    out_file.open("linked_list_solution.csv");
    if (!out_file.is_open()){
        cout << "failed\n";
        return;
    }
    out_file << "Employee_ID;Salary;Department\n";
    
    Employee* tmp = head;
    while (tmp != NULL){
        out_file << tmp->get_id() << ";" << tmp->get_salary() << ";" << tmp->get_department() << "\n";
        tmp = tmp->get_next();
    }
    out_file.close();
}

void SingleLinkedList::empty_list(){
    Employee* tmp = head;
    while (tmp != NULL){
        tmp = tmp->get_next();
        head->set_next(NULL);
        delete head;
        head = tmp;
    }
}

// --------------------

int highest_id = 0;

int main(int argc, char** argv){
    SingleLinkedList* sll = new SingleLinkedList();
    
    // read csv file to add all datas to linked list
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
        sll->add_emp(stoi(id), stoi(salary), stoi(department)); // add to new node
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
        if (command == "ADD"){ // "ADD" OPERATION
            getline(ss, salary, ';');
            getline(ss, department);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            sll->add_emp(++highest_id, stoi(salary), stoi(department));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "ADD: linked list solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
            
            // ++highest_id: first increase id by 1, then go into function

        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            sll->update_emp(stoi(id), stoi(salary), stoi(department));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: linked list solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";

        } else if (command == "DELETE"){
            getline(ss, id);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            sll->remove_emp(stoi(id));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: linked list solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        }
    }

    // get linked list's datas
    sll->write_to_file();
    sll->empty_list();
    delete sll;
    return 0;
}