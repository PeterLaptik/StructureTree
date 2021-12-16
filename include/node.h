#ifndef NODE_H
#define NODE_H

#include <vector>
#include <map>

#define DEBUG_MODE

#ifdef DEBUG_MODE
    #include <iostream>
#endif // DEBUG_MODE

// Default level for each created node
static const int NODE_DEFAULT_LEVEL = 0;

// Default quantity
static const double NODE_DEFAULT_QUANTITY = 1.0;


///\brief Node is a container element for a product structure tree.
/// Contains pointers to item_t-elements.
/// Each node is able to have undefined quantity of a child nodes.
/// The node is responsible for its` item and children nodes:
/// i.e. node`s item and children are deleting when the node is being deleted.
template <class T>
class Node
{
    // Item type
    typedef T item_t;

    public:
        /// Empty container-element constructor
        Node<T>()
                : m_parent(nullptr),
                m_level(NODE_DEFAULT_LEVEL),
                m_item(nullptr),
                m_quantity(NODE_DEFAULT_QUANTITY)
        { }

        /// Non-empty container-element constructor
        Node<T>(T *item)
                : m_parent(nullptr),
                m_level(NODE_DEFAULT_LEVEL),
                m_item(item),
                m_quantity(NODE_DEFAULT_QUANTITY)
        {
            UpdateLinkCounterForItem(item);
        }


        Node<T>(const Node<T>&) = delete;

        Node<T>(const Node<T>&&) = delete;

        Node<T>& operator=(const Node<T>&) = delete;


        /// Destructor: deletes children sub-trees and its` items
        virtual ~Node()
        {
            // remove handling item if the item has no more links in a counter map
            RemoveItem(m_item);
            // remove children nodes
            for(typename std::vector<Node*>::iterator it=m_children.begin(); it!=m_children.end(); ++it)
                delete (*it);
        }

        ///\brief Assigns item for the node
        /// TODO
        void AssignItem(T *item);

        ///\brief Append child node directly to the parent
        // TODO remove from previous parent
        Node<T>* AppendChild(Node<T> *node)
        {
            node->SetParent(this);
            m_children.push_back(node);
            return node;
        }

        ///\brief Creates a new node for the item
        /// Appends the node as a child node
        ///\return created node
        Node<T>* AppendChild(T *item)
        {
            Node<T> *node_to_add = new Node(item);
            node_to_add->SetParent(this);
            m_children.push_back(node_to_add);
            return node_to_add;
        }


        /// Returns parent node pointer
        /// The parent node cannot be changed from outside:
        /// parent is changed only when the node is added to the other node as a child
        Node<T>* GetParent(void)
        {
            return m_parent;
        }

        /// Checks whether the node has assigned item
        bool HasItem(void) const
        {
            return m_item!=nullptr;
        }

        /// Returns item from a container
        ///\return  pointer to item or null-pointer if the node does not contain item
        T* GetItem(void) const
        {
            return m_item;
        }

        /// Returns node level value
        ///\return node level in a tree
        int GetLevel(void) const
        {
            return m_level;
        }


        // Console output for debug
        // Recursive expanding overall structure
        // For testing and debugging nodes have to implement the interface ITestItem
        #ifdef DEBUG_MODE
            void ToConsole(void) const;
            static void LinkOutput(void);
        #endif // DEBUG_MODE


    protected:
        // Items cash
        // Keeps link counters for each pointer
        static std::map<T*, int> link_counter_map;


    private:
        // Node parent
        Node<T> *m_parent;

        // Node level number in a structure
        int m_level;

        // Assigned item for the node container
        T *m_item;

        // Quantity of items
        // The property is assigned to node
        double m_quantity;

        // Child elements of the node structure
        std::vector<Node*> m_children;

        // Sets node parent
        void SetParent(Node<T> *parent)
        {
            m_parent = parent;
            RefreshLevels();
        }

        // Sets node level
        // The method is called automatically on appending node as a child of the other node
        // to set level to (parent-level + 1)
        void SetLevel(int level)
        {
            m_level = level;
        }

        // Refreshes levels from current node
        // up to all sub-tree nodes
        void RefreshLevels()
        {
            if(m_parent==nullptr)
                return;

            m_level = m_parent->GetLevel() + 1;
            for(typename std::vector<Node*>::iterator it=m_children.begin(); it!=m_children.end(); ++it)
                (*it)->RefreshLevels();
        }


        // Creates link for the item in link counter cash
        // or increments existing link if the item has been added before
        void UpdateLinkCounterForItem(T *item)
        {
            typename std::map<T*, int>::iterator link_counter = Node<T>::link_counter_map.find(item);
            if(link_counter==link_counter_map.end())
                link_counter_map.insert(std::pair<T*, int>(item, 1));
            else
                (*link_counter).second++;
        }


        // Checks and refreshes link counter value for the item
        // Deletes the item if link counter is equal to 1
        // Any item has to be deleted via this method only
        void RemoveItem(T *item)
        {
            // Check and refresh link counter
            typename std::map<T*, int>::iterator it = link_counter_map.find(item);
            // Broken link check
            // If iterator==map.end then something is going wrong
            // Force deleting on broken link implemented
            if(it==link_counter_map.end())
            {
                delete item;
                return;
            }
            // If no more pointers to the item
            if(--(*it).second<1)
            {
                delete item;
                link_counter_map.erase(it); // remove link counter
            }
        }
};

// Static link counter
// Keeps pairs pointer to item - number of occurrences
template <typename T> std::map<T*, int> Node<T>::link_counter_map;



    // Testing and debugging routines
    #ifdef DEBUG_MODE
        // Console output for the structure tree
        template <typename T> void Node<T>::ToConsole(void) const
        {
            for(int i=0; i<m_level; i++)
                std::cout<<"- ";
            std::cout<< (m_item!=nullptr ? m_item->GetName() : "err_empty") <<std::endl;
            for(typename std::vector<Node*>::const_iterator it=m_children.begin(); it!=m_children.end(); ++it)
                (*it)->ToConsole();
        }

        // Output current values of link counters
        template <typename T> void Node<T>::LinkOutput(void)
        {
            std::cout<<"Current links:"<<std::endl;
            for(typename std::map<T*, int>::const_iterator it = Node<T>::link_counter_map.begin(); it!=Node<T>::link_counter_map.end(); ++it)
            {
                T* item = (*it).first;
                int number_of_links = (*it).second;
                std::cout<<item->GetName()<<": "<<number_of_links<<std::endl;
            }
        }
    #endif // DEBUG_MODE

#endif // NODE_H
