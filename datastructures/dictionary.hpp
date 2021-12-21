#pragma once

#include <iostream>
#include <queue>

/*
 * [DONE] be able to define like a dictionary ( dict = { {a, b}, {c, d}, {e, f} }; dict({ {a, b}, {c, d}, {e, f} }); ) 
 * [DONE] be able to define as other dictionary 
 * [DONE] be able to add, remove, or modify any pairs from dictionary
 * [DONE] have overarching search() function returning pointer to node to see if node is in tree
 * [DONE] operators: [], <<
 ext:
 * [DONE] be able to re-optimize the tree for improved lookup times with tree permutations (linking and unlinking and reinserting subtrees)
 */

// needed for easy definition
template<typename TKey, typename TValue>
struct raw_pair
{
    TKey key;
    TValue value;
 
    inline raw_pair(TKey key, TValue value)
        : key(key), value(value)
    {

    }
};

// needed for binary tree structure
template<typename TKey, typename TValue>
struct pair_node
{
    pair_node *parent;
    TKey key;
    TValue value;
    pair_node *l_child;
    pair_node *r_child;

    inline pair_node(TKey key, TValue value, pair_node *parent=NULL, pair_node *l_child=NULL, pair_node *r_child=NULL)
        : key(key), value(value), l_child(l_child), r_child(r_child), parent(parent)
    {

    }
};

// print
template<typename TKey, typename TValue>
void print_inorder(const pair_node<TKey, TValue>* node, std::ostream& stream)
{
    if (node == NULL) return; // return if leaf node

    print_inorder(node->l_child, stream);
    stream << "\t" << node->key << " : " << node->value << ",\n"; // format: "{ key : value },\n"
    print_inorder(node->r_child, stream);
}


template<typename TKey, typename TValue>
class dict
{
private:

    typedef pair_node<TKey, TValue> pair_t;
    typedef raw_pair<TKey, TValue> raw_pair_t;

    unsigned int m_Length = 0;
    pair_t *m_Root = NULL;

    /*
     * REGION private recursive functions
     */

    // dictionary contains unique pairs
    // node should be root node when called (default value)
    inline pair_t *r_search(TKey key, pair_t *node) const
    {
        // found or NULL in which case it isn't present
        // node == NULL comparison needs to go first, because if it doesn't it will cause error 
        if (node == NULL || node->key == key)
            return node;

        if (key < node->key) // recurse left (smaller)
            return r_search(key, node->l_child);
        else // recurse right (bigger)
            return r_search(key, node->r_child);
    }

    inline pair_t *find_subtree_spot(TKey key, pair_t *node, pair_t **parent) const
    { 
        if (key < node->key) // recurse left (smaller)
        {
            if (node->l_child == NULL)
            {
                *parent = node;
                return node->l_child;
            } else
                return find_subtree_spot(key, node->l_child, parent);
        } else                 // recurse right (bigger)
        {
            if (node->r_child == NULL)
            {
                *parent = node;
                return node->r_child;
            } else
                return find_subtree_spot(key, node->r_child, parent);
        }
    }

    // recursively insert element
    // add element to binary tree, return pointer to it
    inline pair_t *r_insert(pair_t *elem, pair_t *node, pair_t *parent=NULL)
    {
        // leaf node
        if (node == NULL)
        {
            node = elem;
            elem->parent = parent;
        } else
        {
            // pick subtree to insert
            if (elem->key < node->key) // recurse left
                node->l_child = r_insert(elem, node->l_child, node);
            else // recurse right
                node->r_child = r_insert(elem, node->r_child, node);
        }

        return node;
    }

