#include "priorityqueue.h"
#include <stdexcept>
#include <algorithm>

PriorityQueue::PriorityQueue() : head(nullptr) {}

PriorityQueue::~PriorityQueue() {
    clear(head);
}

void PriorityQueue::clear(Node* node) {
    if (!node) return;
    clear(node->sibling);
    clear(node->child);
    delete node;
}

bool PriorityQueue::isEmpty() const {
    return head == nullptr;
}

int PriorityQueue::size() const {
    int count = 0;
    Node* curr = head;
    while (curr) {
        // Count nodes in each tree (2^degree)
        count += (1 << curr->degree);
        curr = curr->sibling;
    }
    return count;
}

void PriorityQueue::linkTrees(Node* lesser, Node* greater) {
    lesser->parent = greater;
    lesser->sibling = greater->child;
    greater->child = lesser;
    greater->degree++;
}

// Merge by tree order
PriorityQueue::Node* PriorityQueue::mergeRootLists(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    Node* head = nullptr;
    Node** pos = &head;

    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            *pos = h1;
            h1 = h1->sibling;
        } else {
            *pos = h2;
            h2 = h2->sibling;
        }
        pos = &(*pos)->sibling;
    }

    *pos = (h1 ? h1 : h2);
    return head;
}

// Combine trees of same degree
PriorityQueue::Node* PriorityQueue::unionHeaps(Node* h1, Node* h2) {
    Node* merged = mergeRootLists(h1, h2);
    if (!merged) return nullptr;

    Node* prev = nullptr;
    Node* curr = merged;
    Node* next = curr->sibling;

    while (next) {
        if (curr->degree != next->degree ||
           (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        }
        else if (curr->patient.severity >= next->patient.severity) {
            curr->sibling = next->sibling;
            linkTrees(next, curr);
        }
        else {
            if (!prev) merged = next;
            else prev->sibling = next;
            linkTrees(curr, next);
            curr = next;
        }
        next = curr->sibling;
    }

    return merged;
}

void PriorityQueue::merge(PriorityQueue& other) {
    head = unionHeaps(head, other.head);
    other.head = nullptr;
}

void PriorityQueue::insert(const Patient& p) {
    Node* newNode = new Node(p);
    head = unionHeaps(head, newNode);
}

const Patient& PriorityQueue::peek() const {
    if (!head) throw runtime_error("Heap is empty");
    return getMaxNode()->patient;
}

PriorityQueue::Node* PriorityQueue::getMaxNode() const {
    if (!head) return nullptr;

    Node* maxNode = head;
    Node* curr = head->sibling;

    while (curr) {
        if (curr->patient.severity > maxNode->patient.severity)
            maxNode = curr;
        curr = curr->sibling;
    }
    
    return maxNode;
}

Patient PriorityQueue::extractMax() {
    if (!head) throw runtime_error("Heap is empty");

    Node* maxNode = getMaxNode();
    Patient maxPatient = maxNode->patient;

    // Remove maxNode from root list
    if (maxNode == head) {
        head = head->sibling;
    } else {
        Node* curr = head;
        while (curr->sibling != maxNode)
            curr = curr->sibling;
        curr->sibling = maxNode->sibling;
    }

    // Reverse children list and make into new heap
    Node* child = maxNode->child;
    Node* rev = nullptr;
    while (child) {
        Node* next = child->sibling;
        child->sibling = rev;
        child->parent = nullptr;
        rev = child;
        child = next;
    }

    delete maxNode;

    head = unionHeaps(head, rev);
    return maxPatient;
}
