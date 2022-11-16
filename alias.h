#ifndef ALIASH
#define ALIASH

/**
 * struct AliasData - struct for alias function
 * @AliasData - name of the struct
 * @name: name of the alias value
 * @val: value for the alias
 * @next: pointer to next node
 *
 */

typedef struct AliasData
{
	char *name;
	char *val;
	struct AliasData *next;
} AliasData;

#endif
