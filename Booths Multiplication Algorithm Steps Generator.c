 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

///////////////////////////////////////////////FUNCTION DECLARATIONS///////////////////////////////////////////////////

// LOGICAL FUNCTIONS
int getDecimal(int binary[], int size);
int* getBinaryArray(int number, int size);
void acPlusMinusM(int* ac, int* m, int size);
int getOptimalBinarySize(int number1, int number2);
int* getTwosCompliment(int* number_binary, int size);
int arithematicShiftRight(int* ac, int* qn, int size);
void boothsMultiplicationAlgorithm(int number1, int number2);
int* addTwoBinaryArrays(int* binary_array1, int* binary_array2, int size);

// TABLE PRINTING FUNCTIONS
void printEmptyRow(int count);
void printChar(int number_of_times_to_print, char ch);
void setOpertionType(int operation_no, char operation_name[]);
void printResults(int number1, int number2, int* ac, int* qn, int size);
void printRowData(int n, int* ac, int* qn, int q_one, int count, char operation_name[]);
void printBinariesAndComplements(char ch, int number, int* binary, int* complement, int count);
void printInitials(int num1, int num2, int* num1b, int* num2b, int* num1c, int* num2c, int* ac, int* qn, int q_one, int count);
void printCalculations(char sign, char type, int *ac, int *M, int count);

//////////////////////////////////////////////////////ENDS///////////////////////////////////////////////////////////////

int main()
{
	printf("========= WELCOME TO BOOTH'S SIGNED BINARY MULTIPLICATION ALGORITHM ===========\n\n");
	printf("Enter two integers to multiply using Booth's Algorithm:-\n");
	int number1 = 0, number2 = 0;
	scanf("%d%d", &number1, &number2);

	if(number1 == 0 && number2 == 0)
	{
		printf("Cannot generate the Booth's Algorithm Calculation data!\n"
			"Given both the input integers as zeros\n"
			"\nResultant will be: 0\n");
	}
	else
		boothsMultiplicationAlgorithm(number1, number2);

	return 0;
}


// LOGICAL FUNCTIONS

void boothsMultiplicationAlgorithm(int number1, int number2)
{
	int size = getOptimalBinarySize(number1, number2);

	int* number1_binary = getBinaryArray(abs(number1), size);
	int* number2_binary = getBinaryArray(abs(number2), size);

	int* number1_twos_compliment = getTwosCompliment(number1_binary, size);
	int* number2_twos_compliment = getTwosCompliment(number2_binary, size);

	int ac[size];
	int qn[size];

	for(int i=0 ; i<size ; i++)
	{
		ac[i] = 0;
		qn[i] = (number2 < 0) ? number2_twos_compliment[i] : number2_binary[i];
	}

	int q_one = 0;

	printInitials(number1, number2, number1_binary, number2_binary,
				number1_twos_compliment, number2_twos_compliment, ac, qn, q_one, size);

	int operation_no = 0;

	char operation_name[22];

	int *plusM = (number1 < 0) ? number1_twos_compliment : number1_binary;
	int *minusM = (number1 < 0) ? number1_binary : number1_twos_compliment;

	for(int i=0 ; i<size ; i++)
	{
		if(qn[size-1] == 1 && q_one == 0)
		{
			printCalculations('-', 'q', ac, minusM, size);
			acPlusMinusM(ac, minusM, size);
			printCalculations('-', 'r', ac, minusM, size);
			operation_no = 1;
		}
		else if(qn[size-1] == 0 && q_one == 1)
		{
			printCalculations('+', 'q', ac, plusM, size);
			acPlusMinusM(ac, plusM, size);
			printCalculations('-', 'r', ac, plusM, size);
			operation_no = 2;
		}

		setOpertionType(operation_no, operation_name);

		 // An older code for printing data row
		/* 
			if(operation == 1)
			 {
			 	setOpertionType(1, operation_name);
			 	printRowData(i+1, ac, qn, q_one, size, operation_name);
			 }
			 else if(operation == 2)
			 {
			 	setOpertionType(2, operation_name);
			 	printRowData(i+1, ac, qn, q_one, size, operation_name);
			 }
			 else
			 	setOpertionType(0, operation_name);
		 */

		if(operation_no != 0)
		{
			printRowData(i+1, ac, qn, q_one, size, operation_name);
		}

		q_one = arithematicShiftRight(ac, qn, size);

		setOpertionType(0, operation_name);

		 // logic explanation for the below code
		 /*
			 checking if there was an operation performed which
			 can be either AC = AC - M OR AC = AC + M indicated 
			 by 1 or 2 by the operation_no.

			 if the operation number was 1 or 2 that means  
			 a complete cycle hasn't been completed yet and the 
			 there has to be pending printRow function 
			 with no cycle count No. (that is the else part) just the ASR

			 and if the operation number is 0, that means a cycle is 
			 completed and we will print the row data with the count
			 cycle number.
		 */

		if(operation_no == 0)
			printRowData(i+1, ac, qn, q_one, size, operation_name);
		else
			printRowData(-1, ac, qn, q_one, size, operation_name);

		printEmptyRow(size);

		operation_no = 0;
	}
	printChar(74 + 3*(2*size-1), '=');

	printResults(number1, number2, ac, qn, size);

	free(number1_binary);
	free(number2_binary);
	free(number1_twos_compliment);
	free(number2_twos_compliment);
}

