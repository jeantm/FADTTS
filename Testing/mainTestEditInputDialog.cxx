#include "TestEditInputDialog.h"

int main( int argc, char *argv[] )
{
    TestEditInputDialog testEditInputDialog;
    bool test1 = testEditInputDialog.Test_UploadData( argv[1] );
    bool test2 = testEditInputDialog.Test_Delete( argv[1] );
    bool test3 = testEditInputDialog.Test_RefreshFileInfo( argv[1], argv[2] );

    std::cout << std::endl;
    if( test1 && test2 && test3 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
