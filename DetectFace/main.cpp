#include <iostream>
#include "facehandler.h"

using namespace std;

int main()
{
    cout << "START UP." << endl;

    FaceHandler fh;

    while (waitKey(5) < 0) {
        fh.update();
    }

    cout << "END OF PROGRAM." << endl;
    return 0;
}
