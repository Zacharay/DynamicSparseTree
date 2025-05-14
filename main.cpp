#include <iostream>

struct Node {
    int val;
    Node** childNodes;
    int numChildren;
    Node(int value, int numChildren) : val(value), numChildren(numChildren) {
        childNodes = new Node*[numChildren];
        for (int i = 0; i < numChildren; ++i) {
            childNodes[i] = nullptr;
        }
    }


    ~Node() {

        delete[] childNodes;
    }

    // Method to add a child node at a specific position
    void addChildAt(int index, Node* newChild) {
        if (index < 0 || index >= numChildren) {
            std::cout << "Index out of bounds!" << std::endl;
            return;
        }

        if (childNodes[index] == nullptr) {
            childNodes[index] = newChild;
        } else {
            std::cout << "Position already occupied!" << std::endl;
        }
    }

    // Method to print all child nodes
    void printChildren() {
        std::cout << "Children of node " << val << ": ";
        for (int i = 0; i < numChildren; ++i) {
            if (childNodes[i] != nullptr) {
                std::cout << childNodes[i]->val << " ";
            } else {
                std::cout << "nullptr ";
            }
        }
        std::cout << std::endl;
    }
};

class DynamicSparseTree {
public:
    DynamicSparseTree(int n,int k) {
        m_root = nullptr;
        this->n = n;
        this->k = k;
    }
    ~DynamicSparseTree() {
        if (m_root) {
            delete m_root;
        }
    }
    void insert(int key) {
        if(m_root == nullptr) {
            m_root = new Node(key,n);

            //m_root->printChildren();
            return;
        }


        Node *currentNode = m_root->childNodes[key % n];
        if(currentNode == nullptr) {
            m_root->addChildAt(key%n,new Node(key,k));
            //m_root->printChildren();
            return;
        }

        int currentKey = key/8;
        while(currentNode != nullptr) {
            if(currentNode->childNodes[currentKey % k] == nullptr) {
                currentNode->addChildAt(currentKey%k,new Node(key,k));
                //currentNode->printChildren();
                return;
            }

            currentNode = currentNode->childNodes[currentKey % k];
            currentKey = currentKey/4;
        }
    }
    void printInorder(Node *currentNode=nullptr) {
        if(currentNode == nullptr) {
            printInorder(m_root);
            return;
        }
        std::cout<<currentNode->val<<" ";
        for(int i=0;i<currentNode->numChildren;i++) {
            if(currentNode->childNodes[i] != nullptr) {
                printInorder(currentNode->childNodes[i]);
            }
        }
    }

    bool doesExist(int key) {
        if(m_root && m_root->val == key ) {
            return true;
        }

        Node *currentNode = m_root->childNodes[key % n];
        if(currentNode && currentNode->val == key ) {
            return true;
        }
        int currentKey = key/8;

        while(currentNode && currentNode->childNodes[currentKey % k] != nullptr) {
            if( currentNode->childNodes[currentKey % k]->val == key)return true;
            currentNode = currentNode->childNodes[currentKey % k];
            currentKey = currentKey/4;
        }

        return false;
    }

private:
    int n,k;
    Node* m_root;
};

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int numberOfInstructions;
    std::cin >> numberOfInstructions;
    int minVal,maxVal;
    int n, k;
    std::cin>>minVal>>maxVal;
    std::cin>>n>>k;
    std::cin.ignore();

    DynamicSparseTree tree(n,k);
    for(int i=0;i<numberOfInstructions;i++) {
        char command;
        std::cin>>command;
        switch(command) {
            case 'I':
                int number;
                std::cin>>number;
                tree.insert(number);
                break;
            case 'P':
                tree.printInorder();
                std::cout<<std::endl;
                break;
            case 'L':
                int val;
                std::cin>>val;

                if(tree.doesExist(val)) {
                    std::cout<<val<<" exists"<<std::endl;
                }
                else {
                    std::cout<<val<<" does not exist"<<std::endl;
                }

            break;
            case 'D':
                break;
            default:
                std::cout<<"Wrong command!"<<std::endl;
            break;
        }
    }


    return 0;
}
