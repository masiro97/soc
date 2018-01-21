#include <stdio.h>
#define SIZE 5
void swap(int b[], int*p, int*q);

int main() {

	int max, min;
	int a[SIZE] = { 0 };
	for (int i = 0; i < SIZE; i++)
		scanf_s("%d", &a[i]);

	swap(a, &max, &min);

	printf("max = %d, min = %d\n", max, min);

}

void swap(int b[], int *p, int *q) {

	int max, min;
	int i = 0;
	int s[SIZE] = { 0 };

	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			if (b[x] > b[y])
				i++;
		}
		for (int k = 0; k < SIZE; k++) {

			if (k == i) {

				if (s[k] == 0)
					s[k] = b[x];
				else
					s[k + 1] = b[x];

			}
		}

		i = 0;
	}

	//printf("0 = %d 1 = %d 2 = %d 3 = %d 4 = %d\n", s[0], s[1], s[2], s[3], s[4]);
	for (int m = 0; m < SIZE;m++)
		b[m] = s[m];
	max = b[SIZE - 1];
	min = b[0];
	/*int same = 0;
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			if (b[x] > b[y])
				i++;
		}

		if (i == SIZE - 1) {
			max = b[x];
		}
			
		if (i == 0)
			min = b[x];
		i = 0;
	}

	*/
	*p = max;
	*q = min;
}

