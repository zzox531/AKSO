Sets of Equivalent Sequences

The task involves creating a dynamically loaded library in C that manages sets of sequences with an equivalence relation. 
The elements of these sets are non-empty sequences comprised of the digits 0, 1, and 2. 
For implementation purposes, a sequence is represented as a string, e.g., the sequence {0, 1, 2} is represented as "012". It's possible to assign names to abstract classes.

Library Interface:

The library is expected to provide functions and a type name as declared in the attached seq.h file. 
A valid sequence is a non-empty string made up of '0', '1', or '2', terminated with a null character. 
Similarly, a valid abstract class name is a non-empty string ending with a null character. 
Further operational details, especially regarding what constitutes an invalid parameter, can be inferred from the seq_example.c file provided as part of the task description. 
The expression "the set of sequences remains unchanged" implies that the observable state of the sequence set has not altered.

typedef struct seq seq_t;

This structure type, representing a set of sequences with an equivalence relation, needs to be defined within the scope of this assignment.

seq_t *seq_new(void);

This function creates a new, empty set of sequences. It returns a pointer to the newly created set of sequences or NULL if memory allocation fails, setting errno to ENOMEM.

void seq_delete(seq_t *p);

The seq_delete function is responsible for deleting a set of sequences and freeing all associated memory. 
It performs no action if called with a NULL pointer. After this function is called, the provided pointer becomes invalid.

int seq_add(seq_t *p, char const *s);

The seq_add function adds a given sequence and all its non-empty subsequences that are prefixes of the original sequence to the set. It returns 1 if at least one new sequence was added, 0 if the set remains unchanged, or -1 if any parameter is incorrect or a memory allocation failure occurs, setting errno to EINVAL or ENOMEM, respectively.

int seq_remove(seq_t *p, char const *s);

This function removes a specified sequence and all sequences of which it is a prefix from the set. 
It returns 1 if at least one sequence was removed, 0 if the set remains unchanged, or -1 if any parameter is incorrect, setting errno to EINVAL.

int seq_valid(seq_t *p, char const *s);

seq_valid checks whether a given sequence belongs to the set. 
It returns 1 if the sequence is part of the set, 0 if it is not, or -1 if any parameter is incorrect, setting errno to EINVAL.

int seq_set_name(seq_t *p, char const *s, char const *n);

The seq_set_name function assigns or changes the name of the abstract class to which the given sequence belongs. 
It returns 1 if the name was successfully assigned or changed, 0 if the sequence does not belong to the set or the name remains unchanged, or -1 if any parameter is incorrect or a memory allocation error occurs, setting errno to EINVAL or ENOMEM.

char const *seq_get_name(seq_t *p, char const *s);

This function retrieves the name of the abstract class to which a given sequence belongs. 
It returns a pointer to the name or NULL if the sequence does not belong to the set or if the abstract class does not have a name, setting errno to 0 or EINVAL as appropriate.

int seq_equiv(seq_t *p, char const *s1, char const *s2);

seq_equiv merges two sequences into one abstract class if they are represented by the provided sequences. 
If neither abstract class has a name, the new class will not have one either. 
If exactly one class has a name, the new class inherits it. If both classes have different names, the new class's name is a combination of these names. 
The function returns 1 if a new abstract class was created, 0 if no new class was formed because the sequences already belong to the same class or one does not belong to the set, or -1 if any parameter is incorrect or a memory allocation error occurred, setting errno to EINVAL or ENOMEM.

Strings representing sequences may no longer exist after the function execution ends. The implementation should not impose artificial limits on data size, with the only constraints being the available memory and the machine's word size. The implementation will be validated using Valgrind to ensure no memory leaks or inconsistent data structures, even in the event of a memory allocation error.

Compilation:

In order to compile the example test, LD_LIBRARY_PATH needs to be changed using:

export LD_LIBRARY_PATH=current_path

Then, compile all the files using "make all".

In order to run one of the tests (params, simple, equivalence, memory), run:

./seq <test_name>