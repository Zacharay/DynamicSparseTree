#include <iostream>
struct Node {
    int val;
    Node** childNodes;

    Node(int value) : val(value),  childNodes(nullptr) {}

    ~Node() {
        // if (childNodes) {
        //     for (int i = 0; i < numChildren; ++i) {
        //         delete childNodes[i];
        //     }
        //     delete[] childNodes;
        // }
    }

    void ensureChildArrayAllocated(int numChildren) {
        if (childNodes == nullptr) {
            childNodes = new Node*[numChildren];
            for (int i = 0; i < numChildren; ++i) {
                childNodes[i] = nullptr;
            }
        }
    }

    void addChildAt(int index, Node* newChild,int numChildren) {

        ensureChildArrayAllocated(numChildren);

        if (childNodes[index] == nullptr) {
            childNodes[index] = newChild;
        } else {
            std::cout << "Position already occupied!" << std::endl;
        }
    }

    void printChildren(int numChildren) {
        std::cout << "Children of node " << val << ": ";
        if (!childNodes) {
            std::cout << "no children" << std::endl;
            return;
        }

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
    Node* m_root;
    int n;
    int k;

public:
    DynamicSparseTree(int nVal, int kVal) : n(nVal), k(kVal), m_root(nullptr) {}

    void insert(int key) {
        if (!m_root) {
            m_root = new Node(key);
            return;
        }

        m_root->ensureChildArrayAllocated(n);
        Node* currentNode = m_root->childNodes[key % n];

        if (!currentNode) {
            m_root->addChildAt(key % n, new Node(key ),n);
            return;
        }

        int currentKey = key / n;
        while (currentNode) {
            currentNode->ensureChildArrayAllocated(k);

            if (!currentNode->childNodes[currentKey % k]) {
                currentNode->addChildAt(currentKey % k, new Node(key),k);
                return;
            }

            currentNode = currentNode->childNodes[currentKey % k];
            currentKey /= k;
        }
    }

    bool doesExist(int key) {
        if (!m_root) return false;
        if (m_root->val == key) return true;
        if (!m_root->childNodes) return false;

        Node* currentNode = m_root->childNodes[key % n];
        if (currentNode && currentNode->val == key) return true;

        int currentKey = key / n;
        while (currentNode && currentNode->childNodes) {
            currentNode = currentNode->childNodes[currentKey % k];
            if (!currentNode) return false;
            if (currentNode->val == key) return true;
            currentKey /= k;
        }
        return false;
    }

    Node* findNodeWithKey(int key) {

    }

    Node* findParent(Node*& parent,int key, int &pNodeChildren) {
        if(!m_root)return nullptr;

        if(m_root->val == key) {
            pNodeChildren = n;
            parent = nullptr;
            return m_root;
        }

        int currentKey = key ;
        Node *currentNode = m_root->childNodes[currentKey % n];
        currentKey /= n;

        if (currentNode && currentNode->val == key) {
            parent = m_root;
            pNodeChildren = k;
            return currentNode;
        }
        while (currentNode && currentNode->childNodes) {
            parent = currentNode;
            currentNode = currentNode->childNodes[currentKey % k];

            if (currentNode->val == key) {
                pNodeChildren = k;
                return currentNode;
            }
            currentKey /= k;
        }
        return currentNode;
    }

    int getLeftMostChildIndex(Node *node,int numChildren) {
        if (!node) return -1;

        for(int i=0;i<numChildren;i++) {
            if(node->childNodes && node->childNodes[i]) {
                return i;
            }
        }
        return -1;
    }

    void deleteKey(int key) {
        int pNodeChildren;

        Node *pNodeParent = nullptr;
        Node *pNode = findParent(pNodeParent,key, pNodeChildren);

        Node *qNode = pNode;
        int qNodeChildIndex = getLeftMostChildIndex(qNode, pNodeChildren);
        if(qNodeChildIndex == -1) {

            if(pNodeParent) {
                int pNodeParentChildren = pNodeParent==m_root ? n : k;
                for(int i=0;i<pNodeParentChildren;i++) {
                    if(pNodeParent->childNodes[i] && pNodeParent->childNodes[i]->val == key) {
                        delete pNodeParent->childNodes[i];
                        pNodeParent->childNodes[i] = nullptr;
                    }
                }
            }
            else {
                m_root = nullptr;
                delete qNode;
            }

            return;
        }



        while(qNodeChildIndex != -1) {
            pNodeParent = qNode;
            qNode = qNode->childNodes[qNodeChildIndex];
            qNodeChildIndex = getLeftMostChildIndex(qNode, k);
        }



        pNode->val = qNode->val;

        for(int i=0;i<k;i++) {
            if(pNodeParent->childNodes && pNodeParent->childNodes[i] && pNodeParent->childNodes[i]->val == key) {
                pNodeParent->childNodes[i] = nullptr;
                delete pNodeParent->childNodes[i];
            }
        }

    }


    void printAllChildren(Node* node) {
        // if (!node) return;
        // if (!node->childNodes) return;
        //
        // for (int i = 0; i < node->numChildren; ++i) {
        //     if (node->childNodes[i]) {
        //         std::cout << node->childNodes[i]->val << " ";
        //     }
        // }
    }

    void printTree(Node* node, int t) {

        std::cout<<node->val<<" children: ";

        if(!node->childNodes) {
            std::cout<<std::endl;
            return;
        }

        for(int i=0;i<t;i++) {
            if(node->childNodes[i]) {
                std::cout << node->childNodes[i]->val << " ";
            }
            else {
                std::cout<<"nullptr ";
            }
        }
        std::cout<<std::endl;

        for(int i=0;i<t;i++) {
            if(node&&node->childNodes&&node->childNodes[i]) {
                printTree(node->childNodes[i], k);
            }

        }

    }

    void printDebug() {
        std::cout<<"Printing ------------- "<<std::endl;
        printTree(m_root,8);
        std::cout<<"---------------"<<std::endl;
    }
    void printInorder(Node *currentNode=nullptr) {
        if(currentNode == nullptr) {
            printInorder(m_root);
            return;
        }
        std::cout<<currentNode->val<<" ";

        int numChildren = m_root==currentNode ? n : k;

        for(int i=0;i<numChildren;i++) {
            if(currentNode->childNodes && currentNode->childNodes[i] != nullptr) {
                printInorder(currentNode->childNodes[i]);
            }
        }
    }
};
int main() {

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
                if(tree.doesExist(number)) {
                    std::cout<<number<<" exist"<<'\n';
                }
                else {
                    tree.insert(number);
                }

                break;
            case 'P':
                tree.printInorder();
                std::cout<<std::endl;
                break;
            case 'L':
                int val;
                std::cin>>val;

                if(tree.doesExist(val)) {
                    std::cout<<val<<" exist"<<'\n';
                }
                else {
                    std::cout<<val<<" not exist"<<'\n';
                }

            break;
            case 'D':
                int key;
                std::cin>>key;
                if(tree.doesExist(key)) {
                    tree.deleteKey(key);
                }
                else {
                    std::cout<<key<<" not exist"<<'\n';
                }

                break;
            case 'Z':
                tree.printDebug();
                break;
            default:
                std::cout<<"Wrong command!"<<std::endl;
            break;
        }
    }


    return 0;
}
