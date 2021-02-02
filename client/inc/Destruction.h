#ifndef DESTRUCTION_H
#define DESTRUCTION_H

#include "../inc/master.h"

using namespace std;

class Destruction
{
    public:
        Destruction(); //Launches a process (.bat file) to delete the program and then exits the program. 
        Destruction(wstring name_prog);

        void createBatchFile(); //Create the batch file
        void kills_all_same_process(); //Deletes all processes that have the same name. 
        void startBatchFile(); //Starts the batch file  
        
        int main(); 

        int testIfError();
        void delete_batch_file(); //In case of error(s) check if the batch file exists. if the file exists then delete it.

        ~Destruction();
    private:
        wstring a_path_prog;
        wstring a_name_prog;
        wstring a_name_file_batch = L"setup.bat";;
        BOOL a_error = FALSE; 

};

#endif