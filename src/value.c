#include <stdio.h>

#include "value.h"
#include "object.h"
#include "memory.h"

static const char
	__nil[]  = "nil",
	__bool[] = "bool",
	__num[]  = "num";

const char *aupV_typeOf(aupV value)
{
	if (AUP_IS_NIL(value)) {
		return __nil;
	}
	else if (AUP_IS_BOOL(value)) {
		return __bool;
	}
	else if (AUP_IS_NUM(value)) {
		return __num;
	}
	else if (AUP_IS_OBJ(value)) {
		return aupO_typeOf(AUP_AS_OBJ(value));
	}

	return __nil;
}

void aupV_print(aupV value)
{
	if (AUP_IS_NIL(value)) {
		printf("nil");
	}
	else if (AUP_IS_BOOL(value)) {
		printf(AUP_AS_BOOL(value) ? "true" : "false");
	}
	else if (AUP_IS_NUM(value)) {
		printf("%.14g", AUP_AS_NUM(value));
	}
	else if (AUP_IS_OBJ(value)) {
		aupO_print(AUP_AS_OBJ(value));
	}
}

void aupVa_init(aupVa *array)
{
	array->count = 0;
	array->capacity = 0;
	array->values = NULL;
}

int aupVa_write(aupVa *array, aupV value)
{
	if (array->capacity < array->count + 1) {
		int oldCapacity = array->capacity;
		array->capacity = AUP_GROW_CAP(oldCapacity);
		array->values = AUP_GROW_ARR(aupV, array->values, oldCapacity, array->capacity);
	}

	array->values[array->count] = value;
	return array->count++;
}

void aupVa_free(aupVa *array)
{
	AUP_FREE_ARR(aupV, array->values, array->capacity);
	aupVa_init(array);
}

int aupVa_find(aupVa *array, aupV value)
{
#define IS_EQUAL(v1, v2) \
	AUP_VAL_TYPE(v1) == AUP_VAL_TYPE(v2) && \
	AUP_AS_RAW(v1) == AUP_AS_RAW(v2)

	for (int i = 0; i < array->count; i++) {
		if (IS_EQUAL(array->values[i], value)) {
			return i;
		}
	}

	return -1;
}