/* @Author
Student Name : Ertuğrul Şahin
StudentID : 150210028
Date : 18.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <queue>
#include <stack>

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

// ##############################################

class Node{
    private:
        Employee *employee;
        Node *left;
        Node *right;
    public:
        Node(Employee *employee){
            this->employee = employee;
            this->left = NULL;
            this->right = NULL;
        }
        ~Node(){
            if (this->employee != NULL){
                delete employee;
            }
        }
        Employee* get_employee(){
            return this->employee;
        }
        void set_employee(Employee* employee){
            this->employee = employee;
        }
        void set_left(Node* left){
            this->left = left;
        }
        void set_right(Node* right){
            this->right = right;
        }
        Node* get_left(){
            return this->left;
        }
        Node* get_right(){
            return this->right;
        }
};

// ##############################################

class BinaryTree{
    private:
        Node *root;
    public:
        BinaryTree();
        void insert(Employee* employee);
        void update(Employee* employee);
        void remove(int id);
        Node* findMinParent(Node* current);
        Employee *search(int id);
        void printToFile(ofstream &output_file);
        void delete_tree();
};

BinaryTree::BinaryTree(){
    this->root = NULL;
}

void BinaryTree::insert(Employee* employee){
    Node* new_node = new Node(employee);
    if (root == NULL){ // if root is null
        root = new_node;
        return;
    } else { // if root is not null
        Node* current = root;
        Node* parent = NULL;
        int new_id, cur_id;
        new_id = new_node->get_employee()->get_id();
        while (true){ // find suitable location for new employee
            parent = current;
            cur_id = current->get_employee()->get_id();
            if (new_id < cur_id){
                current = current->get_left();
                if (current == NULL){
                    parent->set_left(new_node);
                    return;
                }
            } else if (new_id > cur_id){
                current = current->get_right();
                if (current == NULL){
                    parent->set_right(new_node);
                    return;
                }
            }
        }
    }
}

void BinaryTree::update(Employee* employee){
    Employee* emp = search(employee->get_id());
    if (emp == NULL){
        cout << "ERROR: An invalid ID to update\n";
        return;
    } else { // if there is a valid id
        emp->set_salary(employee->get_salary());
        emp->set_department(employee->get_department());
    }
}

void BinaryTree::remove(int id){
    if (root == NULL){ // if tree is empty
        cout << "ERROR: There is no Employee\n";
        return;
    } else {
        Employee* emp = search(id);
        if (emp == NULL){ // if id don't match any employee's id
            cout << "ERROR: An invalid ID to delete\n";
            return;
        } else {
            // if id to be deleted exists in the tree
            Node* current = root;
            Node* parent = NULL;
            int cur_id;
            while (true){ // set current and parent
                parent = current;
                cur_id = current->get_employee()->get_id();
                if (id < cur_id){
                    current = current->get_left();
                } else if (id > cur_id){
                    current = current->get_right();
                }
                if (id == current->get_employee()->get_id()){
                    break;
                }
            }
            // after obtaining proper current and parent
            if (current == root){ // if node to be deleted is root
                if (current->get_left() == NULL && current->get_right() == NULL){ // if root has no child
                    root = NULL;
                    delete current;
                } else if (current->get_right() == NULL && current->get_left() != NULL){
                    root = root->get_left();
                    current->set_left(NULL);
                    delete current;
                } else if (current->get_left() == NULL && current->get_right() != NULL){
                    root = root->get_right();
                    current->set_right(NULL);
                    delete current;
                } else { // if root has two children
                    Node* tmp = root->get_right();
                    if (root->get_right()->get_left() == NULL){
                    // if the root of the right tree of the root has no left child
                        delete root->get_employee();
                        root->set_employee(tmp->get_employee());
                        root->set_right(tmp->get_right());
                        tmp->set_right(NULL); // left is already NULL
                        tmp->set_employee(NULL); // since tmp's and root's employee addresses are the same
                        delete tmp; 
                    } else {
                        tmp = findMinParent(tmp);
                        delete root->get_employee();
                        root->set_employee(tmp->get_left()->get_employee());
                        current = tmp->get_left();
                        tmp->set_left(current->get_right());
                        current->set_right(NULL);
                        current->set_employee(NULL); // since current's and root's employee addresses are the same
                        delete current;
                    }
                }
            } else { // // if node to be deleted is not root
                if (current->get_left() == NULL && current->get_right() == NULL){
                    if (parent->get_left() == current){
                        parent->set_left(NULL);
                    } else if (parent->get_right() == current){
                        parent->set_right(NULL);
                    }
                    delete current;
                } else if (current->get_right() == NULL && current->get_left() != NULL){
                    if(current == parent->get_left()){
                        parent->set_left(current->get_left());
                        current->set_left(NULL);
                        delete current;
                    } else {
                        parent->set_right(current->get_left());
                        current->set_left(NULL);
                        delete current;
                    }
                } else if (current->get_left() == NULL && current->get_right() != NULL){
                    if(current == parent->get_right()){
                        parent->set_right(current->get_right());
                        current->set_right(NULL);
                        delete current;
                    } else {
                        parent->set_left(current->get_right());
                        current->set_right(NULL);
                        delete current;
                    }
                } else { // if the node to be deleted has two children
                    // min node in right tree method
                    Node* tmp = current->get_right();
                    if (current->get_right()->get_left() == NULL){
                    // if the root of the right tree of the node to be deleted has no left child
                        delete current->get_employee();
                        current->set_employee(tmp->get_employee());
                        current->set_right(tmp->get_right());
                        tmp->set_right(NULL); // left is already NULL
                        tmp->set_employee(NULL);
                        delete tmp;
                    } else {
                        tmp = findMinParent(tmp);
                        delete current->get_employee();
                        current->set_employee(tmp->get_left()->get_employee());
                        current = tmp->get_left();
                        tmp->set_left(current->get_right());
                        current->set_right(NULL);
                        current->set_employee(NULL); 
                        delete current;
                    }
                }
            }
        }
    }
}

Node* BinaryTree::findMinParent(Node* current){
    // this function is used to get leftmost node's parent
    while (current->get_left() != NULL && current->get_left()->get_left() != NULL){
        current = current->get_left();
    }
    return current;
}

Employee* BinaryTree::search(int id){
    if (root == NULL){
        return NULL;
    }
    Node* current = root;
    int cur_id;
    while(current != NULL){ // Scans the entire tree by visiting the necessary places
        cur_id = current->get_employee()->get_id();
        if (id < cur_id){
            current = current->get_left();
        } else if (id > cur_id){
            current = current->get_right();
        } else { // if id = current id (if id is found)
            return current->get_employee();
        }
    }
    // if id is not found
    return NULL;
}

void BinaryTree::printToFile(ofstream &output_file){
    // inorder traversal with stack
    if (root == NULL){
        return;
    }
    stack<Node*> node_stack;
    Node* current = root;

    while (!node_stack.empty() || current != NULL){
        while(current != NULL){
            node_stack.push(current);
            current = current->get_left();
        }
        current = node_stack.top();
        node_stack.pop();
        output_file << current->get_employee()->get_id() << ";"
                    << current->get_employee()->get_salary() << ";"
                    << current->get_employee()->get_department() << "\n";
        
        current = current->get_right();
    }
}

void BinaryTree::delete_tree(){
    if (root == NULL) {
        cout << "Tree is already empty" << endl;
        return;
    }
    stack<Node*> node_stack;
    node_stack.push(root); // first, add root to stack

    while (!node_stack.empty()) { // continue until stack is empty
        Node* current = node_stack.top();
        node_stack.pop();
        if (current->get_right() != NULL) { // if exists, get right child 
            node_stack.push(current->get_right());
        }
        if (current->get_left() != NULL) { // if exists, get left child
            node_stack.push(current->get_left());
        }
        delete current;
    }
    root = NULL;
}

int tree_index(vector<BinaryTree*>& BSTs, int id, int& highest_index){
    /* this function is used for addition.
    The goal is to get existing index or
    generate new tree and then get its index.
    
    div <= id => return div
    div > id => push back until div, return div

    The reason for this is to arrange the trees in order in the vector.
    */
    int div = id / 5000;
    if (div > highest_index){
        for (int i = highest_index + 1; i <= div; i++){
            BinaryTree* new_bst = new BinaryTree();
            BSTs.push_back(new_bst);
        }
        highest_index = div;
        return div;
    }
    return div;
}

