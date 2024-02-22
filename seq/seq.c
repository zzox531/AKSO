// Author - Ignacy Kozakiewicz
// Grade - 19/20
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct Node Node;
typedef struct Node* Tree;
typedef struct Pair Pair;
typedef struct seq_t seq_t;
typedef struct Abs_c Abs_c;

#define CHECK_NODE_SEQ(p , s, val)\
if (p == NULL || is_seq_valid(s) == 0){  \
    errno = EINVAL; \
    return val;\
}\

#define CHECK_NODE_SEQ_NAME(p,s,n)\
if (p == NULL || is_seq_valid(s) == 0 || is_name_valid(n) == 0) {\
    errno = EINVAL;\
    return -1;\
}\

#define CHECK_BRANCH_MALLOC(n,b,r,s,id)\
if (r == -1) {\
    errno = ENOMEM;\
    free_sequences(b);\
    n -> children[(int)(s[id] - '0')] = NULL;\
}\

#define CHECK_NULL(n, val)\
if (n == NULL) {\
    errno = ENOMEM;\
    return val;\
}\

#define CHECK_NULL_ABS(n)\
if (n -> abs_class == NULL) {\
    free(n -> children);\
    free(n);\
    errno = ENOMEM;\
    return -1;\
}\

#define CHECK_NULL_ABS_NAME(a)\
if (a -> name == NULL) {\
    free(a);\
    errno=ENOMEM;\
    return -1;\
}\

#define CHECK_ROOT_NULL(r, s)\
if (r == NULL) {\
    free(s);\
    errno = ENOMEM;\
    return NULL;\
}\

#define CHECK_NODE_CHILDREN_NULL(n)\
if (n -> children == NULL) {\
    errno = ENOMEM;\
    free(n);\
    return -1;\
}\

#define CHECK_ROOT_CHILDREN_NULL(r, s)\
if (r -> children == NULL) {\
    errno = ENOMEM;\
    free(r);\
    free(s);\
    return NULL;\
}\

// Structure of "trie" tree containing information
// about all sequences in the set
struct seq_t {
    Tree sequences;
};

// Structure containing a Node of tree
// With a pointer to an abstraction class of this Node
struct Node {
    Tree* children;
    Abs_c* abs_class;
};

// Structure implementing the abstraction class 
// Contains a pointer to its' name, and amount
// of elements pointing at it. When el_num equals 0
// memory of the abstraction class gets deallocated
struct Abs_c {
    char* name;
    int el_num;
};

// Structure containing pair of tree nodes
// searched node, its' parent, with an index 
// of the last character of sequence represented by "node"
struct Pair {
    Tree node;
    Tree parent;
    int str_id;
};

seq_t* seq_new() {
    seq_t* new_set = (seq_t*)malloc(sizeof(seq_t));

    CHECK_NULL(new_set, NULL);
    
    Tree root = (Tree)malloc(sizeof(Node));
    
    CHECK_ROOT_NULL(root, new_set);
    
    root -> children = (Tree*)malloc(3*sizeof(Tree));
    
    CHECK_ROOT_CHILDREN_NULL(root, new_set);
    
    root -> children[0] = NULL;
    root -> children[1] = NULL;
    root -> children[2] = NULL;
    root -> abs_class = NULL;
    new_set -> sequences = root;
    
    return new_set;
}

static int get_name_size(char const* s) {
    int id = 0;
    
    while (s[id] != '\0') {
        id++;
    }

   return id;
}

static void free_sequences(Tree t) {
    if (t -> children[0] != NULL) free_sequences(t -> children[0]);
    if (t -> children[1] != NULL) free_sequences(t -> children[1]);
    if (t -> children[2] != NULL) free_sequences(t -> children[2]);
    if (t -> children != NULL) free(t -> children);
    if (t -> abs_class != NULL) {
        if (t -> abs_class -> el_num == 1) {
            if (t -> abs_class -> name != NULL) free(t -> abs_class -> name);
            free(t -> abs_class);
        }
        else t -> abs_class -> el_num -= 1;
    }
    
    free(t);
}

void seq_delete(seq_t *p){
    if (p == NULL) return;
    
    free_sequences(p -> sequences);
    free(p);
}

