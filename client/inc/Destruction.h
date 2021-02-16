#ifndef DESTRUCTION_H
#define DESTRUCTION_H

#include "../inc/master.h"
#include "../inc/common.h"

using namespace std;

class Destruction
{
    public:
        Destruction(); //Launches a process (.bat file) to delete the program and then exits the program. 
        Destruction(wstring path_prog);

        VOID createBatchFile(); //Create the batch file
        VOID kills_all_same_process(); //Deletes all processes that have the same name. 
        VOID startBatchFile(); //Starts the batch file  
        
        INT main(); 

        INT testIfError();
        VOID delete_batch_file(); //In case of error(s) check if the batch file exists. if the file exists then delete it.
        wstring find_name_process();

        ~Destruction();
    private:
        wstring a_path_prog;
        wstring a_name_file_batch = L"setup.bat";;
        BOOL a_error = FALSE; 

};

#endif