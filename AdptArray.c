#include <stdio.h>
#include <stdlib.h>

#include "AdptArray.h"

struct AdptArray_
{
    int size;
    PElement* elements;
    COPY_FUNC copy_func;
    DEL_FUNC delete_func;
    PRINT_FUNC print_func;
};

PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC delete, PRINT_FUNC print)
{
    PAdptArray adpt_arr = malloc(sizeof(struct AdptArray_));

    if (adpt_arr != NULL) 
    {
        adpt_arr->size = 0;
        adpt_arr->elements = NULL;

        adpt_arr->copy_func = copy;
        adpt_arr->delete_func = delete;
        adpt_arr->print_func = print;
    }

    return adpt_arr;
}

void DeleteAdptArray(PAdptArray adpt_arr)
{
    if (adpt_arr == NULL)
        return;

    if (adpt_arr->elements != NULL)
    {
        for (int i = 0; i < adpt_arr->size; i++)
        {
            if (adpt_arr->elements[i] != NULL)
                adpt_arr->delete_func(adpt_arr->elements[i]);
        }
        

        free(adpt_arr->elements);
    }

    free(adpt_arr);
}

Result SetAdptArrayAt(PAdptArray adpt_arr, int index, PElement element)
{
    if (adpt_arr == NULL || index < 0)
        return FAIL;

    if (index < adpt_arr->size)
    {
        adpt_arr->delete_func(adpt_arr->elements[index]);
    }
    else // We would need to extend the array.
    {
        PElement* temp;
        if (adpt_arr->size == 0)
            temp = malloc((index+1)*sizeof(PElement));
        else
            temp = realloc(adpt_arr->elements, (index+1)*sizeof(PElement));

        if (temp == NULL)
            return FAIL;
        
        adpt_arr->elements = temp;

        for (int i = adpt_arr->size; i < index+1; i++)
        {
            adpt_arr->elements[i] = NULL;
        }
    }

    adpt_arr->elements[index] = adpt_arr->copy_func(element);
    adpt_arr->size = index + 1;

    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray adpt_arr, int index)
{
    if (adpt_arr == NULL || adpt_arr->elements == NULL)
        return NULL;

    if (index < 0 || adpt_arr->size-1 < index)
        return NULL;

    return adpt_arr->elements[index];
}

int GetAdptArraySize(PAdptArray adpt_arr)
{
    if (adpt_arr == NULL)
        return -1;

    return adpt_arr->size;
}

void PrintDB(PAdptArray adpt_arr)
{
    if (adpt_arr == NULL)
        printf("Error: adpt_arr is NULL.\n");

    for (int i = 0; i < adpt_arr->size; i++)
    {
        if (adpt_arr->elements[i] != NULL)
            adpt_arr->print_func(adpt_arr->elements[i]);
    }
}
