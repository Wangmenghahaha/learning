#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

extern char *optarg;

static void print_usage(const char *prog)
{
	printf("Usage: %s [-n -a -c -d]\n", prog);
	puts(" -n  --name    print name  \n"
	     " -a  --add     print 10 + value \n"
		 " -c  --clean   print clean \n"
		 " -d  			 print value \n");
}

static void parse_opts(int argc, char *argv[])
{
	int c;
	int base = 10;
	int sum;

	static const struct option lopts[] = {
		{"add", 1, 0, 'a'},		/* 有参数， */
		{"clean", 0, 0, 'c'},	/* 没有参数 */
		{"name", 1, 0, 'n'},	/* 有参数 */
		{0, 0, 0, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "a:n:cd::", lopts, NULL);

		if (c == -1) {
			break;
		}

		switch (c) {
			case 'a':
				sum = base + atoi(optarg);
				printf("sum is %d\n", sum);
				break;
			
			case 'c':
				printf("print clean\n");
				break;
			
			/* 只有短选项，参数可选 */
			case 'd':
				if (optarg) {
					printf("value = %s\n", optarg);
				} else {
					printf("no value\n");
				}
				break;

			case 'n':
				printf("I am %s\n", optarg);
				break;

			default:
				print_usage(argv[0]);
				break;
		}
	}	
}




int main(int argc, char *argv[])
{
	parse_opts(argc, argv);
	return 0;
}
