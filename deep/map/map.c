#include <stdlib.h>
#include <assert.h>
#include "map.h"

#define NEG (-1)
#define EQUAL 0
#define EMPTY NULL
#define EMPTY_THEN_NODE_IS_HEAD NULL
#define NODE_IS_TAIL NULL
#define END_OF_LIST NULL
#define EMPTY_MAP NULL

typedef struct Node_t {
    MapKeyElement key;
    MapDataElement data;
    struct Node_t *next;
} *Node;

typedef struct Map_t {
    Node head;
    Node iterator;
    // Those are given funcs
    copyMapDataElements copyData;
    copyMapKeyElements copyKey;
    freeMapDataElements freeData;
    freeMapKeyElements freeKey;
    compareMapKeyElements cmpKey;
} Map_t;

/** ------------------- STATICS FUNCTIONS --------------------------**/


/** Deallocates given Node*/
static void destroyNode(Map map, Node node) {
    map->freeKey(node->key);
    map->freeData(node->data);
    free(node);
}

/** nodeCreate: creating a Node of map by given key and value,
 *  the key and value must be reassign.
 *
 * @param map -  the map is used for the function (copy,free)
 * @param key -  A key to reassign(a copy) to the node
 * @param data - Data to reassign(a copy) to the node
 * @return
 *               NULL - If memory allocation, (assuming VALID values (assertion used))
 *               otherwise - new Node of map with the copy
 */
static Node nodeCreate(Map map, MapKeyElement key, MapDataElement data) {
    assert(key && data && map);
    Node new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->key = map->copyKey(key);
    if (new_node->key == NULL) {
        free(new_node);
        return NULL;
    }
    new_node->data = map->copyData(data);
    if (new_node->data == NULL) {
        map->freeKey(key);
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    return new_node;
}

/** insert: insert and sort a target Node of map the the map
 *
 * @param map Target map to insert
 * @param node Target Node to insert
 *        all the values must be checked before sent, means VALID
 */
static void insert(Map map, Node *node) {
    Node iter = map->head;
    if (iter == EMPTY_MAP) {
        map->head = (*node);
        return;
    }
    Node prev = EMPTY;
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, (*node)->key) > 0) { // MEANS LEFT KEY SMALLER
            if (prev == EMPTY_THEN_NODE_IS_HEAD) {
                map->head = (*node);
                (*node)->next = iter;
                return;
            }
            prev->next = (*node);
            (*node)->next = iter;
            return;
        }
        prev = iter;
        iter = iter->next;
    }
    assert(iter == NULL);
    prev->next = (*node);
}


Map mapCreate(copyMapDataElements copyData,
              copyMapKeyElements copyKey,
              freeMapDataElements freeData,
              freeMapKeyElements freeKey,
              compareMapKeyElements cmpKey
) {
    if (copyData == NULL || copyKey == NULL || freeData == NULL
        || freeKey == NULL || cmpKey == NULL) {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }

    map->copyData = copyData;
    map->copyKey = copyKey;
    map->freeData = freeData;
    map->freeKey = freeKey;
    map->cmpKey = cmpKey;
    map->head = NULL;
    map->iterator = NULL;
    return map;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if (map == NULL || keyElement == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Node iter = map->head;
    Node prev = EMPTY;
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, keyElement) == EQUAL) {
            if (prev == EMPTY_THEN_NODE_IS_HEAD) {
                map->head = map->head->next;
                destroyNode(map, iter);
                return MAP_SUCCESS;
            }
            if (iter->next == NODE_IS_TAIL) {
                prev->next = NULL;
                destroyNode(map, iter);
            } else {
                prev->next = iter->next;
                destroyNode(map, iter);
            }
            return MAP_SUCCESS;
        }
        prev = iter;
        iter = iter->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapResult mapClear(Map map) {
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    map->iterator = NULL;
    Node prev = map->head;
    while (map->head != EMPTY) {
        map->head = map->head->next;
        destroyNode(map, prev);
        prev = map->head;
    }
    assert(map->head == NULL);
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    if (map == NULL) {
        return;
    }
    mapClear(map);
    free(map);
}

int mapGetSize(Map map) {
    if (map == NULL) {
        return NEG;
    }
    int count = 0;
    Node iterator = map->head;
    while (iterator != NULL) {
        count++;
        iterator = iterator->next;
    }
    return count;
}

bool mapContains(Map map, MapKeyElement element) {
    if (map == NULL || element == NULL || map->head == NULL) {
        return false;
    }
    Node iter = map->head;
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, element) == EQUAL) {
            return true;
        }
        iter = iter->next;
    }
    return false;
}

Map mapCopy(Map map) {
    if (map == NULL) {
        return NULL;
    }
    map->iterator = NULL;

    Map new_map = mapCreate(map->copyData, map->copyKey, map->freeData, map->freeKey, map->cmpKey);

    if (new_map == NULL) {
        return NULL;
    }
    Node iter = map->head;
    Node prev = EMPTY;
    Node new_node = EMPTY;
    while (iter != END_OF_LIST) {
        new_node = nodeCreate(new_map, iter->key, iter->data);
        if (new_node == NULL) {
            mapDestroy(new_map);
            return NULL;
        }
        if (prev == EMPTY_THEN_NODE_IS_HEAD) {
            new_map->head = new_node;
        } else {
            iter->next = new_node;
        }
        prev = iter;
        iter = iter->next;
    }
    return new_map;

}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if (map == NULL || dataElement == NULL || keyElement == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    map->iterator = NULL;

    if (mapContains(map, keyElement)) {
        mapRemove(map, keyElement);
    }

    Node new_node = nodeCreate(map, keyElement, dataElement);
    if (new_node == NULL) {
        free(new_node);
        return MAP_OUT_OF_MEMORY;
    }
    insert(map, &new_node);
    return MAP_SUCCESS;
}

MapKeyElement mapGetFirst(Map map) {
    if (map == NULL || map->head == NULL) {
        return NULL;
    }

    map->iterator = map->head;
    return map->copyKey(map->iterator->key);
}

MapKeyElement mapGetNext(Map map) {
    if (map == NULL || map->iterator == NULL) {
        return NULL;
    }

    map->iterator = map->iterator->next;
    if (map->iterator == NULL) {
        return NULL;
    }

    return map->copyKey(map->iterator->key);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    if (map == NULL || keyElement == NULL || map->head == NULL) {
        return NULL;
    }
    Node iter = map->head;
    while (iter != NULL) {
        if (map->cmpKey(keyElement, iter->key) == EQUAL) {
            return iter->data;
        }
        iter = iter->next;
    }

    return NULL;
}
