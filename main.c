// REPLACE THIS FILE WITH YOUR OWN CODE.
// READ ELEV.H FOR INFORMATION ON HOW TO USE THE ELEVATOR FUNCTIONS.

#include "elev.h"
#include "run_elevator.h"
#include <stdio.h>


int main()
{
    // Initialize hardware
    if (!elev_init()) {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }
    
    printf("Press STOP button to stop elevator and exit program.\n");

    run_elevator(); // LET THE SHIT BEGIN!!!!11oneone!!
    
    return 0;
}

