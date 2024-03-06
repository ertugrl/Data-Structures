/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 18.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <map>

using namespace std;

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

void insert(map<int, Employee*>& emp_map, Employee* employee){
    emp_map[employee->get_id()] = employee; 
}

void update(map<int, Employee*>& emp_map, int id, int salary, int department){
    if (emp_map.find(id) != emp_map.end()){ 
        emp_map[id]->set_salary(salary);
        emp_map[id]->set_department(department);
    } else {
        cout << "ERROR: An invalid ID to update\n";
    }
}

void remove(map<int, Employee*>& emp_map, int id){
    if (emp_map.empty()){
        cout << "ERROR: There is no Employee\n";
    } else {
        auto my_auto = emp_map.find(id);
        if (my_auto != emp_map.end()){
            delete my_auto->second;
            emp_map.erase(my_auto);
        } else {
            cout << "ERROR: An invalid ID to delete\n";
        }
    }
}

void print_to_file(ofstream& out_file, map<int, Employee*>& emp_map){
    out_file << "Employee_ID;Salary;Department\n";

    for (auto& pair : emp_map) {
        int id = pair.first;
        Employee* employee = pair.second;
        out_file << id << ";" << employee->get_salary() << ";" << employee->get_department() << "\n";
    }
}

void remove_entire_emp(map<int, Employee*>& emp_map){
    for (auto pair: emp_map){
        delete pair.second;
    }
    emp_map.clear();
}

int highest_id = 0;

int main(int argc, char** argv){
    map<int, Employee*> emp_map;

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
        Employee* new_emp = new Employee(stoi(id), stoi(salary), stoi(department));
        insert(emp_map, new_emp);
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
            insert(emp_map, new_emp);
            // clock_t end = clock(); // finish measure
            // cout << "ADD: Map solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            // clock_t start = clock(); // start to measure
            update(emp_map, stoi(id), stoi(salary), stoi(department));
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: Map solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "DELETE"){
            getline(ss, id);
            // clock_t start = clock(); // start to measure
            remove(emp_map, stoi(id));
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: Map solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        }
    }

    // write to file
    ofstream out_file;
    out_file.open("output.csv");
    if (!out_file.is_open()){
        cout << "failed\n";
        return 1;
    }
    print_to_file(out_file, emp_map);
    remove_entire_emp(emp_map);
    return 0;
}