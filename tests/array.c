#include <gae.h>

#include <stdio.h>

static void testArrayOfInts()
{
	gae_array_t array;
	unsigned int i = 0;

	gae_array_init(&array, sizeof(int));
	for (i = 5; i > 0; --i) {
		gae_array_push(&array, &i);
	}

	printf("array length: %d\n", gae_array_length(&array));

	for (i = 0; i < gae_array_length(&array); ++i) {
		int* d = gae_array_get(&array, i);
		printf("%d: %d\n", i, *d);
	}

	gae_array_destroy(&array);
}

static void testArrayOfStrings()
{
	gae_array_t array;
	unsigned int i = 0;
	char* const strings[] = {
		"Robert", "Badminton", "Bad Self", "Rick", "Sausages", "Potato"
	};

	gae_array_init(&array, 32);
	for (i = 0; i < 6; ++i) {
		gae_array_push(&array, strings[i]);
	}

	printf("array length: %d\n", gae_array_length(&array));
	
	for (i = 0; i < gae_array_length(&array); ++i) {
		char* d = gae_array_get(&array, i);
		printf("%d: %s\n", i, d);
	}

	gae_array_destroy(&array);
}

int main(int argc, char** argv)
{
	(void)(argc);
	(void)(argv);
	
	testArrayOfInts();
	testArrayOfStrings();
	
	return 0;
}
