# Transactions

It's a group project for three person.

Part 1. Implementation of in-memory key-value store based on a hash table

- The program is developed in C++ language of C++17 standard
- It is a static library (with a hash_table.h header file)
- The library is represented as a HashTable class, which stores the information in a hash table and contains all the necessary methods for working with them.
- Prepared full coverage of HashTable class methods with unit-tests

Part 2. Implementation of in-memory key-value store based on self-balancing binary search tree (red-black-tree)

- It is a static library (with a self_balancing_binary_search_tree.h header file)
- The library is represented as a SelfBalancingBinarySearchTree class, which stores information in the form of a
self-balancing binary search tree and contains all the necessary methods for working with them.

Part 3. Implementation of in-memory key-value store based on B+ trees

- It is a static library (with a b_plus_tree.h header file)
- The library is represented as a BPlusTree class, which stores information in the form of a B+ tree and contains all the necessary methods for working with them.

Part 4. Implementation of the console interface

A console interface is implemented that provides the user with the following options:

- Initial selection of the store type to be used in the process of running the program:
    - hash table
    - self-balancing binary search tree
    - b-plus tree
- Ability to enter commands for work with storage

Part 5. Research on temporal characteristics.

Research on temporal characteristics of in-memory key-value store implementations based on a hash table and also a binary search tree.
- The user sets the number of items in the store
- Person in the store is randomly generated
- The user sets the number of iterations of one operation
- Perform each operation listed below the number of times specified by the user, and then display the average running
- time among the obtained values

Measure the time for the following operations by both types of stores:
- Getting an arbitrary item
- Adding an item
- Deleting an item
- Getting a list of all elements in the dictionary
- Finding the item key by value
