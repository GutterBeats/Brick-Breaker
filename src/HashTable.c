//
// Created by Anthony Lesch on 7/31/24.
//

#include <stdlib.h>
#include <string.h>
#include <SDL_log.h>

#include "HashTable.h"

#include "Prime.h"

#define HT_RESIZE_DOWN_PERCENT 30
#define HT_RESIZE_UP_PERCENT 70
#define HT_INITIAL_BASE_SIZE 53
#define HT_PRIME_1 163
#define HT_PRIME_2 157

// Create a new hash table item.
static HashTable* CreateHashTable(int baseSize);
static HashTableItem* CreateNewItem(const char* key, const char* value);
static void DeleteItem(HashTableItem* item);
static int GetItemHash(const char* key, int numberBuckets, int attempt);
static int CalculateHash(const char* value, int seed, int modulus);
static void ResizeTable(HashTable* table, int baseSize);
static void ResizeUp(HashTable* table);
static void ResizeDown(HashTable* table);

static HashTableItem HT_DELETED_ITEM = { NULL, NULL };

HashTable* HT_CreateHashTable(void)
{
    return CreateHashTable(HT_INITIAL_BASE_SIZE);
}

void HT_DeleteHashTable(HashTable* table)
{
    for (int i = 0; i < table->Count; ++i)
    {
        HashTableItem* item = table->Items[i];
        if (item == NULL) continue;

        DeleteItem(item);
    }

    free(table->Items);
    free(table);
}

void HT_Insert(HashTable* table, const char* key, const char* value)
{
    if (table == NULL) return;

    const int load = table->Count * 100 / table->Size;
    if (load > HT_RESIZE_UP_PERCENT)
    {
        ResizeUp(table);
    }

    HashTableItem* item = CreateNewItem(key, value);
    int index = GetItemHash(item->Key, table->Size, 0);
    HashTableItem* current = table->Items[index];

    int i = 1;
    while (current != NULL)
    {
        if (current != &HT_DELETED_ITEM
            && strcmp(current->Key, key) == 0)
        {
            DeleteItem(current);
            table->Items[index] = item;

            return;
        }

        index = GetItemHash(item->Key, table->Size, i);
        current = table->Items[index];

        ++i;
    }

    table->Items[index] = item;
    table->Count++;
}

char* HT_Search(const HashTable* table, const char* key)
{
    int i = 0;
    int index = GetItemHash(key, table->Size, i++);
    const HashTableItem* item = table->Items[index];

    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM
            && strcmp(item->Key, key) == 0)
        {
            return item->Value;
        }

        index = GetItemHash(key, table->Size, i);
        item = table->Items[index];
        ++i;
    }

    return NULL;
}

void HT_Delete(HashTable* table, const char* key)
{
    if (table == NULL) return;

    const int load = table->Count * 100 / table->Size;
    if (load < HT_RESIZE_DOWN_PERCENT)
    {
        ResizeDown(table);
    }

    int i = 0;
    int index = GetItemHash(key, table->Size, i++);
    HashTableItem* item = table->Items[index];

    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->Key, key) == 0)
            {
                DeleteItem(item);
                table->Items[index] = &HT_DELETED_ITEM;
                table->Count--;

                return;
            }
        }

        index = GetItemHash(key, table->Size, i);
        item = table->Items[index];
        i++;
    }
}

static HashTable* CreateHashTable(const int baseSize)
{
    HashTable* table = malloc(sizeof(HashTable));
    if (table == NULL)
    {
        return NULL;
    }

    table->BaseSize = baseSize;
    table->Size = NextPrime(baseSize);
    table->Count = 0;
    table->Items = calloc(table->Size, sizeof(HashTableItem*));

    return table;
}

static HashTableItem* CreateNewItem(const char* key, const char* value)
{
    // ReSharper disable once CppDFAMemoryLeak
    HashTableItem* item = malloc(sizeof(HashTableItem));
    item->Key = strdup(key);
    item->Value = strdup(value);

    return item;
}

static void DeleteItem(HashTableItem* item)
{
    free(item->Key);
    free(item->Value);
    free(item);
}

static int GetItemHash(const char* key, const int numberBuckets, const int attempt)
{
    const int hashA = CalculateHash(key, HT_PRIME_1, numberBuckets);
    const int hashB = CalculateHash(key, HT_PRIME_2, numberBuckets - 1);

    return (hashA + attempt * (hashB + 1)) % numberBuckets;
}

static int CalculateHash(const char* value, const int seed, const int modulus)
{
    long hash = 0;
    const int length = strlen(value);

    for (int i = 0; i < length; ++i)
    {
        hash += pow(seed, length - (i + 1)) * value[i];
    }

    hash %= modulus;

    return hash;
}

static void ResizeTable(HashTable* table, const int baseSize)
{
    if (baseSize < HT_INITIAL_BASE_SIZE) return;

    HashTable* newHashTable = CreateHashTable(baseSize);
    for (int i = 0; i < table->Size; i++)
    {
        const HashTableItem* item = table->Items[i];
        if (item != NULL && item != &HT_DELETED_ITEM)
        {
            HT_Insert(newHashTable, item->Key, item->Value);
        }
    }

    table->BaseSize = newHashTable->BaseSize;
    table->Count = newHashTable->Count;

    // To delete new_ht, we give it ht's size and items
    const int tempSize = table->Size;
    table->Size = newHashTable->Size;
    newHashTable->Size = tempSize;

    HashTableItem** tmp_items = table->Items;
    table->Items = newHashTable->Items;
    newHashTable->Items = tmp_items;

    HT_DeleteHashTable(newHashTable);
}

static void ResizeUp(HashTable* table)
{
    // ReSharper disable once CppDFANullDereference
    const int new_size = table->BaseSize * 2;
    ResizeTable(table, new_size);
}


static void ResizeDown(HashTable* table)
{
    const int new_size = table->BaseSize / 2;
    ResizeTable(table, new_size);
}
