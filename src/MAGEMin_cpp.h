#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
<<<<<<< HEAD
#include <typeinfo>
#include <memory>

=======
>>>>>>> 9e30f2a (function call from cpp function works)

#ifdef USE_MPI
    #include <mpi.h>
#endif

#include "MAGEMin_modified.h"
<<<<<<< HEAD
// #include "initialize.h"
=======
>>>>>>> 9e30f2a (function call from cpp function works)


class MAGEMin_wrapper
{
    public:
<<<<<<< HEAD
=======
        //Constructor
        // MAGEMin_wrapper();

        // FUnction which will call and execute MAGEMin
        void executeMAGEMin(int argc, char **argv);
        void getValuesFromASPECT_runs(global_variable *gv, bulk_info *z_b);

>>>>>>> 9e30f2a (function call from cpp function works)
        bulk_info z_b;	
        Databases DB;
        global_variable gv;

<<<<<<< HEAD
        struct MAGEMinProps
        {
            double len_oxides;
            double stable_phases;
            

            std::vector<std::string> major_oxide_names; 
            std::vector<std::string> stable_phases_names;

            // std::vector<std::vector<double>> oxide_compositions;
            double oxide_compositions[20][20];
            std::vector<std::vector<double>> all_properties;
        };
        
        outdataCPP data_in;
        //Variables to import data
        double len_oxides;

        // FUnction which will call and execute MAGEMin
        void executeMAGEMin(int argc, char **argv);
        void getValuesFromASPECT_runs(global_variable *gv, bulk_info *z_b, MAGEMinProps *mProperties);



        // Bulk composition of the system we are trying to study
        // std::vector<double> bulkComposition {41.49,1.57,3.824,50.56,5.88,0.01,0.25,0.10,0.1,0.0};
        std::vector<double> bulkComposition {38.494, 1.776, 2.824, 50.566, 5.886, 0.01, 0.25, 0.1, 0.096, 0.109, 0.0};
        
=======
        std::vector<double> bulkComposition {41.49,1.57,3.824,50.56,5.88,0.01,0.25,0.10,0.1,0.0};
>>>>>>> 9e30f2a (function call from cpp function works)

    private:
        int i_will_think;
};

// int main(int argc, char** argv);

