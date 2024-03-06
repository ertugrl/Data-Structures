/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 29.10.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void add_emp(string, int, int, int&);
void update_emp(string, int, int, int);
void delete_emp(string, int);
int line_size(string);

int main(int argc, char** argv){
    string csv_file = argv[1];
    int last_id = line_size(csv_file);

    // read operations file
    ifstream opr_file(argv[2]);
    if (!opr_file.is_open()){
        cout << "Operation file couldn't open\n";
    }

    string row;
    string command;
    string id, salary, department;
    while (getline(opr_file, row)){ // get line
        istringstream ss(row);
        getline(ss, command, ';'); // get command
        if (command == "ADD"){
            getline(ss, salary, ';');
            getline(ss, department);
            add_emp(csv_file, stoi(salary), stoi(department), last_id);
        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            update_emp(csv_file, stoi(id), stoi(salary), stoi(department));
        } else if (command == "DELETE"){
            getline(ss, id);
            delete_emp(csv_file, stoi(id));
        }
    }

    return 0;
}


int line_size(string csv){ // get the number of employees
    int line_count = 0;

    ifstream file(csv);

    if (!file.is_open()){
        cout << "CSV file couldn't open\n";
        return 1;
    }

    string line;
    while (getline(file, line)){
        line_count++;
    }
    line_count--; // the first line includes titles

    file.close();
    return line_count;
}

void add_emp(string csv_file, int salary, int department, int& last_id){
    // open the file
    ofstream fl(csv_file, ios::app); 
    if (!fl.is_open()){
        cout << "Failed to open the file.\n";
        return;
    }

    int new_id = last_id + 1; // new_id: id to be added

    fl << new_id << ";" << salary << ";" << department << "\n"; // write new line

    fl.close();
    last_id++; // update the last employee's id (with pass by reference)
}

void update_emp(string csv_file, int id, int salary, int department){
    // open the csv file in read mode
    ifstream file_in(csv_file);
    if(!file_in.is_open()){
        cout << "Failed to open the file.\n";
        return;
    }

    // create a temporary file
    string tmp_file = "tmp_file.csv";
    ofstream file_out(tmp_file);
    if (!file_out.is_open()){
        cout << "Failed to open the file.\n";
        file_in.close();
        return;
    }
    
    bool is_id_found = false; // check whether the id exists
    int id_tmp, salary_tmp, department_tmp;
    string line;
    string value;

    getline(file_in, line);
    file_out << line << "\n"; // write titles to temporary file
    
    // get each line from current file
    while (getline(file_in, line)){ // start directly with information (titles was passed above)
        istringstream ss(line);
        getline(ss, value, ';');
        id_tmp = stoi(value); // get line's id value

        if(id_tmp == id){ // if id values match...
            salary_tmp = salary;
            department_tmp = department;
            is_id_found = true; 
            // ... use these values
        } else {
            // otherwise, use these
            getline(ss, value, ';');
            salary_tmp = stoi(value);

            getline(ss, value);
            department_tmp = stoi(value); 
        }

        // create new line and write to temporary file
        file_out << id_tmp << ";" << salary_tmp << ";" << department_tmp << "\n";
    }

    file_in.close();
    file_out.close();

    if (is_id_found){ // if id was found
        remove(csv_file.c_str()); // remove old file
        rename(tmp_file.c_str(), csv_file.c_str()); // change the name of the new file to the old one
    } else { // was not found
        remove(tmp_file.c_str()); // remove new file (both of two files are the same due to not updating                    )
        cout << "ERROR: An invalid ID to update\n";
    }
}

void delete_emp(string csv_file, int id){
    // open the csv file in read mode
    ifstream file_in(csv_file);
    if(!file_in.is_open()){
        cout << "Failed to open the file.\n";
        return;
    }
    // create a temporary file
    string tmp_file = "tmp_file.csv";
    ofstream file_out(tmp_file);

    if (!file_out.is_open()){
        cout << "Failed to open the file.\n";
        file_in.close();
        return;
    }
    
    bool is_id_found = false; // check whether the id exists
    int id_tmp, salary_tmp, department_tmp;
    string line;
    string value;

    getline(file_in, line);
    file_out << line << "\n"; // write titles to temporary file
    
    // get each line from current file
    while (getline(file_in, line)){ // start directly with information (titles was passed above)
        istringstream ss(line);
        getline(ss, value, ';');
        id_tmp = stoi(value); // get line's id value

        if (id_tmp == id){ // if id values match
            is_id_found = true;
            continue; // pass this line which we want to delete
        } else {
            getline(ss, value, ';');
            salary_tmp = stoi(value);

            getline(ss, value);
            department_tmp = stoi(value); 
        }

        // create new line and write to temporary file
        file_out << id_tmp << ";" << salary_tmp << ";" << department_tmp << "\n";
    }

    file_in.close();
    file_out.close();

    if (is_id_found){ // if id was found
        remove(csv_file.c_str()); // remove old file
        rename(tmp_file.c_str(), csv_file.c_str()); // change the name of the new file to the old one
    } else { // was not found
        remove(tmp_file.c_str()); // remove new file (both of two files are the same due to not updating
        cout << "ERROR: There is no Employee\n";
    }
}