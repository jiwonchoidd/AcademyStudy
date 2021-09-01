#include <stdio.h>
#include <stddef.h>
// hash example
#include <iostream>
#include <functional>
#include <string>


size_t stringLength(const char* source)
{
	if (source == NULL) { return 0; }

	size_t length = 0;
	while (*source != '\0') 
	{
		length++;
		source++;
	}
	return length;
}
// hash function 
static size_t djb_hash(const char* cp)
{
	size_t hash = 5381;
	while (*cp)
		hash = 33 * hash ^ (unsigned char)*cp++;
	return hash;
}

//  hash function
static size_t fnv1a_hash(const char* cp)
{
	size_t hash = 0x811c9dc5;
	while (*cp) {
		hash ^= (unsigned char)*cp++;
		hash *= 0x01000193;
	}
	return hash;
}
static size_t getHash(const char* source)
{
	size_t length = stringLength(source);
	size_t hash = 0;
	for (size_t i = 0; i < length; i++) 
	{
		char c = source[i];
		int a = c - '0';
		hash = (hash * 10) + a;
	}

	return hash;
}

static const char *const testCases[] = {
	"this",
	"is",
	"a",
	"test",
	"but",
	"i",
	"should",
	"use",
	"real",
	"dictonary"
};

#define TABLE_SIZE (16)
//unsigned long hash(unsigned char *str)
//{
//	unsigned long hash = 5381;
//	int c;
//
//	while (c = *str++)
//		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//	return hash;
//}
//unsigned long hash(unsigned char *str)
//{
//	unsigned long hash = 5381;
//	int c;
//
//	while (c = *str++)
//		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//	return hash;
//}

int main()
{
	size_t name = getHash("Ryan McCullagh");
	printf("%zu\n", name);

	for (size_t i = 0; i < (sizeof(testCases) / sizeof(testCases[0])); i++) 
	{
		const char* source = testCases[i];
		size_t hash = fnv1a_hash(source);
		printf("%s <==> %zu\n", source, (hash % TABLE_SIZE));
	}
	
	//////////////////////////////////////////////////////////
	char nts1[] = "Test";
	char nts2[] = "Test";
	std::string str1(nts1);
	std::string str2(nts2);

	std::hash<char*> ptr_hash;
	std::hash<std::string> str_hash;

	std::cout << "same hashes:\n" << std::boolalpha;
	std::cout << "nts1 and nts2: " << (ptr_hash(nts1) == ptr_hash(nts2)) << '\n';
	std::cout << "str1 and str2: " << (str_hash(str1) == str_hash(str2)) << '\n';
	return 0;
}
