#include <iostream>
#include <vector>

using namespace std;

template<typename T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) const {
        if (l < r)
            return 1;
        else if (l > r)
            return -1;
        else
            return 0;
    }
};

template <typename Key, typename Comparator = DefaultComparator<Key>>
class BalancedTree {
    struct Node {
        Key key;
        Node* leftChild;
        Node* rightChild;
        int height; // node height
        int nodeCount; // node count

        Node(const Key& key) :
            leftChild(nullptr),
            rightChild(nullptr),
            key(key),
            height(1),
            nodeCount(1) {
        }
    };

public:
    BalancedTree(Comparator comparator = Comparator()) : root(nullptr), comparator(comparator) {}

    ~BalancedTree() {
        destroyTree(root);
    }

    bool contains(const Key& key) const {
        return findKey(key, root) != nullptr;
    }

    void add(const Key& key) {
        root = insertNode(key, root);
    }

    void remove(const Key& key) {
        root = removeNode(findKey(key), root);
    }

    int getNodeNumber(const Key& key) const {
        return getNodeNumber(key, root);
    }

private:
    void destroyTree(Node* node) {
        if (node->leftChild)
            destroyTree(node->leftChild);
        if (node->rightChild)
            destroyTree(node->rightChild);
        delete node;
    }

    int findKey(int k) {
        Node* node = root;
        while (node) {
            int temp = node->leftChild ? node->leftChild->nodeCount : 0;
            if (temp == k)
                return node->key;
            if (temp > k)
                node = node->leftChild;
            else {
                node = node->rightChild;
                k = k - 1 - temp;
            }
        }
        return -1;
    }

    int getNodeNumber(const Key& key, Node* node) const {
        if (!node)
            return -1;
        int comparison = comparator(key, node->key);
        if (comparison == 0) {
            return node->leftChild ? node->leftChild->nodeCount : 0;
        }
        if (comparison < 0) {
            return getNodeNumber(key, node->leftChild);
        }
        int leftCount = node->leftChild ? node->leftChild->nodeCount : 0;
        return leftCount + 1 + getNodeNumber(key, node->rightChild);
    }

    Node* removeNode(const Key& key, Node* node) {
        if (!node)
            return nullptr;
        int res = comparator(key, node->key);
        if (res == -1)
            node->leftChild = removeNode(key, node->leftChild);
        else if (res == 1)
            node->rightChild = removeNode(key, node->rightChild);
        else {
            Node* left = node->leftChild;
            Node* right = node->rightChild;

            delete node;

            if (!right)
                return left;

            Node* min_node;
            right = findandRemoveMin(right, min_node);
            min_node->rightChild = right;
            min_node->leftChild = left;

            return balance(min_node);
        }
        return balance(node);
    }
    Node* findandRemoveMin(Node* node, Node*& min) {
        if (node->leftChild == nullptr) {
            min = node; //запоминаем минимальный узел текущего поддерева
            return node->rightChild;
        }
        node->leftChild = findandRemoveMin(node->leftChild, min);
        return balance(node);
    }
    /*Node* findфMin(Node* node) {
        return node->leftChild ? findMin(node->leftChild) : node;
    }

    Node* removeMin(Node* node) {
        if (node->leftChild == nullptr)
            return node->rightChild;
        node->leftChild = removeMin(node->leftChild);
        return balance(node);
    }*/

    Node* insertNode(const Key& key, Node* node) {
        if (!node)
            return new Node(key);
        int comparison = comparator(key, node->key);
        if (comparison < 0)
            node->leftChild = insertNode(key, node->leftChild);
        else if (comparison > 0)
            node->rightChild = insertNode(key, node->rightChild);
        else
            return node;
        return balance(node);
    }

    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    int getNodeCount(Node* node) {
        return node ? node->nodeCount : 0;
    }

    int getBalanceFactor(Node* node) {
        return getHeight(node->rightChild) - getHeight(node->leftChild);
    }

    void fixHeight(Node* node) {
        if (!node)
            return;
        node->height = max(getHeight(node->leftChild), getHeight(node->rightChild)) + 1;
        node->nodeCount = getNodeCount(node->leftChild) + getNodeCount(node->rightChild) + 1;
    }

    Node* rotateLeft(Node* node) {
        Node* temp = node->rightChild;
        node->rightChild = temp->leftChild;
        temp->leftChild = node;
        fixHeight(node);
        fixHeight(temp);
        return temp;
    }

    Node* rotateRight(Node* node) {
        Node* temp = node->leftChild;
        node->leftChild = temp->rightChild;
        temp->rightChild = node;
        fixHeight(node);
        fixHeight(temp);
        return temp;
    }

    Node* balance(Node* node) {
        fixHeight(node);
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor == 2) {
            if (getBalanceFactor(node->rightChild) < 0) {
                node->rightChild = rotateRight(node->rightChild);
            }
            return rotateLeft(node);
        }
        else if (balanceFactor == -2) {
            if (getBalanceFactor(node->leftChild) > 0) {
                node->leftChild = rotateLeft(node->leftChild);
            }
            return rotateRight(node);
        }
        return node;
    }

    Node* root;
    Comparator comparator;
};

int main() {
    BalancedTree<int> tree;
    int n, command, value;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> command >> value;
        if (command == 1) {
            tree.add(value);
            cout << tree.getNodeNumber(value) << endl;
        }
        else if (command == 2) {
            tree.remove(value);
        }
        else
            return 0;
    }
    return 0;
}