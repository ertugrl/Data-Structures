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

class Employee {
private:
    int id;
    int salary;
    int department;
public:
    Employee(int, int, int);
    ~Employee();
    void set_salary(int);
    void set_department(int);
    int get_id();
    int get_salary();
    int get_department();
};

Employee::Employee(int id, int sal, int dept){
    this->id = id;
    this->salary = sal;
    this->department = dept;
}

Employee::~Employee(){}

void Employee::set_salary(int salary){
    this->salary = salary;
}
void Employee::set_department(int department){
    this->department = department;
}

int Employee::get_id(){
    return id;
}
int Employee::get_salary(){
    return salary;
}
int Employee::get_department(){
    return department;
}

int line_count(string csv){ // get the number of employees
    int counter = 0;

    ifstream file(csv);
    if (!file.is_open()){
        cout << "CSV file couldn't open" << endl;
        return 1;
    }

    counter--; // the first line includes titles
    string line;
    while (getline(file, line)){
        counter++;
    }

    file.close();
    return counter;
}

void add_emp(Employee**& emp_arr, int salary, int department, int& size, int& highest_id){
    int new_size = size + 1;
    Employee** new_arr = new Employee*[new_size];

    int emp_id;
    int emp_salary;
    int emp_department;
    // copy old array to new array
    for (int index = 0; index < size; index++){
        emp_id = emp_arr[index]->get_id();
        emp_salary = emp_arr[index]->get_salary();
        emp_department = emp_arr[index]->get_department();
        new_arr[index] = new Employee(emp_id, emp_salary, emp_department);
    }
    // adding
    highest_id++; // update highest_id
    new_arr[size] = new Employee(highest_id, salary, department);
    size++; // size was acquired by "pass by reference", "int&"

    // swap the array pointers
     Employee** tmp = emp_arr;
     emp_arr = new_arr;

    // delete old array
    for (int i = 0; i < (size-1); i++){
        delete tmp[i]; // Delete each allocated array containing employee information 
    }
    delete[] tmp; // Delete the array containing pointers showing employee information
}

void update_emp(Employee**& emp_arr, int id, int salary, int department, int size){
    // size doesn't change, so we don't need to pass by referance for size (int& size)
    int index;
    // If i takes the last value in the for loop and does not enter the if condition, it enters 'else if'
    for (int i = 0; i < size; i++){
        if (id == emp_arr[i]->get_id()){
            index = i;
            break; // get the index for valid id 
        } else if (i == size-1){ 
            cout << "ERROR: An invalid ID to update";
            return;
        }
    }
    // update
    emp_arr[index]->set_salary(salary);
    emp_arr[index]->set_department(department);
}

void delete_emp(Employee**& emp_arr, int id, int& size){
    int index;
    if (size == 0){
        cout << "ERROR: There is no Employee";
        return;
    } else {
        // If i takes the last value in the for loop and does not enter the if condition, it enters 'else if'
        for (int i = 0; i < size; i++){
            if (id == emp_arr[i]->get_id()){
                index = i;
                break; // get the index for valid id 
            } else if (i == size-1){
                cout << "ERROR: An invalid ID to delete";
                return;
            }
        }
    }

    Employee** new_arr = new Employee*[size-1];
    int emp_id;
    int emp_salary;
    int emp_department;
    // copy old array to new array
    int j = 0;
    for (int i = 0; i < size; i++){
        if(i == index){
            continue; // pass the employee desired to be deleted
        }
        /*
        While assigning values to the array sequentially, when the id to be deleted comes across,
        the j value does not increase so that there is no space in the array.
        */
        emp_id = emp_arr[i]->get_id();
        emp_salary = emp_arr[i]->get_salary();
        emp_department = emp_arr[i]->get_department();
        new_arr[j] = new Employee(emp_id, emp_salary, emp_department);
        j++;
    }
    // swap the array pointers
    Employee** tmp = emp_arr;
    emp_arr = new_arr;

    // delete old array
    for (int i = 0; i < size; i++){
        delete tmp[i]; // Delete each allocated array containing employee information 
    }
    delete[] tmp; // Delete the array containing pointers showing employee information

    // update size
    size--;
}

int main(int argc, char** argv){
    // size is the number of CURRENT employees 
    int size = line_count(argv[1]);
    // highest_id is the last id (even if the employee with the last id is deleted, highest_id DOES NOT CHANGE due to forward adding operations)
    int highest_id = size;
    
    string csv_file = argv[1];
    ifstream file(csv_file); // read mode
    if (!file.is_open()){
        cout << "CSV file couldn't open" << endl;
        return 1;
    }
    Employee** emp_arr = new Employee*[size];
    string line;
    bool first_line = true; // to pass the first line due to titles
    int current_index = 0; // the current index in Employee array
    while (getline(file, line)){
        if(first_line){
            first_line = false;
            continue; // passed the first line
        }

        istringstream ss(line); // to render text line
        string token; // to store each values (id, salary, etc.)
        int values[3]; // to temporarily store data as integer 
        int index = 0;

        while (getline(ss, token, ';')){
            values[index] = stoi(token);
            index++;
        } // exp: value[3] = {id, salary, department}

        emp_arr[current_index] = new Employee(values[0], values[1], values[2]);
        current_index++;
    }
    file.close();


    // operations
    string opr_fl = argv[2]; // get operation file from CLI
    ifstream opr_file(opr_fl);
    if (!opr_file.is_open()){
        cout << "Operation opr_file couldn't open" << endl;
    }

    string row; // read lines
    string command;
    string id, salary, department;
    while (getline(opr_file, row)){ // get line
        istringstream ss(row);
        getline(ss, command, ';'); // get operation

        if (command == "ADD"){ // "ADD" OPERATION
            getline(ss, salary, ';');
            getline(ss, department, ';');
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            add_emp(emp_arr, stoi(salary), stoi(department), size, highest_id);
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "ADD: array solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
            
            // total line updated (size++)
            // highest id updated (highest_id)

        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department, ';');
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            update_emp(emp_arr, stoi(id), stoi(salary), stoi(department), size);
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: array solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";

        } else if (command == "DELETE"){
            getline(ss, id);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            delete_emp(emp_arr, stoi(id), size);
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: array solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        }
    }

    ofstream out_file;
    out_file.open("array_solution.csv");
    if (!out_file.is_open()){
        cout << "Failed" << "\n";
        return 1;
    }
    out_file << "Employee_ID;Salary;Department\n";  
    for (int i = 0; i < size; i++){
        out_file << emp_arr[i]->get_id() << ";" << emp_arr[i]->get_salary() << ";" << emp_arr[i]->get_department() << "\n"; 
    }
    out_file.close();


    for (int i = 0; i < size; i++) { // deleting
        delete emp_arr[i];
    }
    delete[] emp_arr;

    return 0;
}