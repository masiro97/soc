/*

크기5인 행렬에 5개의 수를 받고 이를 크기순으로 재배열하는거...
단 행렬은 함수를 이용해서 함수내에서 배열되고 이를 메인함수로 반환해서 메인함수에서 출력해야함*/

#include <stdio.h>
#define SIZE 5
void swap(int b[]);

int main() {

	int a[SIZE] = { 0 };
	for (int i = 0; i < SIZE; i++)
		scanf_s("%d", &a[i]);

	swap(a);

	for (int n = 0;n < SIZE;n++)
		printf("%d ", a[n]);

	return 0;

}

void swap(int b[]) {
	
	int i = 0;
	int same = 0;
	int s[SIZE] = { 0 };

	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			if (b[x] > b[y])
				i++;
			else if (b[x] == b[y])
				same++;
		}
		for (int k = 0; k < SIZE; k++) {

		/*	if (same > 0) {
				for (int s = 0;s < same;s++)
					s[k + s] = b[x];
			}*/
			if (k == i) {

				if (s[k] == 0)
					s[k] = b[x];
				else {
					for (int j = 0;j < same;j++)
						s[k + j] = b[x];
				}
			}
		}

		i = 0;
	}

	//printf("0 = %d 1 = %d 2 = %d 3 = %d 4 = %d\n", s[0], s[1], s[2], s[3], s[4]);
	for (int m = 0; m < SIZE;m++)
		b[m] = s[m];
	//printf("0 = %d 1 = %d 2 = %d 3 = %d 4 = %d\n", b[0], b[1], b[2], b[3], b[4]);

}