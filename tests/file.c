#include <gae.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void testFile()
{
	const char* testText = "test.txt";
	const char* testBin = "test.bin";
	const char* testString = "Hello World!\nHere\'s a new line.";
	int testVal = 1234;
	int readVal = -1;
	
	gae_file_t testFile;
	
	gae_file_init(&testFile, GAE_FILE_MODE_WRITE, GAE_FILE_TYPE_ASCII);
	gae_file_open(&testFile, testText);
	gae_file_write(&testFile, strlen(testString), (void*)testString);
	gae_file_close(&testFile);
	gae_file_destroy(&testFile);
	
	gae_file_init(&testFile, GAE_FILE_MODE_APPEND, GAE_FILE_TYPE_BINARY);
	gae_file_open(&testFile, testBin);
	gae_file_write(&testFile, sizeof(testVal), &testVal);
	gae_file_destroy(&testFile);
	
	gae_file_init(&testFile, GAE_FILE_MODE_READ, GAE_FILE_TYPE_BINARY);
	gae_file_open(&testFile, testBin);
	gae_file_read(&testFile, sizeof(testVal), (void*)&readVal);
	gae_file_close(&testFile);
	gae_file_destroy(&testFile);
	if (testVal != readVal)
		printf("Unexpected value: %d\n", readVal);
}

static void testBuffer()
{
	const char* testText = "test.txt";
	const char* testString = "Hello World!\nHere\'s a new line.";
	const char* testString2 = "Good Morning\nThis is a different string.";
	gae_file_t testFile;
	gae_buffer_t testBuffer;
	
	gae_file_init(&testFile, GAE_FILE_MODE_READ, GAE_FILE_TYPE_ASCII);
	gae_file_open(&testFile, testText);
	gae_buffer_from_file(&testBuffer, &testFile);
	gae_file_destroy(&testFile);
	
	if (0 != strcmp((char*)testBuffer.data, testString))
		printf("Unexpected value: %s\n", (char*)testBuffer.data);
	gae_buffer_destroy(&testBuffer);
	
	gae_buffer_init(&testBuffer, GAE_BUFFER_FIXED, strlen(testString2) + 1);
	gae_buffer_write(&testBuffer, strlen(testString2) + 1, (void*)testString2);
	if (0 != strcmp((char*)testBuffer.data, testString2))
		printf("Unexpected buffer: %s\n", (char*)testBuffer.data);
	gae_file_init(&testFile, GAE_FILE_MODE_WRITE, GAE_FILE_TYPE_BINARY);
	gae_file_open(&testFile, testText);
	gae_file_write(&testFile, testBuffer.length, testBuffer.data);
	gae_file_destroy(&testFile);
	gae_buffer_destroy(&testBuffer);
}

static void print_node(gae_json_document_t* doc, gae_json_node_t* node, unsigned int depth)
{
	gae_json_node_t* child = node->child;
	gae_json_node_t* sibling = node->sibling;
	
	char* name = gae_json_string(doc, &node->name);
	char* value = gae_json_string(doc, &node->value);
	unsigned int iDepth = depth;
	while (0 < iDepth--) printf(".");
	printf("%s : %s\n", name, value);

	free(name);
	free(value);
	
	if (0 != child)
		print_node(doc, child, depth + 1);
	
	if (0 != sibling)
		print_node(doc, sibling, depth);
}

/* Test a little string of json */
static void testJS()
{
	const char* testjs = "{ \"name\": \"bob\", \"version\": 2.0, \"winning\" : { \"isAwesome\" : true, \"isBad\" : false, \"isWorse\": null, }, }";
	gae_json_document_t jsDoc;
	gae_json_node_t* found;
	
	gae_buffer_init(&(jsDoc.buffer), GAE_BUFFER_FIXED, strlen(testjs) + 1);
	gae_buffer_write(&(jsDoc.buffer), strlen(testjs) + 1, (void*)testjs);
	jsDoc.buffer.pos = 0;
	gae_json_document_parse(&jsDoc);
	
	print_node(&jsDoc, jsDoc.root, 0);
	
	found = gae_json_node_find(&jsDoc, jsDoc.root, "isBad");
	if (0 != found)
		print_node(&jsDoc, found, 5);
	
	gae_json_document_destroy(&jsDoc);
}

/* Test parsing an entire file of json */
static void testJS2()
{
	gae_json_document_t jsDoc;
	gae_json_document_init(&jsDoc, "Duck.gltf");
	gae_json_document_parse(&jsDoc);
	
	print_node(&jsDoc, jsDoc.root, 0);
	gae_json_document_destroy(&jsDoc);
}

/* Test parsing a string of json into something more usable */
struct json_test_struct {
	int x;
	int y;
	int w;
	int h;
};

static int int_from_node(gae_json_document_t* jsDoc, gae_json_node_t* node)
{
	char* value = gae_json_string(jsDoc, &node->value);
	int val = atoi(value);
	free(value);
	
	return val;
}

static void json_to_box(gae_json_document_t* jsDoc, struct json_test_struct* box, gae_json_node_t* node)
{
	gae_json_node_t* x = gae_json_node_find(jsDoc, node, "x");
	gae_json_node_t* y = gae_json_node_find(jsDoc, node, "y");
	gae_json_node_t* w = gae_json_node_find(jsDoc, node, "w");
	gae_json_node_t* h = gae_json_node_find(jsDoc, node, "h");
	
	box->x = int_from_node(jsDoc, x);
	box->y = int_from_node(jsDoc, y);
	box->w = int_from_node(jsDoc, w);
	box->h = int_from_node(jsDoc, h);
}

static void print_box(struct json_test_struct* box)
{
	printf("Box: %d, %d, %d, %d\n", box->x, box->y, box->w, box->h);
}

static void testJS3()
{
	const char* testjs =
	"{ \"boxes\": [ \
		{ \"x\" : 5, \"y\" : 0, \"w\" : 3, \"h\" : 3 }, \
		{ \"x\" : 4, \"y\" : 10, \"w\" : 4, \"h\" : 4 }, \
		{ \"x\" : 3, \"y\" : 20, \"w\" : 8, \"h\" : 19 }, \
		{ \"x\" : 2, \"y\" : 30, \"w\" : 5, \"h\" : 10 }, \
		]\
	}";
	gae_json_document_t jsDoc;
	gae_json_node_t* found;
	struct json_test_struct boxes[4];
	unsigned int i = 0;
	
	gae_buffer_init(&(jsDoc.buffer), GAE_BUFFER_FIXED, strlen(testjs) + 1);
	gae_buffer_write(&(jsDoc.buffer), strlen(testjs) + 1, (void*)testjs);
	jsDoc.buffer.pos = 0;
	gae_json_document_parse(&jsDoc);
	
	found = gae_json_node_find(&jsDoc, jsDoc.root, "boxes");
	
	found = found->child;
	for (i = 0; i < 4; ++i) {
		json_to_box(&jsDoc, &boxes[i], found);
		found = found->sibling;
	}
	
	for (i = 0; i < 4; ++i)
		print_box(&boxes[i]);
	
	gae_json_document_destroy(&jsDoc);
}

int main(int argc, char** argv)
{
	(void)(argc);
	(void)(argv);
	
	testFile();
	testBuffer();
	testJS();
	testJS2();
	testJS3();
	
	return 0;
}
