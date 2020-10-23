
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef long double data_set_type;

//Global variables. ****************************************************************************************************
size_t num_of_data = 0;
double expected_frequency[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
double actual_frequency[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int digit_counter[9] = { 0 };
double nb_deviation = 0;


//Function header definitions. ****************************************************************************************************

void sort_data(data_set_type* data);

data_set_type calc_mean(data_set_type* data);

data_set_type calc_median(data_set_type* data);

data_set_type calc_variance(data_set_type* data, data_set_type mean);

void calc_mode(data_set_type* data);

void first_digit_frequency(data_set_type* data);

void nb_analysis();

void calc_nb_variance_and_deviation();

void print_nb_relationship();

void output(data_set_type* data);


//main function.
int main(int argc, char* argv[]) {

	//Titles program.
	printf("Newcomb-Benford Stats (v2.0.0), copyright 2020 Hashem Abou Ahmad\n");
	printf("================================================================\n");
	//Prompts user to enter the desired data input.
	printf("Enter the number of data that is being passed: ");
	//Scans desired input.
	scanf_s("%d", &num_of_data);

	//Allocates memory in the heap for the data.
	data_set_type* data;
	data = (data_set_type*)malloc(num_of_data * sizeof(data_set_type));

	//Prompts user to enter the data set.
	printf("Enter a real number followed by the return key. Terminate input with ^Z\n");

	for (size_t i = 0; i < num_of_data; ++i) {
		scanf_s("%lf", (data + i));
		//Checks for bad data.
		if ((*(data + i)) <= 0.0) {
			//Explains issue.
			printf("Invalid entry, please enter a value greater than zero.\n\n");
			--i;
		}
	}//End of for loop.

	int ch;
	while ((ch = getchar() != EOF)) {
		//Notifies user that desired limit has reached.
		printf("Number of data entered reached the desired limit. Terminate input with ^Z (Ctrl + z)\n");
	}//End of while loop.

	//Invokes the data sort function.
	sort_data(data);
	
	//Invokes the output function.
	output(data);

	//Deallocates memory reserved previously.
	free(data);

	return 0;

}//End of main() function.


//Functions. ****************************************************************************************************
void sort_data(data_set_type* data) {

	data_set_type temp;
	int swapped;
	while (true) {
		swapped = 0;
		for (size_t i = 0; i < num_of_data - 1; ++i) {

			if (data[i] > data[i + 1]) {
				temp = data[i];
				data[i] = data[i + 1];
				data[i + 1] = temp;
				swapped = 1;
			}

		}
		if (swapped == 0) {
			break;
		}
	}
}//End of sort_data() function.

data_set_type calc_mean(data_set_type* data) {

	data_set_type sum = 0;
	for (size_t i = 0; i < num_of_data; ++i) {
		sum += data[i];
	}
	return sum / num_of_data;

}//End of calc_mean() function.

data_set_type calc_median(data_set_type* data) {
	int left_middle_index;
	int right_middle_index;
	data_set_type median;
	if (num_of_data % 2 == 0) {
		right_middle_index = num_of_data / 2;
		left_middle_index = right_middle_index - 1;
		median = (data[right_middle_index] + data[left_middle_index]) / 2.0;
		return median;
	}
	else {
		return data[num_of_data / 2];
	}
}//End of calc_median() function.

data_set_type calc_variance(data_set_type* data, data_set_type mean) {

	data_set_type sum_of_numerator = 0;

	for (size_t i = 0; i < num_of_data; ++i) {
		sum_of_numerator += pow(data[i] - mean, 2);
	}

	return sum_of_numerator / num_of_data;

}//End of calc_variance() function.

void calc_mode(data_set_type* data) {

	data_set_type* multi_modal_ptr = (data_set_type*)calloc(num_of_data, sizeof(data_set_type));

	int match_counter = 0;
	int max_counter = 0;
	int uni_modal_counter = 0;
	int j = 0;
	data_set_type max_value = 0;

	for (size_t i = 0; i < num_of_data - 1; ++i) {
		if (data[i] == data[i + 1]) {
			++match_counter;
			++uni_modal_counter;

			if (uni_modal_counter > max_counter) {
				max_value = data[i];
				max_counter = uni_modal_counter;
			}

			if (uni_modal_counter == max_counter) {
				multi_modal_ptr[j] = data[i];
				++j;
			}
		}
		else {
			uni_modal_counter = 0;
		}
	}

	if (match_counter == 0 || match_counter == ceil(num_of_data / 2.0)) {
		printf("Mode = no mode\n");
	}
	else if (match_counter > 0) {
		printf("Mode = { %g }x%d\n", max_value, max_counter + 1);
	}
	else {
		printf("Mode = { ");
		for (size_t i = 0; i < num_of_data; ++i) {

			if (multi_modal_ptr[i] != 0) {
				printf("%lf ", multi_modal_ptr[i]);
			}
		}
		printf("}x%d\n", max_counter + 1);
	}

	free(multi_modal_ptr);
}//End of calc_mode() function.

void first_digit_frequency(data_set_type* data) {

	int first_digit, count;

	int digits[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (size_t i = 0; i < num_of_data; ++i) {

		count = log10((int)data[i]);

		first_digit = (int)data[i] / pow(10, count);

		for (int j = 0; j < sizeof(digits) / sizeof(int); ++j) {

			if (first_digit == digits[j]) {
				++digit_counter[j];
			}
		}
	}

	for (int i = 0; i < sizeof(digits) / sizeof(int); ++i) {
		printf("  [%d] = %d\n", digits[i], digit_counter[i]);
	}
}//End of first_digit_frequency() function.

void nb_analysis() {

	int total = 0;
	for (int i = 0; i < 9; ++i) {
		total += digit_counter[i];
	}

	int digits[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i = 0; i < 9; ++i) {
		expected_frequency[i] = log10(1.0 + 1.0 / (expected_frequency[i])) * 100.0;
		actual_frequency[i] = (double)digit_counter[i] / total * 100.0;
		printf(" %5.2lf%% [%d] = %5.2lf%%", expected_frequency[i], digits[i], actual_frequency[i]);
		wchar_t graph[52] = { 179 };
		for (size_t j = 1; j < (size_t)(actual_frequency[i]); ++j) {
			graph[j] = 220;
		}
		printf("\t%ls\n", graph);
	}
}//End of nb_analysis() function.

void calc_nb_variance_and_deviation()
{
	double sum = 0;
	double nb_variance = 0;
	for (int i = 0; i < 9; ++i)
	{
		sum += pow((actual_frequency[i] / expected_frequency[i] - 1), 2);
	}
	nb_variance = sum / 9.0;
	nb_deviation = sqrt(nb_variance);

	printf("Variance = %.5lf%%\n", nb_variance * 100.0);
	printf("Std. Dev. = %.5lf%%", nb_deviation * 100.0);
}//End of calc_nb_variance_and_deviation() function.

void print_nb_relationship() {
	if (nb_deviation >= 0.5)
	{
		printf("There is no Benford relationship.\n");
	}
	else if (nb_deviation >= 0.35)
	{
		printf("There is a weak Benford relationship.\n");
	}
	else if (nb_deviation >= 0.2)
	{
		printf("There is a moderate Benford relationship.\n");
	}
	else if (nb_deviation >= 0.1)
	{
		printf("There is a strong Benford relationship.\n");
	}
	else
	{
		printf("There is a very strong Benford relationship.\n");
	}
}//End of print_nb_relationship() function.

void output(data_set_type* data) {
	printf("\nStandard Analysis\n");
	printf("------------------------------------------------------------------\n");
	printf("# of elements = %d\n", num_of_data);
	printf("Range = [%g ... %g]\n", data[0], data[num_of_data - 1]);
	printf("Arithmetic mean = %g\n", calc_mean(data));
	printf("Arithmetic median = %g\n", calc_median(data));
	printf("Variance = %g\n", calc_variance(data, calc_mean(data)));
	printf("Standard Deviation = %g\n", sqrtl(calc_variance(data, calc_mean(data))));
	calc_mode(data);
	putchar('\n');
	first_digit_frequency(data);
	putchar('\n');
	printf("Newcomb-Benford's Law Analysis\n");
	printf("------------------------------------------------------------------\n");
	printf("    exp dig\tfreq\t0\t10\t20\t30\t40\t50\n");
	nb_analysis();
	putchar('\n');
	calc_nb_variance_and_deviation();
	putchar('\n');
	print_nb_relationship();
	printf("------------------------------------------------------------------\n");
}//End of output() function.