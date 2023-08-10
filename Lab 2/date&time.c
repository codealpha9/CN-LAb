#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<time.h>
int main()
{
	time_t t;
    time(&t);
    printf("\nThis program has been written at date and time %s",ctime(&t));
    printf("\n\n\t\t Vande Matram\n\n\n\n");
	return 0;
}