/*
	Description-->
	Takes two integers and calculates the optimal
	size for containing their binary form of
	both integers.
*/
int getOptimalBinarySize(int number1, int number2)
{
	int size = 0;

	if(number1 < 0 || number2 < 0)
	{
		size++;
	}

	number1 = abs(number1);
	number2 = abs(number2);

	if(number1 > number2)
	{
		size += log(number1)/log(2) + 1 + 1;
	}
	else
	{
		size += log(number2)/log(2) + 1 + 1;
	}

	return size;
}

/*
	Description-->
	Takes an integer and a calculated size
	and returns a pointer to a binary
	array of the given integer.
*/
int* getBinaryArray(int number, int size)
{
	int* binary_array = (int*)calloc(size, sizeof(int));

	int i = size-1;

	while(number)
	{
		binary_array[i] = number % 2;
		number /= 2;
		i--;
	}

	return binary_array;
}

/*
	Description-->
	Takes two binary arrays and a size
	and returns a pointer to an binary
	array of the sum of the given two 
	binary arrays.
*/
int* addTwoBinaryArrays(int* binary_array1, int* binary_array2, int size)
{
	int* sum_binary_array = (int*)calloc(size, sizeof(int));

	int carry = 0, ith_index_sum = 0;
	size--;

	while(size >= 0)
	{	
		ith_index_sum = binary_array1[size] + binary_array2[size] + carry;

		if(ith_index_sum == 3)
		{
			sum_binary_array[size] = 1;
		}
		else if(ith_index_sum == 2)
		{
			sum_binary_array[size] = 0;

			if(carry == 0)
				carry = 1;

		}
		else if(ith_index_sum == 1)
		{
			sum_binary_array[size] = 1;

			if(carry == 1)
				carry = 0;
		}
		else
		{
			sum_binary_array[size] = 0;
		}

		size--;
	}

	return sum_binary_array;
}

/*
	Description-->
	Takes a binary arrat and a size and
	returns a pointer to the another
	binary array which is the 2's
	complement of the given binary array.
*/
int* getTwosCompliment(int* number_binary, int size)
{
	int ones_compliment[size];

	for(int i=0 ; i<size ; i++)
	{
		if(number_binary[i] == 0)
			ones_compliment[i] = 1;
		else
			ones_compliment[i] = 0;
	}

	int onein_binary[size];

	for(int i=0 ; i<size ; i++)
		onein_binary[i] = 0;

	onein_binary[size-1] = 1;

	return addTwoBinaryArrays(ones_compliment, onein_binary, size);
}

/*
	Description-->
	Takes the two binary arrays "AC" and "Qn"
	and an integer size and shifts the bits
	in both the arrays to the right starting
	from index 1 of the "AC" array
	and returns an integer for q0 at the end.
*/
int arithematicShiftRight(int* ac, int* qn, int size)
{
	int previous_bit = ac[0];
	int next_bit = 0;

	for(int i=1 ; i<size ; i++)
	{
		next_bit = ac[i];
		ac[i] = previous_bit;
		previous_bit = next_bit;
	}

	for(int i=0 ; i<size ; i++)
	{
		next_bit = qn[i];
		qn[i] = previous_bit;
		previous_bit = next_bit;
	}

	return next_bit;
}

/*
	Description-->
	Takes the two binary arrays "AC" and "M"
	and rewrites "AC" array with the resultant
	of addition of the "AC" and "M".
*/
void acPlusMinusM(int* ac, int* m, int size)
{
	int carry = 0, ith_index_sum = 0;
	size--;

	while(size >= 0)
	{	
		ith_index_sum = ac[size] + m[size] + carry;

		if(ith_index_sum == 3)
		{
			ac[size] = 1;
		}
		else if(ith_index_sum == 2)
		{
			ac[size] = 0;

			if(carry == 0)
				carry = 1;

		}
		else if(ith_index_sum == 1)
		{
			ac[size] = 1;

			if(carry == 1)
				carry = 0;
		}
		else
		{
			ac[size] = 0;
		}

		size--;
	}
}

