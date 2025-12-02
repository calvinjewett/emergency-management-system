#pragma once
#include <vector>
#include "patient.h" 

class PriorityQueue {
private:
    struct Node {
        Patient patient;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(const Patient& p)
            : patient(p), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    Node* head; // Head of binomial tree list

    Node* mergeRootLists(Node* h1, Node* h2);
    void linkTrees(Node* lesser, Node* greater);
    Node* unionHeaps(Node* h1, Node* h2);

    Node* getMaxNode() const;
    Node* extractMaxNode();

public:
    PriorityQueue();
    ~PriorityQueue();

    void insert(const Patient& p);
    Patient extractMax();
    const Patient& peek() const;
    bool isEmpty() const;
    int size() const;
    void merge(PriorityQueue& other);

private:
    void clear(Node* node);
};