    // recursively remove element, and free memory
    inline pair_t *r_remove(pair_t *elem, pair_t *node)
    {
        if (node == NULL)
            return NULL;

        if (elem->key < node->key) // recurse left (target is smaller)
            node->l_child = r_remove(elem, node->l_child);
        else if (elem->key > node->key) // recurse right (target is bigger)
            node->r_child = r_remove(elem, node->r_child);
        else // found target node
        {
            // right subtree or leaf node
            if (node->l_child == NULL)
            {
                pair_t *r_child = node->r_child;
                delete node;

                return r_child;

            // left subtree or leaf node
            } else if (node->r_child == NULL)
            {
                pair_t *l_child = node->l_child;
                delete node;

                return l_child;
            
            // two links
            // either recurse left and get the largest node
            // or recurse right and get the smallest
            // they both fit the criteria and can replace the removed node
            // then call remove recursively on the one that the removed node was replaced for
            // I WENT FOR SMALLEST IN RIGHT SUBTREE
            } else
            {
                pair_t *temp = min(node->r_child);

                // copy data
                node->key = temp->key;
                node->value = temp->value;

                // go into the right subtree, remove the leftmost node we found and swapped data with
                // prevents us from having two nodes in our tree with the same value
                node->r_child = r_remove(temp, node->r_child);
            }
        }
        return node;
    }

    inline pair_t *min(pair_t *node) const
    {
        while (node->l_child != NULL) node = node->l_child;
        return node;
    }

    inline pair_t *max(pair_t *node) const
    {
        while (node->r_child != NULL) node = node->r_child;
        return node;
    }
    
    inline void find_median(pair_t* node, int& median, pair_t** median_node)
    {
        if (node == NULL || !median) return;

        find_median(node->l_child, median, median_node);

        if (!--median)
            *median_node = node;

        find_median(node->r_child, median, median_node);
    }
    
    inline int find_subtree_length(pair_t *node)
    {
        if (node == NULL) return 0;
        int r = 1;
        r += find_subtree_length(node->l_child);
        r += find_subtree_length(node->r_child);
        return r;
    }

    // insert subtree into other tree
    // basically r_insert but for subtrees
    // prev_node needed to set parent of subtree root
    inline void r_insert_subtree(pair_t *node, pair_t *sub_root)
    {
        // this will bring back the NULL node address
        pair_t *parent = NULL;
        pair_t *spot = find_subtree_spot(sub_root->key, node, &parent);

        // put in spot
        spot = sub_root;
        spot->parent = parent;

        // save children subtrees
        pair_t *left, *right;
        left = spot->l_child;
        right = spot->r_child;

        // link new node
        if (spot->key < parent->key) // left child
            parent->l_child = spot;
        else
            parent->r_child = spot;

        // unlink
        spot->l_child = NULL;
        spot->r_child = NULL;

        // call again on children subtrees
        if (left != NULL)
            r_insert_subtree(node, left);
        if (right != NULL)
            r_insert_subtree(node, right);
    }

    /*
    inline void print_bfs(pair_t* node, int level = 0)
    {
        if (node == NULL) return;

        for (int i = 0; i < level; i++)
            std::cout << "-";
        std::cout << node->key << std::endl;

        print_bfs(node->l_child, level + 1);
        print_bfs(node->r_child, level + 1);
    }
    */
public:
    ///*
    inline void print_bfs(pair_t* node, int level = 0)
    {
        if (node == NULL) return;

        for (int i = 0; i < level; i++)
            std::cout << "-";
        std::cout << node->key << std::endl;

        print_bfs(node->l_child, level + 1);
        print_bfs(node->r_child, level + 1);
    }
    //*/
    // find median with one inorder traversing pointer
    // unlink subtree with the root as median
    // unlink subtree with median's parent and insert to tree with median root (as they must all be bigger)
    // insert the rest of the tree (as they must all be smaller)
    // hard_optimize goes through all the subtrees and optimizes perfectly
    inline void optimize(const bool hard_optimize, pair_t* root, pair_t* parent=NULL)
    {
        //print_bfs(m_Root);
        //std::cout << std::endl;

        // return if leaf node
        if (root == NULL)
            return;

        // it must have at least 1 child or it should terminate 
        if (root->l_child == NULL && root->r_child == NULL)
            return;


        int length_count = find_subtree_length(root);

        // get median
        int steps = (length_count + 1) / 2;
        pair_t* median = NULL;
        find_median(root, steps, &median);

        //std::cout << "root: " << root->key << "\t" << "median: " << median->key << "\n\n\n";


        pair_t* old_root = root;

        if (median != old_root)
        {
            // unlink median from parent
            if (median->key < median->parent->key)
                median->parent->l_child = NULL;
            else
                median->parent->r_child = NULL;

            // insert everything else above it
            r_insert_subtree(median, old_root);

            // set parent and parents children
            // median is root
            median->parent = parent;
            if (median->parent != NULL)
            {
                if (median->key < median->parent->key)
                    median->parent->l_child = median;
                else
                    median->parent->r_child = median;
            }

            // set the parents of children
            if (median->l_child != NULL)
                median->l_child->parent = median;
            if (median->r_child != NULL)
                median->r_child->parent = median;

            // link in root
            if (root == m_Root)
            {
                root = median;
                m_Root = root; // make sure to set m_Root to root as well
            }
        }
        
        // hard optimization
        if (hard_optimize)
        {
            // hard_optimize must be true so this is fine
            if (median->l_child != NULL) // so it doesn't use m_Root
                optimize(true, median->l_child, median);
            if (median->r_child != NULL)
                optimize(true, median->r_child, median);
        }
    }

