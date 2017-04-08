#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
/* ========================================================================
 * test procedure:
 * 1. load sleepy module
 * 2. gcc write_test.c
 * 3. sudo ./a.out 0 50 (seconds for sleeping)
 * 4. open new command window
 * 5. sudo ./a.out 2 9 (any number except 0)
 * 6. check if you see "/dev/sleepy0 woken up abnormally. Time left = 42"
 ======================================================================== */
int main( int argc, char* argv[]){
    int dev_n = -1;
    int write_val;
    //check the command line argument
    if (argc != 3){
        printf("Usage: %s <dev number> <write value>\n", argv[0]);
        return -1;
    }
    sscanf(argv[1],"%d", &dev_n);
    sscanf(argv[2],"%d", &write_val);
    if (dev_n <= -1 ){
        printf("Device number %s is not an integer (or less than 0)\n", argv[1]);
        return -1;
    }
    if (dev_n < 0 || dev_n > 9){
        printf("Device number %s is out of range (0,9)\n", argv[1]);
        return -1;
    }
    
    int w_filedesc = -1;
    switch (dev_n){
        //sleepy write test
        case 0:
            w_filedesc = open("/dev/sleepy0", O_WRONLY);	//write to /dev/sleepy0
            //printf("%d\n",w_filedesc);
            if ( (w_filedesc < 0)){
                printf("Failed to open file /dev/sleepy0\n");
                return -1;
            }
            //write to /dev/sleepy0
            int w_ret = write(w_filedesc, &write_val,4);
            if ( w_ret < 0){
                printf("Error while writing /dev/sleepy0. Return %d\n",w_ret);
            }
            else if (w_ret == 0){/*sleepy dev woken up normally*/
                printf("/dev/sleepy0 woken up normally\n");
            } else {
                printf("/dev/sleepy0 woken up abnormally. Time left = %d\n",w_ret);
            }
            close(w_filedesc);
            break;
        //sleepy read test    
        default:
            w_filedesc = open("/dev/sleepy0", O_RDONLY);	//read
            if ( (w_filedesc < 0) || (w_filedesc < 0) ){
                printf("Failed to open file /dev/sleepy0\n");
                return -1;
            }
	       int read_v;	
            //write to /dev/sleepy3
            if ( read(w_filedesc, read_v, 4) != 0 ){
                printf("Error while reading /dev/sleepy0\n");
            }
            close(w_filedesc);
            break;
    }
    return 0;
}
