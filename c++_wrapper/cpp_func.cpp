#include <iostream>
#include "c_func.h"

<<<<<<< HEAD
int main() 
{
=======
int main() {
>>>>>>> 9e30f2a (function call from cpp function works)
    SomeStruct data;  // Initialize struct in C++

    // Input data in C++
    data.a = 10;
    data.b = 2.5;

<<<<<<< HEAD
    // Call     C function for calculations
=======
    // Call C function for calculations
>>>>>>> 9e30f2a (function call from cpp function works)
    perform_calculations(&data);

    // Output result in C++
    std::cout << "Result: " << data.c << std::endl;

<<<<<<< HEAD
    std::cout<<"\nStrings: "<<data.name[0]<<std::endl;
    std::cout<<"\nStrings: "<<data.name[1]<<std::endl;

=======
>>>>>>> 9e30f2a (function call from cpp function works)
    return 0;
}
