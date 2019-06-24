#include <stdio.h>
#include <unistd.h>

// TODO : read file "ex2_sub" into here

int flag=0;

// TODO : replace printout to printf

main(int argc, char *argv[])
{
   FILE *fp;
   int opt;

   if (argc == 1) {
      printout("Usage : a.out [-n] filename ...\n");
      exit(1);
   }
	// TODO : change to "VI EDITOR exercise code"
	printout("VI EDITOR code\n");

	// TODO : populate below lines 5 times
	printf("insert this line\n");i

   while((opt = getopt(argc, argv, "n")) != -1) {
      switch(opt) {
         case 'n':
            flag = 1;
            break;
         default:
            printout("unknown option : %c\n", optopt);
      }
   }
   
   if(argc != 2)
      argv++;
 
   while (*++argv) {
      printout("\n[filename : %s]\n", *argv);
      if((fp = fopen(*argv, "r")) == NULL) {
         perror("fopen failed");
         exit(2);
      }
      output_file(fp);
      fclose(fp);
   }
   exit(0);
}