static int is_seq_valid(char const* s){ // 1 - Valid, 0 - Not valid
    if (s == NULL) return 0;
    
    int id = 0;
    
    while (s[id] != '\0') {
        if (s[id] != '0' && s[id] != '1' && s[id] != '2') {
            return 0;
        }
        id++;
    }
    if (id == 0) return 0;
    
    return 1;
}

static int is_name_valid(char const* n) {
    if (n == NULL || n[0] == '\0') return 0;
    return 1;
}

static int add_character(Tree p, int value) {
    if (p -> children[value] == NULL) {
        Tree node = (Tree)malloc(sizeof(Node));
        
        CHECK_NULL(node, -1);

        node -> children = (Tree*)malloc(3 * sizeof(Tree));
        
        CHECK_NODE_CHILDREN_NULL(node);
        
        node -> children[0] = NULL;
        node -> children[1] = NULL;
        node -> children[2] = NULL;
        node -> abs_class = (Abs_c*)malloc(sizeof(Abs_c));
        
        CHECK_NULL_ABS(node);

        node -> abs_class -> el_num = 1;
        node -> abs_class -> name = NULL;
        p -> children[value] = node;
    }
    return 1;
}

static int add_branch(Tree p, char const* s, int id) {
    int val;

    while (s[id]!='\0') {
        val = add_character(p, (int)(s[id]-'0'));
        if (val==-1) return -1;
        p = p->children[(int)(s[id]-'0')];
        id++;
    }
    return 0;
}

static Pair find_node(Tree p, char const* s) {
    int id = 0;
    Pair res;
    res.node = p;
    res.parent = NULL;
    
    while (s[id] != '\0') {
        if (res.node -> children[(int)(s[id] - '0')] == NULL) {
            res.node = NULL;
            res.parent = NULL;
            res.str_id = 0;
            return res;
        }
        res.parent = res.node;
        res.node = res.node -> children[(int)(s[id] - '0')];
        id++;
    }
    res.str_id = id - 1;
    
    return res;
}

int seq_add(seq_t* p, char const* s) {
    CHECK_NODE_SEQ(p, s, -1);
    
    Tree node = p -> sequences;
    int res = 0;
    int id = 0;
    
    while (s[id] != '\0' && 
    node -> children[(int)(s[id] - '0')] != NULL) {
        node = node -> children[(int)(s[id] - '0')];
        id++;
    }
    if (s[id] == '\0') return 0;
    
    Tree branch_root = (Tree)malloc(sizeof(Node));

    CHECK_NULL(branch_root, -1);

    branch_root -> children = (Tree*)malloc(3 * sizeof(Tree));

    CHECK_NODE_CHILDREN_NULL(branch_root);
    
    branch_root -> children[0] = NULL;
    branch_root -> children[1] = NULL;
    branch_root -> children[2] = NULL;
    branch_root -> abs_class = (Abs_c*)malloc(sizeof(Abs_c));
    
    CHECK_NULL_ABS(branch_root);
    
    branch_root -> abs_class -> el_num = 1;
    branch_root -> abs_class -> name = NULL;
    
    res = add_branch(branch_root, s, id + 1);
    node -> children[(int)(s[id] - '0')] = branch_root;

    CHECK_BRANCH_MALLOC(node, branch_root, res, s, id);
    
    return 1;
}

int seq_remove(seq_t* p, char const* s) {
    CHECK_NODE_SEQ(p, s, -1);
    
    Pair found_seq = find_node(p -> sequences, s);
    
    if (found_seq.node == NULL && found_seq.parent == NULL) return 0;
    found_seq.parent -> children[(int)(s[found_seq.str_id] - '0')] = NULL;
    free_sequences(found_seq.node);
    
    return 1;
}

int seq_valid(seq_t* p, char const* s) {
    CHECK_NODE_SEQ(p, s, -1);

    Pair found_seq = find_node(p -> sequences, s);
    
    if (found_seq.node == NULL && found_seq.parent == NULL) return 0;
    return 1;
}

static int names_equal(char const* s1, char const* s2) {
    if (s1 == NULL || s2 == NULL) return 0;
    
    int id = 0;
    
    while (s1[id] != '\0' && s2[id] != '\0') {
        if (s1[id] != s2[id]){
            return 0;
        }
        id++;
    }
    
    if (s1[id] == '\0' && s2[id] == '\0') return 1;
    return 0;
}

