#include <iostream>

template <class type>
class LinkedList {
private:
    // Struct node of the list
    struct Node {
        Node *p_next;
        type value;

        explicit Node(type val) {
            value = val;
            p_next = nullptr;
        }
    };

    Node *p_head_;
    Node *p_tail_;
    int len_;

    void init() {
        p_head_ = nullptr;
        p_tail_ = nullptr;
        len_ = 0;
    }

    void copyValues(LinkedList const &obj) {
        // Copying values
        Node* p_node = obj.p_head_;
        while (p_node != nullptr)
        {
            insertEnd(p_node -> value);
            p_node = p_node -> p_next;
        }
    }

    // Validate position
    void checkPos (int position) {
        if (position < 0 || position >= len_) {
            throw "Incorrect position";
        }
    }

    // Get node pointer
    Node *getNode(int position) {
        checkPos(position);

        Node *p_node = p_head_;
        while (position--) {
            p_node = p_node -> p_next;
        }
        return p_node;
    }

    // Insert value in the end
    void insertEnd(type value) {
        Node *p_new_node = new Node(value);

        if (p_tail_ != nullptr) {
            p_tail_ -> p_next = p_new_node;
        } else {
            p_head_ = p_new_node;
        }

        p_tail_ = p_new_node;
        len_++;
    }

    // Print list
    void print(std::ostream &stream = std::cout) {
        Node *p_node = p_head_;

        stream << "{";

        if (p_node != nullptr) {
            while (p_node -> p_next != nullptr) {
                std::cout << p_node -> value << ", ";
                p_node = p_node -> p_next;
            }
            stream << p_node -> value;
        }

        stream << "}" << std::endl;
    }

public: // Constructors block
    // Constructor
    LinkedList() {
        init();
    }

    // Copy constructor
    LinkedList(LinkedList const &obj) {
        init();
        copyValues(obj);
    }

    // Сonstructor with initializer_list
    LinkedList(std::initializer_list<type> arr) {
        init();

        // Copying values
        for (type elem : arr) {
            insertEnd(elem);
        }
    }

    // Destructor
    ~LinkedList() {
        clearList();
    }

public: // Overload operators block
    // Subscript operator
    type &operator[](type index) {
        return getNode(index) -> value;
    }

    // Assigned operator
    LinkedList &operator=(LinkedList const &obj) {
        if (this != &obj) {
            // Deleting list data
            clearList();

            // Copying values
            copyValues(obj);

            return *this;
        }
    }

    // Plus operator
    LinkedList operator+(LinkedList const &obj) {
            LinkedList<type> new_list = *this;

            // Copying values
            new_list.copyValues(obj);

            return new_list;
    }

    // Assigned operator with initializer_list
    LinkedList &operator=(std::initializer_list<type> arr) {
        // Deleting list data
        clearList();

        // Copying values
        for (type elem : arr) {
            insertEnd(elem);
        }

        return *this;
    }

    // Print to steam operator
    friend std::ostream &operator<<(std::ostream &stream,
                                    LinkedList &obj) {
        obj.print(stream);
        return stream;
    }
    
public: // Public methods block
    // Get length
    int len() {
        return len_;
    }

    bool contain(type elm) {
        //TODO:
    }

    int find(type elm) {
        //TODO:
    }

    // Insert value
    type insert(type value, int position) {
        if (position == len_) {
            insertEnd(value);
            return value;
        }

        checkPos(position);
        Node *p_new_node = new Node(value);

        if (position == 0) {
            p_new_node -> p_next = p_head_;
            p_head_ = p_new_node;
        } else {
            Node *p_prev_node = getNode(position - 1);
            p_new_node -> p_next = p_prev_node -> p_next;
            p_prev_node -> p_next = p_new_node;
        }

        len_++;

        return value;
    }

    // Remove value
    type remove(int position) {
        Node *p_rm_node;
        type value;

        if (position == 0) {
            p_rm_node = p_head_;
            p_head_ = p_head_ -> p_next;
        } else {
            Node *p_prev_node = getNode(position - 1);
            p_rm_node = p_prev_node -> p_next;
            p_prev_node -> p_next = p_rm_node -> p_next;
        }

        value = p_rm_node -> value;
        delete p_rm_node;
        len_--;
        return value;
    }

    // Remove list elements
    void clearList() {
        Node *p_prev_node;

        while (p_head_ != nullptr) {
            p_prev_node = p_head_;
            p_head_ = p_head_ -> p_next;
            delete p_prev_node;
            len_--;
        }
        p_tail_ = nullptr;
    }
};
