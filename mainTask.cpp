#include <chrono>
#include <iostream>
#include <vector>

//pseudorandom number generator to fill up tree
size_t getUniqNumber() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

//Node of binary search tree
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int data) {
        this->data = data;
        left = NULL;
        right = NULL;
    }
};

//Binary search tree
class BST {
private:
    BSTNode* root;
    //supporting function for the copy constructor
    void copy(const BSTNode* parentNode, BSTNode*& childNode) {
        if (parentNode == NULL) {
            childNode = NULL;
            return;
        }
        childNode = new BSTNode(parentNode->data);
        copy(parentNode->left, childNode->left);
        copy(parentNode->right, childNode->right);
    }

    //supporting function for the destructor
    void clear(BSTNode*& nodeToDelete) {
        if (nodeToDelete == NULL) {
            return;
        }
        clear(nodeToDelete->left);
        clear(nodeToDelete->right);
        delete nodeToDelete;
    }

    //supporting function for the print
    void print(BSTNode* node) {
        if (node == NULL) {
            return;
        }
        print(node->left);
        std::cout << node->data << " ";
        print(node->right);
    }

    //supporting function for the insert
    bool insert(BSTNode*& node, int data) {
        if (node == NULL) {
            node = new BSTNode(data);
            return true;
        }
        else if (data < node->data) {
            return insert(node->left, data);
        }
        else if (data > node->data) {
            return insert(node->right, data);
        }
        return false;
    }

    //supporting function for the contains
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

    //function to find min element
    BSTNode* findMin(BSTNode* node) {
        while (node->left != NULL) {
            node = node->left;
            return node;
        }
    }

    //supporting function for the erase
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
        else {
            //no child
            if (node->left == NULL && node->right == NULL) {
                delete node;
                node = NULL;
            }
            //one child
            else if (root->left==NULL){
                BSTNode* temp = node;
                node = node->right;
                delete temp;
            }
            else if (root->right == NULL) {
                BSTNode* temp = node;
                node = node->left;
                delete temp;
            }
            //two children
            else {
                BSTNode* temp = findMin(node->right);
                node->data = temp->data;
                node->right = erase(node->right, temp->data);
            }
        }
        return node;
    }

    //supporting function for the set intersection
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

    ////supporting function for the set difference
    void difference(BSTNode* node, BST& bst, BST& resultBST) {
        if (node == NULL) {
            return;
        }
        difference(node->left, bst, resultBST);
        if (!bst.contains(node->data)) {
            resultBST.insert(node->data);
        }
        difference(node->right, bst, resultBST);
    }

public:

    //constructor
    BST() {
        root = NULL;
    }

    //copy constructor with using copy function
    BST(const BST& bst) {
        copy(bst.root, root);
    }

    //destructor
    ~BST() {
        clear(root);
    }

    //assignment operator
    BST& operator=(const BST& bst) {
        if (this == &bst) {
            return *this;
        }
        clear(root);
        copy(bst.root, root);
        return *this;
    }

    //print function
    void print() {
        print(root);
        std::cout << std::endl;
    }

    //insert function
    bool insert(int data) {
        return insert(root, data);
    }

    //function that search element in BST
    bool contains(int data) {
        return contains(root, data);
    }

    //erase function
    bool erase(int data) {
        if (!contains(data)) {
            return false;
        }
        root = erase(root, data);
        return true;
    }

    //function that filling up tree with pseudorandom numbers
    void fillUpTree(int uniqNumbersQuantity) {
        for (int i = 0; i < uniqNumbersQuantity; i++)
        {
            insert(getUniqNumber());
        }
    }

    //function that returns the intersection of 2 sets of integers
    BST& setIntersection(BST& bst) {
        BST* resultBST = new BST();
        intersection(root, bst, *resultBST);
        return *resultBST;
    }

    //function that returns the difference of 2 sets of integers
    BST& setDifference(BST& deductibleBST) {
        BST* resultBST = new BST();
        difference(root, deductibleBST, *resultBST);
        return *resultBST;
    }
};

//function returns the average time of tree filling with 100 attempts
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
    std::cout<< "average time of filling up tree with "<< uniqNumbersQuantity<<" numbers: " << averageTime/100 << " microseconds" << std::endl;
}

//function returns the average time of searching random value in tree  with 1000 attempts
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

//function returns the average time of std::vector<int> filling with 100 attempts
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

//function returns the average time of searching random value in std::vector<int> with 1000 attempts
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