int seq_set_name(seq_t *p, char const* s, char const* n) {
    CHECK_NODE_SEQ_NAME(p,s,n);

    Pair found_seq = find_node(p -> sequences, s);
    int sz = get_name_size(n);
    
    if ((found_seq.node == NULL && found_seq.parent == NULL) || 
    names_equal(found_seq.node -> abs_class -> name, n)==1) return 0;
    
    char* new_name = (char*)malloc((size_t)((sz + 1) * sizeof(char))); 
    CHECK_NULL(new_name, -1);
    free(found_seq.node -> abs_class -> name);
    found_seq.node -> abs_class -> name = new_name;
    memcpy(found_seq.node -> abs_class -> name, n, (size_t)(sz * sizeof(char)));
    found_seq.node -> abs_class -> name[sz] = '\0';
    
    return 1;
}

char const* seq_get_name(seq_t *p, char const* s) {
    CHECK_NODE_SEQ(p, s, NULL);

    Pair found_seq = find_node(p -> sequences, s);
    if ((found_seq.node == NULL && found_seq.parent == NULL) || 
    found_seq.node -> abs_class == NULL || 
    found_seq.node -> abs_class -> name == NULL) {
        errno = 0;
        return NULL;
    }
    return found_seq.node -> abs_class -> name;
}

static void fix_pointers(Tree t, Abs_c* new_c, Abs_c* c1, Abs_c* c2) {
    if (t -> abs_class == c1 || t -> abs_class == c2) t -> abs_class = new_c;
    if (t -> children[0] != NULL) fix_pointers(t -> children[0], new_c, c1, c2);
    if (t -> children[1] != NULL) fix_pointers(t -> children[1], new_c, c1, c2);
    if (t -> children[2] != NULL) fix_pointers(t -> children[2], new_c, c1, c2);
}

int seq_equiv(seq_t *p, char const* s1, char const* s2) {
    CHECK_NODE_SEQ(p, s1, -1);
    CHECK_NODE_SEQ(p, s2, -1);

    Pair p1 = find_node(p -> sequences, s1);
    Pair p2 = find_node(p -> sequences, s2);

    if ((p1.node == NULL && p1.parent == NULL) ||
    (p2.node == NULL && p2.parent == NULL)) return 0;
    
    Abs_c *cl1 = p1.node -> abs_class;
    Abs_c *cl2 = p2.node -> abs_class;

    if (cl1 == cl2) return 0;
    
    int sz1 = 1;
    int sz2 = 1;
    Abs_c* new_abs_class = (Abs_c*)malloc(sizeof(Abs_c));
    CHECK_NULL(new_abs_class, -1);
    new_abs_class -> el_num = cl1 -> el_num + cl2 -> el_num;

    if (cl1 -> name == NULL && cl2 -> name == NULL) new_abs_class -> name = NULL;
    else if (cl1 -> name == NULL) {
        sz2 = get_name_size(cl2 -> name);
        new_abs_class -> name = (char*)malloc((size_t)((sz2 + 1) * sizeof(char)));
        CHECK_NULL_ABS_NAME(new_abs_class);
        memcpy(new_abs_class -> name, cl2 -> name, (size_t)(sz2 * sizeof(char)));
        new_abs_class -> name[sz2] = '\0';
    }
    else if (cl2 -> name == NULL) {
        sz1 = get_name_size(cl1 -> name);
        new_abs_class -> name = (char*)malloc((size_t)((sz1 + 1) * sizeof(char)));
        CHECK_NULL_ABS_NAME(new_abs_class);
        memcpy(new_abs_class -> name, cl1 -> name, (size_t)(sz1 * sizeof(char)));
        new_abs_class -> name[sz1] = '\0';
    }
    else {
        sz1 = get_name_size(cl1 -> name);
        sz2 = get_name_size(cl2 -> name);
        new_abs_class -> name = (char*)malloc((size_t)((sz1 + sz2 + 1) * sizeof(char)));
        CHECK_NULL_ABS_NAME(new_abs_class);
        memcpy(new_abs_class -> name, cl1 -> name, (size_t)(sz1 * sizeof(char)));
        memcpy(new_abs_class -> name + sz1, cl2 -> name, (size_t)(sz2 * sizeof(char)));
        new_abs_class -> name[sz1 + sz2] = '\0';
    }

    fix_pointers(p -> sequences, new_abs_class, cl1, cl2);
    free(cl1 -> name);
    free(cl2 -> name);
    free(cl1);
    free(cl2);

    return 1;
}