    /*
     * REGION accessibility
     */

    inline const unsigned int length() const { return m_Length; }

    inline pair_t *root() const { return m_Root; }

    /*
     * REGION setup
     */

    // default constructor
    inline dict() { }

    inline dict(pair_t *new_root)
    {
        delete_postorder(m_Root);
        m_Root = new_root;
        m_Length = find_sub_tree_length(new_root);
    }

    inline dict(const dict<TKey, TValue>& other)
    {
        delete_postorder(m_Root);
        m_Root = NULL; // reset to default
        copy_preorder(this, other.m_Root); // don't want the same memory addresses
    }

    inline dict(std::initializer_list<raw_pair_t> nodes)
    {
        delete_postorder(m_Root);
        m_Root = NULL;
        for (auto it = nodes.begin(); it != nodes.end(); it++)
            insert(it->key, it->value);
    }

    inline ~dict()
    {
        delete_postorder(m_Root);
    }

    /*
     * REGION functionality
     */

    // add element to binary tree, return bool if insert is successful
    inline const bool insert(TKey key, TValue value)
    {
        // already in tree
        if (contains(key))
            return false;     
        else
        {
            m_Root = r_insert(new pair_t(key, value), m_Root); // m_Root default param doesn't work
            m_Length++;
            return true;
        }
    }

    // remove element from binary tree, return bool if insert is successful
    inline const bool remove(TKey key)
    {
        if (!contains(key))
            return false;

        m_Root = r_remove(r_search(key, m_Root), m_Root);
        m_Length--;
        return true;
    }

    inline const bool contains(TKey key) const
    {
        return r_search(key, m_Root) == NULL ? false : true;
    }

    /*
     * REGION operators
     */

    inline void operator=(std::initializer_list<raw_pair_t> nodes)
    {
        delete_postorder(m_Root);
        m_Root = NULL;
        for (auto it = nodes.begin(); it != nodes.end(); it++)
            insert(it->key, it->value);   
    }

    inline void operator=(const dict<TKey, TValue>& other)
    {
        delete_postorder(m_Root);
        m_Root = NULL; // reset to default
        copy_preorder(this, other.m_Root);
    }
    
    inline const TValue& operator[](const TKey key) const
    {       
        return r_search(key, m_Root)->value;
    }
    
    inline TValue& operator[](const TKey key)
    {
        return r_search(key, m_Root)->value;
    }

    template<typename tkey, typename tvalue> 
    inline friend std::ostream& operator<<(std::ostream& stream, const dict<tkey, tvalue>& other)
    {
        stream << "{\n";
           
        print_inorder(other.m_Root, stream);

        stream << "}";
        return stream;
    }
};


// delete
template<typename TKey, typename TValue>
void delete_postorder(const pair_node<TKey, TValue> *node)
{
    if (node == NULL) return; // return if leaf node

    delete_postorder(node->l_child);
    delete_postorder(node->r_child);
    delete node;
}

// copy
template<typename TKey, typename TValue>
void copy_preorder(dict<TKey, TValue> *dictionary, const pair_node<TKey, TValue> *copy_node)
{
    if (copy_node == NULL) return;
    dictionary->insert(copy_node->key, copy_node->value);
    copy_preorder(dictionary, copy_node->l_child);
    copy_preorder(dictionary, copy_node->r_child);
}