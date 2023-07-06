#ifndef LINUX_EMULATOR_GTREE_H
#define LINUX_EMULATOR_GTREE_H

#include "file.h"
#include <iostream>
#include <vector>

namespace LinuxEmulator{

class Node {
public:
    File data;
    Node* parent;
    std::vector<Node*> children;

    Node(const File& value) : data(value), parent(nullptr) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    Node* getParent() const {
        return parent;
    }

    File getData() const {
        return data;
    }

    int getChildCount() const {
        return children.size();
    }

    Node* getChild(int index) const {
        if (index >= 0 && index < children.size()) {
            return children[index];
        }
        return nullptr;
    }

    void addChild(Node* childNode) {
        childNode->parent = this;
        children.push_back(childNode);
    }
};


class GeneralTree {
public:
    GeneralTree();
    GeneralTree(Node*);
    ~GeneralTree();
    File getRootData() const;
    Node* getRoot() const;
    void setRoot(Node* r);
    void setRootData(File f);
    void traverseEnd(Node*);
    Node* findNode(const File& targetValue);
    Node* findNode(Node* currentNode, const File& targetValue); 
    Node* findNode(const std::string& targetName);
    Node* findNode(Node* currentNode, const std::string& targetName);

    GeneralTree& operator=(const GeneralTree& other);
    void copyTreeNodes(Node*, const Node*);
    bool isEqual(const Node*, const Node*) const;
    bool insert(const File&, const File&);
    void insert(Node*, Node*);
    void remove(const File&);
    void traverseEnd();
    void printParents(const File& targetValue);
    void printAllNodes(Node* currentNode);
    bool operator==(const GeneralTree& other) const {
        return isEqual(root, other.root);
    }
private:
    Node* root;
};


GeneralTree::GeneralTree() : root(nullptr) {}
GeneralTree::GeneralTree(Node* r) : root{r} {}

GeneralTree::~GeneralTree() {
    delete root;
}

File GeneralTree::getRootData() const {
    return root->data;
}

Node* GeneralTree::getRoot() const {
    return root;
}

void GeneralTree::setRoot(Node* r) {
    root = r;
}

void GeneralTree::setRootData(File f) {
    if(root == nullptr) {
        root = new Node(f);
    } else {
        root->data = f;
    }
}

void GeneralTree::traverseEnd(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (Node* child : node->children) {
        traverseEnd(child);
    }

    std::cout << node->data.getName() << " ";
}



Node* GeneralTree::findNode(const File& targetValue) {
    return findNode(root, targetValue);
}

Node* GeneralTree::findNode(Node* currentNode, const File& targetValue) {
    if (currentNode == nullptr) {
        return nullptr;
    }

    if (currentNode->data == targetValue) {
        return currentNode;
    }

    for (Node* child : currentNode->children) {
        Node* foundNode = findNode(child, targetValue);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}

// Node* GeneralTree::findNode(Node* currentNode, const File& targetDirectory) {
//     if (currentNode == nullptr || currentNode->data.getName() == targetDirectory.getName()) {
//         return currentNode;
//     }

//     for (Node* child : currentNode->children) {
//         if (child->data.getIsDirectory()) {
//             Node* result = findNode(child, targetDirectory);
//             if (result != nullptr) {
//                 return result;
//             }
//         }
//     }

//     return nullptr;
// }

Node* GeneralTree::findNode(const std::string& targetName) {
    return findNode(root, targetName);
}

Node* GeneralTree::findNode(Node* currentNode, const std::string& targetName) {
    if (currentNode == nullptr) {
        return nullptr;
    }
    
    if (currentNode->data.getName() == targetName) {
        return currentNode;
    }
    
    for (Node* child : currentNode->children) {
        Node* foundNode = findNode(child, targetName);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }
    
    return nullptr;
}



GeneralTree& GeneralTree::operator=(const GeneralTree& other) {
    if (this == &other) {
        return *this; 
    }

    delete root;

    if (other.root != nullptr) {
        root = new Node(other.root->data);
        copyTreeNodes(root, other.root);
    } else {
        root = nullptr;
    }

    return *this;
}

void GeneralTree::copyTreeNodes(Node* newNode, const Node* otherNode) {
    for (const Node* child : otherNode->children) {
        Node* newChild = new Node(child->data);
        newChild->parent = newNode;
        newNode->children.push_back(newChild);
        copyTreeNodes(newChild, child);
    }
}

bool GeneralTree::isEqual(const Node* node1, const Node* node2) const {
    if (node1 == nullptr && node2 == nullptr) {
        return true; 
    }

    if (node1 == nullptr || node2 == nullptr) {
        return false;  
    }

    if (node1->data != node2->data) {
        return false; 
    }

    if (node1->children.size() != node2->children.size()) {
        return false;  
    }

    for (size_t i = 0; i < node1->children.size(); i++) {
        if (!isEqual(node1->children[i], node2->children[i])) {
            return false;  
        }
    }

    return true;  
}

bool GeneralTree::insert(const File& value, const File& parentValue) {
    Node* newNode = new Node(value);

    if (root == nullptr) {
        root = newNode;
        return true;
    }

    std::vector<Node*> queue;
    queue.push_back(root);

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.erase(queue.begin());

        if (current->data.getName() == parentValue.getName()) {
            current->children.push_back(newNode);
            return true; // Insertion successful
        }

        for (Node* child : current->children) {
            queue.push_back(child);
        }
    }

    delete newNode;
    std::cout << "Parent with value " << parentValue.getName() << " not found." << std::endl;
    return false; 
}

void GeneralTree::insert(Node* parentNode, Node* childNode) {
    childNode->parent = parentNode;
    parentNode->children.push_back(childNode);
}

void GeneralTree::remove(const File& value) {
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }

    std::vector<Node*> queue;
    queue.push_back(root);

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.erase(queue.begin());

        for (auto it = current->children.begin(); it != current->children.end(); ++it) {
            if ((*it)->data.getName() == value.getName()) {
                delete (*it);
                current->children.erase(it);
                std::cout << "Node with value " << value.getName() << " deleted." << std::endl;
                return;
            }

            queue.push_back(*it);
        }
    }
    std::cout << "Node with value " << value.getName() << " not found." << std::endl;
}

void GeneralTree::traverseEnd() {
    std::cout << "End-order traversal: ";
    traverseEnd(root);
    std::cout << std::endl;
}


void GeneralTree::printParents(const File& targetValue) {
    Node* targetNode = findNode(root, targetValue);
    if (targetNode == nullptr) {
        std::cout << "Node with value " << targetValue.getName() << " not found." << std::endl;
        return;
    }

    std::cout << "Parents of node with value " << targetValue.getName() << ": ";

    std::vector<std::string> parents;
    Node* parentNode = targetNode->parent;
    while (parentNode != nullptr) {
        parents.push_back(parentNode->data.getName());
        parentNode = parentNode->parent;
    }

    for (auto it = parents.rbegin(); it != parents.rend(); ++it) {
        std::cout << "/" << *it;
    }

    std::cout << std::endl;
}

void GeneralTree::printAllNodes(Node* currentNode) {
    if (currentNode == nullptr) {
        return;
    }

    std::cout << currentNode->data.getName() << " ";

    for (Node* child : currentNode->children) {
        printAllNodes(child);
    }
}

}   //namespace LinuxEmulator

#endif   //LINUX_EMULATOR_GTREE_H
