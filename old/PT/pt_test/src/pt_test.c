#include <stdio.h>
#include <stdlib.h>

#define SIRKA 4 // sirka(uhol) trojuholnika

void medzery(int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		printf(" ");
	}
}

void paskal(int n)
{
	int *pole;
	int *pole2;
	int j, k, l, m;
	int z=1;

	pole = (int*)malloc(n*(sizeof(int)));
	pole2 = (int*)malloc(n*(sizeof(int)));

	// vrchol
	medzery(SIRKA*(n-1));
	printf("%d", z);
	printf("\n");

	for(j=1; j<n; j++)
	{
		for(l=0; l<=j; l++)
		{
			if(l==0)
			{
				pole2[l]=1;
			}
			else if(l==j)
			{
				pole2[j]=1;
			}
			else
			{
				pole2[l] = pole[l-1] + pole[l];
			}
		}
		for(m=0; m<=(l+1); m++)
		{
			pole[m]=pole2[m];
		}

		medzery(SIRKA*(n-j-1)); // medzeri s pravej strany trojuholnika

		for(k=0; k<=j; k++)
		{
			if(pole2[k]%2 == 0)
			{
				printf("*");
				medzery((2*SIRKA) - 1); // tu vzdy len 1 medzera
				continue;
			}
			else
			{
				printf("%d", pole2[k]);
			}
			// zistujeme medzery
			// napr pri jednocifernom cisle potrebujeme 1 medzeru teda vydelime cislo 10
			// i = 1
			// pr. 2 ciferne cislo
			// 55/10 je 5, i=1
			// 5/10 je 	0, i=2
			// teda odcitame 2 medzeri
			int i=0;
			int pom = pole[k];
			while(pom > 0){
				i++;
				pom /= 10;
			}
			medzery((2*SIRKA) - i);
		}
		printf("\n");
	}
	printf("\n");
}

void main()
{
	paskal(50);
}