/*
	Description-->
	Takes the binary array and it's size
	and returns the equivalent decimal
	integer for the given binary array.
*/
int getDecimal(int binary[], int size)
{
	int decimal = 0;
	int multiplier = 1;
	size--;

	while(size >= 0)
	{
		decimal += binary[size] * multiplier;
		multiplier *= 2;
		size--;
	}

	return decimal;
}


//==================================================== TABLE PRINTING FUNCTIONS ===========================================

void printResults(int number1, int number2, int* ac, int* qn, int size)
{
	int product_binary[size*2];
	printf("\n\nResultant Binary Product: ");
	int index = 0;

	for(int i=0 ; i<size ; i++, index++)
	{
		printf("%d", ac[i]);
		product_binary[index] = ac[i];
	}
	for(int i=0 ; i<size ; i++, index++)
	{
		printf("%d", qn[i]);
		product_binary[index] = qn[i];
	}

	int decimal_product = 0;

	// checking if only one of the number is negative.
	if((number1 < 0 && number2 > 0) || (number1 > 0 && number2 < 0))
	{
		int* twos_complement_of_resultant = getTwosCompliment(product_binary, size*2);

		printf("\n\nSince only one of the number is negative..."
			"\nTaking 2's complement of the resultant binary product\n");

		printf("\n2's Complement of the Resultant Binary Product: ");
		for(int i=0 ; i<size*2 ; i++)
		{
			printf("%d", twos_complement_of_resultant[i]);
		}

		decimal_product = getDecimal(twos_complement_of_resultant, size*2);
		printf("\n\nDecimal Equivalent: -%d", decimal_product);

		free(twos_complement_of_resultant);
	}
	else
	{
		decimal_product = getDecimal(product_binary, size*2);
		printf("\n\nDecimal Equivalent: %d", decimal_product);
	}
}

void printBinariesAndComplements(char ch, int number, int* binary, int* complement, int count)
{
	if(ch == 'b')
	{
		if(number < 0)
		{
			printf("%d in binary is: ", number);
			for(int i=0 ; i<count ; i++)
			{
				printf("%d ", complement[i]);
			}
			printf("\t(2's Complement of %d)\n", abs(number));
		}
		else
		{
			printf("%d in binary is: ", number);
			for(int i=0 ; i<count ; i++)
			{
				printf("%d ", binary[i]);
			}
			printf("\n");
		}
	}
	else
	{
		if(number < 0)
		{
			printf("2's Complement of %d is: ", number);
			for(int i=0 ; i<count ; i++)
			{
				printf("%d ", binary[i]);
			}
			printf("\t(2's Complement of %d is the binary of %d)\n", number, abs(number));
		}
		else
		{
			printf("2's Complement of %d is: ", number);
			for(int i=0 ; i<count ; i++)
			{
				printf("%d ", complement[i]);
			}
			printf("\n");
		}
	}

}

void printChar(int number_of_times_to_print, char ch)
{
	for(int i=0 ; i<number_of_times_to_print ; i++)
	{
		printf("%c", ch);
	}
}

void printEmptyRow(int count)
{
	printf("|     |    ");
	for(int i=0 ; i<count ; i++)
		printf("  ");
	printf("  |    ");

	for(int i=0 ; i<count ; i++)
		printf("  ");
	printf("  |      |                                                       ");
	printf("\n");
}

void printInitials(int num1, int num2, int* num1b, int* num2b, int* num1c, int* num2c, int* ac, int* qn, int q_one, int count)
{
	printf("\n===============INITIAL VALUES GENERATED FOR ENTERED DATA===============\n\n");

	printf("In Decimals:-\n");
	printf("First Number: %d\n", num1);
	printf("Second Number: %d\n\n", num2);

	printf("In Binary:-\n");
	printBinariesAndComplements('b', num1, num1b, num1c, count);
	printBinariesAndComplements('b', num2, num2b, num2c, count);
	printf("\n");

	printf("Negative in Binary:- (Two's Complement)\n");
	printBinariesAndComplements('c', num1, num1b, num1c, count);
	printBinariesAndComplements('c', num2, num2b, num2c, count);
	printf("\n");

	printf("AC: ");
	for(int i=0 ; i<count ; i++)
		printf("0 ");
	printf("\n");

	printf("Qn: ");
	for(int i=0 ; i<count ; i++)
		printf("%d ", qn[i]);
	printf("\n");

	printf("Q0: %d\n", q_one);
	printf("Count: %d\n\n", count);

	int *select = (num1 < 0) ? num1c : num1b;

	printf("(+M) = ");
	for(int i=0 ; i<count ; i++)
		printf("%d ", select[i]);
	printf("\n");

	select = (num1 < 0) ? num1b : num1c;

	printf("(-M) = ");
	for(int i=0 ; i<count ; i++)
		printf("%d ", select[i]);
	printf("\n\n");


	int spaces = 2*count-1;


	printf("************************CALCULATION DATA GENERATED************************\n\n");
	printChar(74 + 3*spaces, '=');
	printf("\n");

	printf("|  N  |  ");

	printChar((2+spaces)/2, ' ');
	printf("AC");
	printChar((2+spaces)/2, ' ');
	printf("  |  ");

	printChar((2+spaces)/2, ' ');
	printf("Qn");
	printChar((2+spaces)/2, ' ');
	printf("  |  q0  |  ");

	printChar((20+spaces)/2, ' ');
	printf("OPERATIONS PERFORMED");
	printChar((20+spaces)/2, ' ');
	printf("  |\n");
	printChar(74 + 3*spaces, '=');
	printf("\n");

	printf("|  0  |    ");
	for(int i=0 ; i<count ; i++)
		printf("%d ", ac[i]);
	printf("  |    ");

	for(int i=0 ; i<count ; i++)
		printf("%d ", qn[i]);
	printf("  |  %d   |  Initials                                            ", q_one);
	printf("\n");

	printEmptyRow(count);
}