int find_index(int id, int& highest_index){
    // only find the index of proper tree
    int div = id / 5000;
    if (div > highest_index){
        return -1;
    } else {
        return div;
    }
}

int highest_id = 0;
int highest_index = -1; // keep max tree index

int main(int argc, char** argv){
    vector<BinaryTree*> BSTs; 

    // read csv file to add all datas to BSTs (binary search trees)
    string csv_file = argv[1];

    ifstream file(csv_file); // read mode
    if (!file.is_open()){
        cout << "CSV file couldn't open";
        return 1;
    }

    int nth_tree; // To specify which tree x belongs to
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
        nth_tree = tree_index(BSTs, stoi(id), highest_index);
        BSTs[nth_tree]->insert(emp); // add to new node
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
            nth_tree = tree_index(BSTs, highest_id, highest_index);
            BSTs[nth_tree]->insert(new_emp);
            // clock_t end = clock(); // finish measure
            // cout << "ADD: Hybrid solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "UPDATE"){
            getline(ss, id, ';');
            getline(ss, salary, ';');
            getline(ss, department);
            // clock_t start = clock(); // start to measure
            nth_tree = find_index(stoi(id), highest_index); // find tree's index
            if (nth_tree != -1){
                Employee* new_emp = new Employee(stoi(id), stoi(salary), stoi(department));
                BSTs[nth_tree]->update(new_emp);
                delete new_emp;
            } else {
                cout << "ERROR: An invalid ID to update\n";
            }
            // clock_t end = clock(); // finish measure
            // cout << "UPDATE: Hybrid solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        } else if (command == "DELETE"){
            getline(ss, id);
            // clock_t start = clock(); // start to measure
            nth_tree = find_index(stoi(id), highest_index); // find tree's index
            if (nth_tree != -1){
                BSTs[nth_tree]->remove(stoi(id)); 
            } else {
                cout << "ERROR: An invalid ID to delete\n";
            }
            // clock_t end = clock(); // finish measure
            // cout << "DELETE: Hybrid solution " << (double)(end - start) * 1000 / CLOCKS_PER_SEC << " miliseconds\n";
        }
    }

    // write to file
    ofstream out_file;
    out_file.open("output.csv");
    if (!out_file.is_open()){
        cout << "failed\n";
        return 1;
    }
    out_file << "Employee_ID;Salary;Department\n"; // add titles
    for (BinaryTree* bst : BSTs){ // get each element (BinaryTree*)
        bst->printToFile(out_file); // write each tree
        bst->delete_tree(); // delete each tree
        delete bst; // delete each tree pointer
    }
    return 0;
}