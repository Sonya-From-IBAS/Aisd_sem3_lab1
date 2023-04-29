#include <chrono>
#include <iostream>
#include <vector>


//!!!comments are added only where the original classes and functions were changed!!!
size_t getUniqNumber() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

//add quantity - number of duplicate values
struct BSTNode {
    int data;
    int quantity;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int data, int quantity=1) {
        this->data = data;
        left = NULL;
        right = NULL;
        this->quantity = quantity;
    }
};

class BST {
private:
    BSTNode* root;
    void copy(const BSTNode* parentNode, BSTNode*& childNode) {
        if (parentNode == NULL) {
            childNode = NULL;
            return;
        }
        childNode = new BSTNode(parentNode->data, parentNode->quantity); //added a second argument for the constructor
        copy(parentNode->left, childNode->left);
        copy(parentNode->right, childNode->right);
    }

    void clear(BSTNode*& nodeToDelete) {
        if (nodeToDelete == NULL) {
            return;
        }
        clear(nodeToDelete->left);
        clear(nodeToDelete->right);
        delete nodeToDelete;
    }
    //return number of elements which equal data
    int getElemQuantity(BSTNode* node, int data) {
        if (node->data == data) {
            return node->quantity;
        }
        else if (data < node->data) {
            return getElemQuantity(node->left, data);
        }
        else if (data > node->data) {
            return getElemQuantity(node->right, data);
        }

    }

    void print(BSTNode* node) {
        if (node == NULL) {
            return;
        }
        print(node->left);
        for (int i = 0; i < node->quantity; i++) //added a <for> to output all duplicate elements
        {
            std::cout << node->data << " ";
        }
        print(node->right);
    }

    void insert(BSTNode*& node, int data) {//changed the return value
        if (node == NULL) {
            node = new BSTNode(data);
            return;
        }
        else if (data < node->data) {
            return insert(node->left, data);
        }
        else if (data > node->data) {
            return insert(node->right, data);
        }
        else {//added one more condition
            node->quantity += 1;
            return;
        }
    }

    bool contains(BSTNode* node, int data) {
        if (node == NULL) {
            return false;
        }
        else if (data < node->data) {
            return contains(node->left, data);
        }
        else if (data > node->data) {
            return contains(node->right, data);
        }
        return true;
    }

    BSTNode* findMin(BSTNode* node) {
        while (node->left != NULL) {
            node = node->left;
            return node;
        }
    }

    BSTNode* erase(BSTNode* node, int data) {
        if (node == NULL) {
            return node;
        }
        else if (data < node->data) {
            node->left = erase(node->left, data);
        }
        else if (data > node->data) {
            node->right = erase(node->right, data);
        }
        else {//added a check for the number of values to each condition
            if (node->left == NULL && node->right == NULL) {
                if (node->quantity > 1) {
                    node->quantity -= 1;
                }
                else {
                    delete node;
                    node = NULL;
                }
            }
            else if (root->left == NULL) {
                if (node->quantity > 1) {
                    node->quantity -= 1;
                }
                else {
                    BSTNode* temp = node;
                    node = node->right;
                    delete temp;
                }
            }
            else if (root->right == NULL) {
                if (node->quantity > 1) {
                    node->quantity -= 1;
                }
                else {
                    BSTNode* temp = node;
                    node = node->left;
                    delete temp;
                }
            }
            else {
                if (node->quantity > 1) {
                    node->quantity -= 1;
                }
                else {
                    BSTNode* temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = erase(node->right, temp->data);
                }
            }
        }
        return node;
    }

    void intersection(BSTNode* node, BST& bst, BST& resultBST) {
        if (node == NULL) {
            return;
        }
        intersection(node->left, bst, resultBST);
        if (bst.contains(node->data)) {
            resultBST.insert(node->data);
        }
        intersection(node->right, bst, resultBST);
    }


    void difference(BSTNode* node, BST& bst, BST& resultBST) {
        if (node == NULL) {
            return;
        }
        difference(node->left, bst, resultBST);
        if (!bst.contains(node->data) || node->quantity>bst.getElemQuantity(bst.root, node->data)) {//added one more condition
            resultBST.insert(node->data);
        }
        difference(node->right, bst, resultBST);
    }

public:

    BST() {
        root = NULL;
    }

    BST(const BST& bst) {
        copy(bst.root, root);
    }

    ~BST() {
        clear(root);
    }

    BST& operator=(const BST& bst) {
        if (this == &bst) {
            return *this;
        }
        clear(root);
        copy(bst.root, root);
        return *this;
    }

    void print() {
        print(root);
        std::cout << std::endl;
    }

    void insert(int data) {//changed the return value
        insert(root, data);
        return;
    }


    bool contains(int data) {
        return contains(root, data);
    }

    bool erase(int data) {
        if (!contains(data)) {
            return false;
        }
        root = erase(root, data);
        return true;
    }


    void fillUpTree(int uniqNumbersQuantity) {
        for (int i = 0; i < uniqNumbersQuantity; i++)
        {
            insert(getUniqNumber());
        }
    }

    BST& setIntersection(BST& bst) {
        BST* resultBST = new BST();
        intersection(root, bst, *resultBST);
        return *resultBST;
    }

    BST& setDifference(BST& deductibleBST) {
        BST* resultBST = new BST();
        difference(root, deductibleBST, *resultBST);
        return *resultBST;
    }
};

void averageFillingTime(int uniqNumbersQuantity) {
    double averageTime = 0;
    for (int i = 0; i < 100; i++)
    {
        BST bst;
        auto begin = std::chrono::steady_clock::now();
        bst.fillUpTree(uniqNumbersQuantity);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        averageTime += duration.count();
    }
    std::cout << "average time of filling up tree with " << uniqNumbersQuantity << " numbers: " << averageTime / 100 << " microseconds" << std::endl;
}

void averageSearchingTime(int uniqNumbersQuantity) {
    double averageTime = 0;
    for (int i = 0; i < 1000; i++)
    {
        BST bst;
        bst.fillUpTree(uniqNumbersQuantity);
        auto begin = std::chrono::steady_clock::now();
        bst.contains(getUniqNumber());
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        averageTime += duration.count();
    }
    std::cout << "average time of searching random value in tree in " << uniqNumbersQuantity << " numbers: " << averageTime / 1000 << " microseconds" << std::endl;
}

void averageFillingTimeVector(int uniqNumbersQuantity) {
    double averageTime = 0;
    for (int i = 0; i < 100; i++)
    {
        std::vector<int> vect;
        auto begin = std::chrono::steady_clock::now();
        for (int i = 0; i < uniqNumbersQuantity; i++)
        {
            vect.push_back(getUniqNumber());
        }
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        averageTime += duration.count();
    }
    std::cout << "average time of filling up std::vector with " << uniqNumbersQuantity << " numbers: " << averageTime / 100 << " microseconds" << std::endl;
}

void averageSearchingTimeVector(int uniqNumbersQuantity) {
    double averageTime = 0;
    for (int i = 0; i < 1000; i++)
    {
        std::vector<int> vect;
        for (int i = 0; i < uniqNumbersQuantity; i++)
        {
            vect.push_back(getUniqNumber());
        }
        auto begin = std::chrono::steady_clock::now();
        std::find(vect.begin(), vect.end(), getUniqNumber());
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        averageTime += duration.count();
    }
    std::cout << "average time of searching random value in std::vector in " << uniqNumbersQuantity << " numbers: " << averageTime / 1000 << " microseconds" << std::endl;
}