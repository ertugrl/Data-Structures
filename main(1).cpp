#include <iostream>
#include <stdio.h>

using namespace std;

void pointer_examples();
void array_examples();
void pass_by_examples();
void pass_by_ref(int&,int&);
void pass_by_value(int, int);
void pass_by_pointer(int*, int*);
void two_dimensional_array();
void printing_fixed_sized_2d_array();
void dynamic_2d_array();
void template_function();
template<typename T>
T get_Min(T , T );

int main(){

    pointer_examples();
    array_examples();
    two_dimensional_array();
    pass_by_examples();
    template_function();
    return EXIT_SUCCESS;
}

void pass_by_examples(){
    int x = 3;
    int y = 5;

    pass_by_value(x, y);
    cout << "Outside pass by value function =>  x = " << x << " , y = " << y << endl;

    pass_by_ref(x, y);
    cout << "Outside pass by reference function =>  x = " << x << " , y = " << y << endl;

    pass_by_pointer(&x, &y);
    cout << "Outside pass by pointer function =>  x = " << x << " , y = " << y << endl;

}

void pass_by_value(int a, int b){
    int temp = b;
    b = a;
    a = temp;
    cout << "Inside pass by value function =>  x = " << a << " , y = " << b << endl;
}

void pass_by_ref(int& a, int& b){
   int temp = b;
   b = a;
   a = temp;
   cout << "Inside pass by reference function =>  x = " << a << " , y = " << b << endl;
}

void pass_by_pointer(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
    cout << "Inside pass by pointer function =>  x = " << *a << " , y = " << *b << endl;
}


void array_examples(){
    // Defining of an array with fixed size and without size
    int arr[5] = {10, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};

    // Define an integer pointer to arr
    int* ptr = arr;
    // Printing address of the array using array name
    cout << "Memory address of arr: " << &arr << endl;
    // Printing address of the array using ptr
    cout << "Memory address of arr: " << ptr << endl;

    cout << "First element: " << *ptr << endl;
    cout << "Second element: " << *(ptr+1) << endl;
    cout << "Third element: " << *(ptr+2) << endl;
    cout << "Fourth element: " << *(ptr+3) << endl;
    cout << "Fifth element: " << *(ptr+4) << endl;

    // Printing the elements of the array with pointers
    // ptr[2] ==> *(ptr+2)

    cout << "First element: " << 0[ptr] << endl;
    cout << "Second element: " << ptr[1] << endl;
    cout << "Third element: " << ptr[2] << endl;
    cout << "Fourth element: " << ptr[3] << endl;
    cout << "Fifth element: " << 4[ptr] << endl;

    // Unconventional syntax of array indexing
    // DO NOT USE THIS IN THE HOMEWORKS AND EXAMS
    

    // Traverse the array using for loop with pointer
    for (int i = 0; i < 5; i++) {
        cout << ptr[i] << "";
    }
    cout << endl;
    
    // Dynamically allocating memory for 10 integers
    int* ptr2 = new int[10];

    // Filling the array
    for (int i = 0; i < 10; i++) {
        ptr2[i] = 10 - i;
    }
    
    // Printing the array elements
    for (int i = 0; i < 10; i++) {
        cout << ptr2[i] << " ";
    }
    cout << endl;
    

    // Releasing memory pointed by pointer-variable
    delete[] ptr2;

}

void pointer_examples(){
    // Initiazlize an integer, pointer and reference to this integer variable
    int x = 3;
    int* ptr;
    ptr = &x;
    int &ref = x;
    //cout << x << " " << ptr << " " << ref << " " << *ptr << " " << &ref << endl;

    // Reassign the pointer
    (*ptr) = 15;
    // &ref = 15; Reassigning of a reference is not possible.
    int &q = ref;
    // Printing the memory addresses and varibles
    // A pointer has its own memory address and size on the stack,
    // whereas a reference shares the same memory address with the original variable and takes up no space on the stack.
    cout << "x: \t" << x << endl;
    cout << "&x: \t" << &x << endl;
    cout << "ptr: \t" << ptr << endl;
    cout << "*ptr: \t" << *ptr << endl;
    cout << "ref: \t" << ref << endl;
    cout << "&ref: \t" << &ref << endl;

    // Showing pointer can be assigned to NULL
    int* nullPTR = NULL;
    //int &nullRef = NULL;


    // Initiazlize a pointer with new operator
    double* ptr2 = new double;
    cout << "ptr2: \t" << ptr2 << endl;
    cout << "ptr2 +1: \t" << ptr2+1 << endl;

    *ptr2 = 100;
    cout << "*ptr2: \t" << *ptr2 << endl;
    cout << "(*ptr2) +1: \t" << (*ptr2)+1 << endl;
    // Deallocation
}

void two_dimensional_array(){
    printing_fixed_sized_2d_array();
    dynamic_2d_array();
}

void printing_fixed_sized_2d_array(){
    int certain_sized_array[5][2] = {
                                    {1, 5},
                                    {3, 2},
                                    {7, 4},
                                    {2, 9},
                                    {6, 3}
    }; 
    // Printing the array
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            cout << certain_sized_array[i][j] << " ";
        }
        cout << endl;
    }
    

    // Initialization of a pointer that stores the address of an array with 2 elements for printing the rows
    int(*p)[2];
    for (int i = 0; i < 5; i++) {
        p = &certain_sized_array[i];
        cout << "Row " << i << ": ";
        for (int j = 0; j < 2; j++) {
            cout << "\t" << *(*p+j);
        }
        cout << endl;
    }
}

/**
 * In this function, implement a 2D dynamically allocated array.
 * Get the number of row and columns from the user.
 * Fill the array with random numbers from 0 to 9.
 * Finally, for avo,d,ng memory leaks deallocate the memory. 
*/
void dynamic_2d_array(){
    // Creating dynamic 2D array
    int** dynamic_array;
    int row, column;
    cout << "Please enter row and column numbers: ";
    cin >> row >> column;

    dynamic_array = new int*[row];
    for (int i = 0; i < row; i++) {
        dynamic_array[i] = new int[column];
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            dynamic_array[i][j] = rand()%10;
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            cout << dynamic_array[i][j] << " ";
        }
        cout << endl;
    }


    for (int i = 0; i < row; i++) {
        delete[] dynamic_array[i];
    }

    delete[] dynamic_array;
}


void template_function(){
    cout << get_Min<int>(2,9) << endl;
    cout << get_Min<double> (13.6, 6.3) << endl;
    cout << get_Min<char> ('u', 's') << endl;
}

template <typename T>
T get_Min(T x, T y){
    return (x < y) ? x : y;
}