
#include "util.h"


void dump(HashTable* tab) {

    printf("\ntab->cap = %d\n", tab->cap);
    printf("tab->count = %d\n", tab->count);
    printf("tab->tombstones = %d\n", tab->tombstones);
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL)
                printf("%3d.\t%s\t%lu\n", i + 1, tab->table[i]->key,
                       *(long*)tab->table[i]->data); //, tab->table[i]->hash);
            else
                printf("%3d.\ttombstone\n", i + 1);
        }
        else
            printf("%3d.\tblank\n", i + 1);
    }

    printf("\n");
}

int main() {

    const char* keys[] = { "qwer", "asdf", "zxcv", "wert", "sdfg",
                           "xcvb", "erty", "dfgh", "cvbn", "rtyu",
                           "fghj", "vbnm", "tyui", "ghjk", "bnm",
                           "yuio", "hjkl", "uiop", "jkl",  NULL };

    HashTable* table = create_hashtable();
    long value;

    for(int i = 0; i < 5; i++) {
        value = random();
        insert_hashtable(table, keys[i], &value, sizeof(value));
    }

    dump(table);

    for(int i = 5; i < 6; i++) {
        value = random();
        insert_hashtable(table, keys[i], &value, sizeof(value));
    }

    dump(table);

    for(int i = 3; keys[i] != NULL; i++) {
        value = random();
        insert_hashtable(table, keys[i], &value, sizeof(value));
    }

    dump(table);

    remove_hashtable(table, "vbnm");
    remove_hashtable(table, "jkl");
    remove_hashtable(table, "xcvb");
    remove_hashtable(table, "ghjk");

    dump(table);

    value = random();
    insert_hashtable(table, "vbnm", &value, sizeof(value));
    value = random();
    insert_hashtable(table, "ghjk", &value, sizeof(value));

    dump(table);

    long val;
    char* str = "ghjk";
    bool res = find_hashtable(table, str, &val, sizeof(val));
    printf("find: %s: %s: %lu\n", str, res ? "true" : "false", val);
    val = 0;

    str = "sdfg";
    res = find_hashtable(table, str, &val, sizeof(val));
    printf("find: %s: %s: %lu\n", str, res ? "true" : "false", val);
    val = 0;

    str = "jkl";
    res = find_hashtable(table, str, &val, sizeof(val));
    printf("find: %s: %s: %lu\n", str, res ? "true" : "false", val);
    val = 0;

    str = "erty";
    res = find_hashtable(table, str, &val, sizeof(val));
    printf("find: %s: %s: %lu\n\n", str, res ? "true" : "false", val);
    val = 0;

    return 0;
}
