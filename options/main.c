#include <stdio.h>
#include <getopt.h>


struct option longopts[] = {
    { "superpi",    no_argument,          NULL, 'p'},
    { "dhrystone",  no_argument,          NULL, 'd'},
    { "whetstone",  no_argument,          NULL, 'w'},
    { "memstream",  no_argument,          NULL, 'm'},
    {     0,    0,    0,    0},
};

int main(int argc, char *argv[])
{
    int c;
    
    while((c = getopt_long(argc, argv, ":pdwm", longopts, NULL)) != -1){
        switch (c){
		    case 'p':
		        printf("[CPU-Test] : super pi\n");
		        break;
		    case 'd':
		        printf("[CPU-Test] : dhrystone\n");
		        break;
		    case 'w':
		        printf("[CPU-Test] : whetstone\n");
		        break;
		    case 'm':
		        printf("[CPU-Test] : memstream\n");
		        break;        
		    case '?':
		        printf("[CPU-Test] : error option\n");
		        break; 		        
		    case ':':
		        printf("[CPU-Test] : lack param\n");
		        break; 
		}
    }
    return 0;
}
