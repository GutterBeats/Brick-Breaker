//
// Created by Anthony Lesch on 7/31/24.
//

#pragma once

typedef struct ht_item
{
    char* Key;
    char* Value;
} HashTableItem;

typedef struct hash_table
{
    int Size;
    int Count;
    int BaseSize;

    HashTableItem** Items;
} HashTable;

HashTable* HT_CreateHashTable(void);
void HT_DeleteHashTable(HashTable* table);

void HT_Insert(HashTable* table, const char* key, const char* value);
char* HT_Search(const HashTable* table, const char* key);
void HT_Delete(HashTable* table, const char* key);
