#include<stdio.h>

void main()
{

	int num1,num2,num3;

    printf("숫자 3개 입력해~~");
    scanf_s("%d%d%d", &num1, &num2, &num3);

	if (num1 % 2 == 0)
		printf("짝수\n");
	else
		printf("홀수\n");


}