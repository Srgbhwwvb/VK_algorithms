//Выведите элементы в порядке level - order(по слоям, “в ширину”).
#include <iostream>
#include <queue>

using namespace std;

template<typename T>
class isLess {
public:
    bool operator()(const T& left, const T& right) { return left <= right; }
};

template <class T, class Comparator = isLess<T>>
class BinaryTree {
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node(T p_value = 0, Node* p_left = nullptr, Node* p_right = nullptr) {
            value = p_value;
            left = p_left;
            right = p_right;
        }
    };


public:
    BinaryTree(const BinaryTree<T>& tree) = delete;
    BinaryTree& operator=(const BinaryTree& tree) = delete;

    BinaryTree(Comparator compare = Comparator()) :
        compare(compare), root(nullptr) {
    }

    ~BinaryTree() {
        destroy_tree();
    }

    void insert(int value) {
        if (!root) {
            root = new Node{ value, nullptr, nullptr };
        }
        else {
            insertNode(root, value);
        }
    }

    void breadthFirstTraversal() {
        if (!root) {
            return;
        }

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            cout << node->value << " ";

            if (node->left) {
                q.push(node->left);
            }
            if (node->right) {
                q.push(node->right);
            }
        }
        cout << endl;
    }

    void destroy_tree() {
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();

            if (node->left) {
                q.push(node->left);
            }
            if (node->right) {
                q.push(node->right);
            }

            delete node;
        }

        root = nullptr;
    }

private:
    Node* root;
    Comparator compare;
    void insertNode(Node*& node, int value) {
        if (value < node->value) {
            if (node->left) {
                insertNode(node->left, value);
            }
            else {
                node->left = new Node{ value, nullptr, nullptr };
            }
        }
        else {
            if (node->right) {
                insertNode(node->right, value);
            }
            else {
                node->right = new Node{ value, nullptr, nullptr };
            }
        }
    }
};

int main() {
    BinaryTree<int>binary_tree;
    unsigned int n = 0;
    cin >> n;
    int i = 0;
    while (n > 0) {
        cin >> i;
        binary_tree.insert(i);
        n--;
    }
    vector<int>answer;
    binary_tree.breadthFirstTraversal();
    


    return 0;
}