#include "types.h"
#include "stat.h"
#include "user.h"
/*
void ps(void)
{
    printf(1,"ps\n");
    return;
}*/
/*
void yield(void)
{
    printf(1,"yield\n");
    return;
}
*/
void test_ps()
{
    int pid = getpid();
    int i, j;
    double x = 0, z;
    printf(1, "1st pid: %d\n", pid);    
    pid = fork();
    if(pid == 0){   //child
        for(i = 0; i < 2; i++){
            for ( z = 0; z < 3000000.0; z += 0.1 )
                x =  x + 3.14 * 89.64;
            //ps();
			//yield();
        }   
        exit();
    } else {    //parent
        for(j = 0; j < 2; j++){
            for ( z = 0; z < 3000000.0; z += 0.1 )
                x =  x + 3.14 * 89.64;
            //ps();
			//yield();
        }   
        wait();
    }   
}

int main(int argc, char **argv)
{
        printf(1, "=== TEST START ===\n");
        test_ps();
        printf(1, "=== TEST   END ===\n");

        exit();

}

