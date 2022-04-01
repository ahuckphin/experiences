/* CELEBT12

   This example provides the amount of processor time
   used by instructions and the system for the parent and child
   processes.

 */
#define _POSIX_SOURCE
#include <sys/times.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int factorial(int n, int m);

void *generatePrimeNumbers(void *primenum) {
    int  i, j, a, b;
    int primeNumbers[] = {};
    int limit = (int *)primenum;

    for (i = 1; i <= limit; i++) {
           j = 0;
           int a = factorial(i, i + 1);
           int b =  (i - 1) + 2;
           if (((a / i) * b) != 0) {
               printf("%d ", ((a / i) * b));
               j++;
           }
    }
    printf("\n");
    return NULL;
}

int factorial(int n, int m) {
    if (n == 1) {
        return 1;
    } else return (n * factorial(n - 1, m)) % m;

}

main(int argc, char *argv[]) {
  int status;
  long i, j;
  struct tms t, s;
  clock_t dub, start;
  int tics_per_second;
  tics_per_second = sysconf(_SC_CLK_TCK);
  start = times(&s);

    int numLimit;
    pthread_t primethread;
    if (argc != 2) {
       printf("You need to enter a valid number!\n");
       exit(-1);
   }
   else {
       int i = 0;
       numLimit = atoi(argv[1]);
       if (numLimit < 2) {
               printf("Please enter a number greater than or equal to 2.\n");
               exit(-1);
       }
   }

  if (fork() == 0) {
     pthread_create(&primethread, NULL, generatePrimeNumbers, (void *)numLimit);
     pthread_exit(NULL);
  }

  if (wait(&status) == -1)
    perror("wait() error");
  else if (!WIFEXITED(status))
    puts("Child did not exit successfully");
  else if ((dub = times(&t)) == -1)
    perror("times() error");
  else {
    printf("process was dubbed %jd seconds ago.\n\n", (dub - start));
    //printf("process was dubbed %jd seconds ago.\n\n", 
    //       ((double) dub)/tics_per_second);
    printf("            utime           stime\n");
    printf("parent:    %f        %f\n",
           ((double) t.tms_utime)/tics_per_second,
           ((double) t.tms_stime)/tics_per_second);
    printf("child:     %f        %f\n",
           ((double) t.tms_cutime)/tics_per_second,
           ((double) t.tms_cstime)/tics_per_second);
  }
}