//main
int main()
{
    bool programm_runner = true;
    size_t programmAction;
    while (programm_runner) {
        std::cout << "User interface for working with binary trees" << std::endl;
        std::cout << "1) Create two sets and output their intersection." << std::endl;
        std::cout << "2) Create two sets and output their difference." << std::endl;
        std::cout << "3) Find out the average time of filling trees with 1000 10000 and 100000 numbers." << std::endl;
        std::cout << "4) Find out the average time of searching value in tree with 1000 10000 and 100000 numbers." << std::endl;
        std::cout << "5) Find out the average time of filling vector with 1000 10000 and 100000 numbers." << std::endl;
        std::cout << "6) Find out the average time of searching value in vector with 1000 10000 and 100000 numbers." << std::endl;
        std::cout << "7) Exit programm." << std::endl;
        std::cin >> programmAction;

        switch (programmAction)
        {
        case 1:
        {
            system("cls");
            std::cout << "You have selected the creating two sets and output their intersection." << std::endl;
            std::cout << "Enter the number of nodes of the first tree:" << std::endl;
            BST bst1;
            int nodesNumber1;
            std::cin >> nodesNumber1;

            std::cout << "\nEnter the number of nodes of the second tree:" << std::endl;
            BST bst2;
            int nodesNumber2;
            std::cin >> nodesNumber2;

            std::cout << "\nAdd node values to the first tree:" << std::endl;
            int data;
            for (int i = 0; i < nodesNumber1; i++)
            {
                std::cin >> data;
                bst1.insert(data);
                std::cout << std::endl;
            }

            std::cout << "\nAdd node values to the second tree:" << std::endl;
            for (int i = 0; i < nodesNumber2; i++)
            {
                std::cin >> data;
                bst2.insert(data);
                std::cout << std::endl;
            }

            BST res = bst1.setIntersection(bst2);
            std::cout << "The intersection of these two sets is:" << std::endl;
            res.print();
            break; 
        }
        case 2: 
        {
            system("cls");
            std::cout << "You have selected the creating two sets and output their difference." << std::endl;
            std::cout << "Enter the number of nodes of the first tree:" << std::endl;
            BST bst1;
            int nodesNumber1;
            std::cin >> nodesNumber1;

            std::cout << "\nEnter the number of nodes of the second tree:" << std::endl;
            BST bst2;
            int nodesNumber2;
            std::cin >> nodesNumber2;

            std::cout << "\nAdd node values to the first tree:" << std::endl;
            int data;
            for (int i = 0; i < nodesNumber1; i++)
            {
                std::cin >> data;
                bst1.insert(data);
                std::cout << std::endl;
            }

            std::cout << "\nAdd node values to the second tree:" << std::endl;
            for (int i = 0; i < nodesNumber2; i++)
            {
                std::cin >> data;
                bst2.insert(data);
                std::cout << std::endl;
            }

            BST res = bst1.setDifference(bst2);
            std::cout << "The difference of these two sets is:" << std::endl;
            res.print();
            break; 
        }
        case 3:
        {
            system("cls");
            std::cout << "You have selected the finding out the average time of filling trees with 1000 10000 and 100000 numbers." << std::endl;
            std::cout << "Let's measure how long the tree is filling up:" << std::endl;
            averageFillingTime(1000);
            averageFillingTime(10000);
            averageFillingTime(100000);
            break;
        }
        case 4:
        {
            system("cls");
            std::cout << "You have selected the finding out the average time of searching value in tree with 1000 10000 and 100000 numbers." << std::endl;
            std::cout << "Let's measure how long the tree is searching the value:" << std::endl;
            averageSearchingTime(1000);
            averageSearchingTime(10000);
            averageSearchingTime(100000);
            break;
        }
        case 5:
        {
            system("cls");
            std::cout << "You have selected the finding out the average time of filling vector with 1000 10000 and 100000 numbers." << std::endl;
            std::cout << "Let's measure how long the vector is filling up:" << std::endl;
            averageFillingTimeVector(1000);
            averageFillingTimeVector(10000);
            averageFillingTimeVector(100000);
            break;
        }
        case 6:
        {
            system("cls");
            std::cout << "You have selected the finding out the average time of searching value in vector with 1000 10000 and 100000 numbers." << std::endl;
            std::cout << "Let's measure how long the vector is searching the value:" << std::endl;
            averageSearchingTimeVector(1000);
            averageSearchingTimeVector(10000);
            averageSearchingTimeVector(100000);
            break;
        }
        case 7:
        {
            programm_runner = false;
            system("cls");
            std::cout << "You have exited the program." << std::endl;
            break;
        }
        default:
        {
            system("cls");
            std::cout << "You have selected non - existent option, try it again." << std::endl;
            break;
        }
        }
        system("pause");
        system("cls");
    }

}

    //BST bst1;
    //BST bst2;

    //bst1.insert(1);
    //bst1.insert(2);
    //bst1.insert(3);
    //bst1.insert(4);
    //bst1.insert(5);
    //bst1.insert(9);

    //bst2.insert(6);
    //bst2.insert(7);
    //bst2.insert(8);
    //bst2.insert(9);
    //bst2.insert(3);
    //bst2.insert(4);

    //BST res1;
    //res1 = bst1.setIntersection(bst2);
    //std::cout << "bst1:\n" << "\n\n";
    //bst1.print();
    //std::cout << "\n\n";


    //std::cout << "bst2:\n" << "\n\n";
    //bst2.print();
    //std::cout << "\n\n";


    //std::cout << "res1:\n" << "\n\n";
    //res1.print();
    //std::cout << "\n\n";


    //BST res2;
    //res2 = bst1.setDifference(bst2);
    //std::cout << "res2:\n" << "\n\n";
    //res2.print();
    //std::cout << "\n\n";

    //std::cout << "Hello, it's Leon's programm.\nTime to measure how long the tree is filling up:" << std::endl;
    //averageFillingTime(1000);
    //averageFillingTime(10000);
    //averageFillingTime(100000);
    //std::cout << "\n" << std::endl;
    //std::cout << "Hello, it's Leon's programm.\nTime to measure how long the tree is searching the value:" << std::endl;
    //averageSearchingTime(1000);
    //averageSearchingTime(10000);
    //averageSearchingTime(100000);
     
    
    //std::cout << "Hello, it's Leon's programm.\nTime to measure how long the vector is filling up:" << std::endl;
    //averageFillingTimeVector(1000);
    //averageFillingTimeVector(10000);
    //averageFillingTimeVector(100000);
    //std::cout << "\n" << std::endl;
    //std::cout << "Hello, it's Leon's programm.\nTime to measure how long the vector is searching the value:" << std::endl;
    //averageSearchingTimeVector(1000);
    //averageSearchingTimeVector(10000);
    //averageSearchingTimeVector(100000);
  

