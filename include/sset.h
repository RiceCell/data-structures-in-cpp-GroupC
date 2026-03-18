// NODE FOR SKIPLISTS
template <typename T>
class SkipNode {
    public:
        T data;
        size_t height;
        SkipNode **next;

        // for sentinel
        SkipNode(size_t height) : height(height) {
            next = new SkipNode*[height];

            for (size_t i = 0; i < height; i++) {
                next[i] = nullptr;
            }
        }

        SkipNode(T x, size_t height) : data(x), height(height) {
            next = new SkipNode*[height];

            for (size_t i = 0; i < height; i++) {
                next[i] = nullptr;
            }
        }

        ~SkipNode() {
            delete[] next;
        }
};

// Red-Black Tree Node
template <typename T>
struct RBTNode {
    T data;
    bool isBlack;
    RBTNode<T> *left = nullptr;
    RBTNode<T> *right = nullptr;
    RBTNode<T> *parent = nullptr;

    RBTNode(const T &x, bool isB = false) : data(x), isBlack(isB) {};
};