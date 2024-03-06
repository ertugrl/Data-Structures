/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 10.11.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

class Employee {
private:
    int id;
    int salary;
    int department;
public:
    Employee(int, int, int);
    void set_salary(int);
    void set_department(int);
    int get_id();
    int get_salary();
    int get_department();
};

class EmployeeController{
    private:
        vector<Employee> employees;
    public:
        void add_emp(int, int, int);
        void update_emp(int, int, int);
        void remove_emp(int);
        bool contains(int);
        void write_to_file();
};

Employee::Employee(int id, int sal, int dept){
    this->id = id;
    this->salary = sal;
    this->department = dept;
}

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

// --------------------

bool EmployeeController::contains(int id){
    for (Employee& emp : employees){
        if (emp.get_id() == id){
            return true;
        }
    }
    return false;
}

void EmployeeController::add_emp(int id, int salary, int department){
    Employee employee(id, salary, department);
    employees.push_back(employee);
}
void EmployeeController::update_emp(int id, int salary, int department){
    if (!contains(id)){
        cout << "ERROR: An invalid ID to update";
        return;
    } else {
        for (Employee& emp : employees){
            if (emp.get_id() == id){
                emp.set_salary(salary);
                emp.set_department(department);
                return;
            }
        }
    }
}

void EmployeeController::remove_emp(int id){
    if (employees.empty()){
        cout << "ERROR: There is no Employee";
    }

    if (!contains(id)){
        cout << "ERROR: An invalid ID to delete";
        return;
    } else {
        for (auto emp = employees.begin(); emp != employees.end(); ++emp) {
            if (emp->get_id() == id) {
                employees.erase(emp);
                return;
            }
        }
    }
}

void EmployeeController::write_to_file(){
    ofstream out_file;
    out_file.open("vector_solution.csv");
    if (!out_file.is_open()){
        cout << "Failed" << "\n";
        return;
    }
    out_file << "Employee_ID;Salary;Department\n";  
    for (Employee emp : employees){
        out_file << emp.get_id() << ";" << emp.get_salary() << ";" << emp.get_department() << "\n";
    }
    out_file.close();
}

// --------------------

int main(int argc, char** argv){
    int highest_id = 0;

    EmployeeController controller;

    string csv_file = argv[1];
    ifstream file(csv_file); // read mode
    if (!file.is_open()){
        cout << "CSV file couldn't open" << endl;
        return 1;
    }
    string line;
    string id, salary, department;
    bool first_line = true; // to pass the first line due to titles
    while (getline(file, line)){
        if(first_line){
            first_line = false;
            continue; // passed the first line
        }

        istringstream ss(line); // to render text line

        getline(ss, id, ';');
        getline(ss, salary, ';');
        getline(ss, department);

        controller.add_emp(stoi(id), stoi(salary), stoi(department));
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
            controller.add_emp(++highest_id, stoi(salary), stoi(department));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "ADD: vector solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";

            // ++highest_id: first increase id by 1, then go into function

        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            controller.update_emp(stoi(id), stoi(salary), stoi(department));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: vector solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";

        } else if (command == "DELETE"){
            getline(ss, id);
            // Execution Time Measuring !!!
            // clock_t start = clock(); // start to measure
            controller.remove_emp(stoi(id));
            // measurement time in milliseconds
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: vector solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        }
    }

    controller.write_to_file();

    return 0;
}