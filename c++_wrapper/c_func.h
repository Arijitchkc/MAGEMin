#ifdef __cplusplus
extern "C" {
#endif


typedef struct SomeStruct{
    int a;
    float b;
    double c;
<<<<<<< HEAD
    char name[2][20];
=======
>>>>>>> 9e30f2a (function call from cpp function works)
} SomeStruct;



// Function declarations of the C code
void perform_calculations(SomeStruct *data);

#ifdef __cplusplus
}
#endif