void printRowData(int n, int* ac, int* qn, int q_one, int count, char operation_name[])
{
	if(n == -1)
		printf("|     |    ");
	else
		printf("|  %d  |    ", n);


	for(int i=0 ; i<count ; i++)
		printf("%d ", ac[i]);
	printf("  |    ");

	for(int i=0 ; i<count ; i++)
		printf("%d ", qn[i]);
	printf("  |  %d   |  %s                                            ", q_one, operation_name);
	printf("\n");

	printEmptyRow(count);

}

void setOpertionType(int operation_no, char operation_name[])
{
	if(operation_no == 0)
	{
		strcpy(operation_name, "ASR");
	}
	else if(operation_no == 1)
	{
		strcpy(operation_name, "AC = AC - M");
	}
	else
		strcpy(operation_name, "AC = AC + M");
}

void printCalculations(char sign, char type, int *ac, int *M, int count)
{
	if(type == 'q')
	{
		printf("\n");
		for(int i=0 ; i<count*2-2 ; i++)
			printf("\t");

		for(int i=0 ; i<count ; i++)
			printf("%d ", ac[i]);
		printf("\t(ac)\n");
		for(int i=0 ; i<count*2-2 ; i++)
			printf("\t");

		for(int i=0 ; i<count ; i++)
			printf("%d ", M[i]);
		printf("\t(%cM)\n", sign);
		for(int i=0 ; i<count*2-2 ; i++)
			printf("\t");

		for(int i=0 ; i<count ; i++)
			printf("--");
		printf("\n");
		for(int i=0 ; i<count*2-2 ; i++)
			printf("\t");
	}
	else
	{
		for(int i=0 ; i<count ; i++)
			printf("%d ", ac[i]);
		printf("\n\n");
	}
}




//=====================================================CODE FOR TESTING PURPOSES==============================================

//////////////////////////////For Optimal Binary Size Testing///////////////////////////////

	// int size = getOptimalBinarySize(number1, number2);
	// printf("%d\n", size);
	// printf("Optimal Binary Size: %d", size);

	////////////////////////////For Binary Conversion Testing///////////////////////////////

	// int* array1 = getBinaryArray(number1, size);
	// int* array2 = getBinaryArray(number2, size);

	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array1[i]);
	// }
	// printf("\n");
	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array2[i]);
	// }
	// printf("\n");


	/////////////////////////////For Adding Two Binarys Testing////////////////////////////////

	// int* sum = addTwoBinaryArrays(array1, array2, size);

	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", sum[i]);
	// }


	/////////////////////////////For Testing Two Compliment////////////////////////////////////

	// int* twos_compliment1 = getTwosCompliment(array1, size);
	// int* twos_compliment2 = getTwosCompliment(array2, size);


	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", twos_compliment1[i]);
	// }
	// printf("\n");
	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", twos_compliment2[i]);
	// }


	/////////////////////////////For Arithmetic Right Shift Testing/////////////////////////////

	// int q = arithematicShiftRight(array1, array2, size);

	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array1[i]);
	// }
	// printf("\n");
	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array2[i]);
	// }
	// printf("\n%d\n", q);


	/////////////////////////////For AC = AC + - M Testing///////////////////////////////////////

	// acPlusMinusM(array1, array2, size);

	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array1[i]);
	// }
	// printf("\n");
	// for(int i=0 ; i<size ; i++)
	// {
	// 	printf("%d ", array2[i]);
	// }




