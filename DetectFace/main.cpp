#include <iostream>
#include "facehandler.h"

using namespace std;

int main()
{
    cout << "START UP." << endl;

    FaceHandler fh;

    bool running = true;
    while (running) {
        fh.update();
        switch(waitKey(5)) {
            case 27:  // ESCAPE
                running = false;
                break;
            case 'd':
                fh.toogle_debug_graphics();
                break;
        }
    }

    cout << "END OF PROGRAM." << endl;
    return 0;
}
