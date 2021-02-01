#include "../inc/master.h"

using namespace std;


class Destruction
{
    public:
        Destruction();
        Destruction(string name_prog);
        int createBatchFile();
        int kills_all_same_process();
        int startBatchFile();
        int main();
    private:
        string a_path_prog;
        string a_name_file_batch;
        string a_name_prog;

};