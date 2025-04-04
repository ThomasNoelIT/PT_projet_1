#include "magic.h"
#include "stdbool.h"
#include "stdlib.h"
#include <sys/stat.h>
#include <sys/types.h>



// Enumeration representing the color of a Red-Black Tree node
typedef enum {
    RED,   // Red color (used for balancing the tree)
    BLACK  // Black color
} Color;

// Structure representing a node in the Red-Black Tree
typedef struct RedBlackTreeNode {
    int pos;        // Position or key of the node
    int delta;      // Stores the range or length of an operation (e.g., deletions)
    int lazyShift;  // Accumulated shift applied to this node (used for range updates)
    int timestamp;  // Timestamp indicating when this node was modified
    struct RedBlackTreeNode *left, *right, *parent; // Pointers to left, right children and parent node
    Color color;    // Node color (RED or BLACK) for balancing the tree
} RBNode;

// Structure representing a Red-Black Tree
typedef struct RedBlackTree {
    RBNode *NIL;   // Sentinel NIL node (used to represent null leaves)
    RBNode *root;  // Root node of the tree
} RBTree;



/*
    Initializes an empty Red-Black Tree.
    
    Returns:
    --------
    - A pointer to the newly created RBTree.
    - NULL if memory allocation fails.
*/
RBTree *RBTreeInit(void) {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree) return NULL;

    // Create and initialize the NIL sentinel node.
    tree->NIL = (RBNode*)malloc(sizeof(RBNode));
    if (!tree->NIL) {
        free(tree);
        return NULL;
    }

    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->NIL->pos = tree->NIL->delta = tree->NIL->lazyShift = tree->NIL->timestamp = 0;
    tree->root = tree->NIL; // Tree starts empty.

    return tree;
}

/*
    Creates a new node for the Red-Black Tree.
    
    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - pos : Position value for the node.
    - delta : The shift value associated with the node.
    - timestamp : Timestamp for tracking the node's operation order.

    Returns:
    --------
    - A pointer to the newly created RBNode.
    - NULL if memory allocation fails.
*/
RBNode *createNode(RBTree *tree, int pos, int delta, int timestamp) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return NULL;

    node->pos = pos;
    node->delta = delta;
    node->lazyShift = delta; // Initially, lazyShift equals delta.
    node->timestamp = timestamp;
    node->left = node->right = node->parent = tree->NIL;
    node->color = RED; // Nodes are inserted as red.

    return node;
}

/*
    Performs a left rotation on the given node in the Red-Black Tree.
    
    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - x : Pointer to the node to be rotated.

    Effects:
    --------
    - Moves the right child of x up to replace x.
    - x becomes the left child of its previous right child.
    - Updates parent-child relationships accordingly.
    - Propagates lazyShift values up the tree.
*/
void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right; // y is the right child of x
    x->right = y->left;
    if (y->left != tree->NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->NIL) {
        tree->root = y; // y becomes the new root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;

    // Update lazyShift propagation
    y->lazyShift += x->lazyShift; //parent
}

/*
    Performs a right rotation on the given node in the Red-Black Tree.
    
    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - y : Pointer to the node to be rotated.

    Effects:
    --------
    - Moves the left child of y up to replace y.
    - y becomes the right child of its previous left child.
    - Updates parent-child relationships accordingly.
    - Ensures lazyShift values are correctly adjusted.
*/
void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left; // x is the left child of y
    y->left = x->right;
    if (x->right != tree->NIL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->NIL) {
        tree->root = x; // x becomes the new root
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;

    // Update lazyShift based on subtree values
    y->lazyShift = y->delta + (y->left != tree->NIL ? y->left->lazyShift : 0); //parent
}


/*
    Fixes the Red-Black Tree properties after an insertion.

    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - z : Pointer to the newly inserted node.

    Effects:
    --------
    - Ensures that the Red-Black Tree maintains its balance and properties.
    - Resolves violations caused by inserting a red node into a tree.
    - Uses rotations and recoloring to restore the tree's structure.

    Cases Handled:
    -------------
    1. If the uncle node (y) is red:
       - Recolor parent, uncle, and grandparent.
       - Move up to the grandparent.
    2. If the uncle node is black and z is a right child:
       - Perform a left rotation on the parent.
    3. If the uncle node is black and z is a left child:
       - Recolor parent and grandparent.
       - Perform a right rotation on the grandparent.
    
    The same logic applies symmetrically when z’s parent is a right child.
*/
void fixInsert(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right; // Uncle node
            if (y->color == RED) {
                // Case 1: Uncle is red -> Recolor
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move up the tree
            } else {
                if (z == z->parent->right) {
                    // Case 2: z is a right child -> Rotate left
                    z = z->parent;
                    leftRotate(tree, z);
                }
                // Case 3: z is a left child -> Recolor and rotate right
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            // Mirror case: Parent is a right child
            RBNode *y = z->parent->parent->left; // Uncle node
            if (y->color == RED) {
                // Case 1: Uncle is red -> Recolor
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move up the tree
            } else {
                if (z == z->parent->left) {
                    // Case 2: z is a left child -> Rotate right
                    z = z->parent;
                    rightRotate(tree, z);
                }
                // Case 3: z is a right child -> Recolor and rotate left
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK; // Ensure the root remains black
}

/*
    Inserts a new node into the Red-Black Tree while maintaining balance.

    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - pos : Position value for the new node.
    - delta : Shift value associated with this node.
    - timestamp : Timestamp to track insertion order.

    Effects:
    --------
    - Creates a new node with the given values.
    - Finds the correct position in the tree while updating lazyShift values.
    - Inserts the node and adjusts tree structure using `fixInsert()`.

    Behavior:
    ---------
    - The function searches for the correct position for the new node.
    - Updates lazyShift values of ancestor nodes when traversing left.
    - Inserts the node as a red node, then calls `fixInsert()` to maintain
      the Red-Black Tree properties.
*/
void RBTreeInsert(RBTree *tree, int pos, int delta, int timestamp) {
    RBNode *z = createNode(tree, pos, delta, timestamp);
    if (!z) return;

    RBNode *y = tree->NIL;
    RBNode *x = tree->root;

    // Find the correct insertion point
    while (x != tree->NIL) {
        y = x;
        if (z->pos < x->pos) {
            x->lazyShift += z->delta; // Propagate shift adjustments
            x = x->left;
        } else {
            x = x->right;
        }
    }

    // Insert node in the tree
    z->parent = y;
    if (y == tree->NIL) {
        tree->root = z; // Tree was empty
    } else if (z->pos < y->pos) {
        y->left = z;
    } else {
        y->right = z;
    }

    // Fix Red-Black Tree properties
    fixInsert(tree, z);
}


/*
    Finds a node in the Red-Black Tree that represents a deletion range 
    containing the given position.

    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - pos : The position to check for deletion.

    Return:
    -------
    - A pointer to the node if the position falls within a deleted range.
    - NULL if no such node exists.

    Behavior:
    ---------
    - Traverses the tree to find a node where `pos` falls within the range
      `[current->pos, current->pos - current->delta)`, indicating deletion.
    - If found, returns the node representing the deleted range.
    - Otherwise, continues searching the left or right subtree based on `pos`.
*/
RBNode* findDeleteNode(RBTree *tree, int pos) {
    RBNode *current = tree->root;

    while (current != tree->NIL) {
        // Check if the position falls within the deleted range
        if (pos >= current->pos && pos < current->pos - current->delta) {
            return current;
        }

        // Traverse the tree based on position
        if (pos < current->pos) {
            current = current->left;
        } else if (pos > current->pos) {
            current = current->right;
        } else {
            return current; // Exact match found
        }
    }
    return NULL; // Position is not deleted
}


/*
    Finds the position of an element in a Red-Black Tree after applying
    the necessary transformations and deletions based on the given direction.

    Arguments:
    ----------
    - sTree : Pointer to the source Red-Black Tree that holds the shifts.
    - dTree : Pointer to the destination Red-Black Tree that holds the deletions.
    - pos : The position to find the new mapped position.
    - direction : Direction of the mapping, determines whether we are looking
                  for the current position (STREAM_IN_OUT) or the original position 
                  before the transformation (STREAM_OUT_IN).

    Return:
    -------
    - The mapped position after applying the transformations and deletions, 
      or -1 if the position is invalid (e.g., deleted).

    Behavior:
    ---------
    - STREAM_IN_OUT (direction == 0): Finds the current position of the element
      given the original position. It accumulates the shifts in the tree and 
      checks if the position has been deleted.
    - STREAM_OUT_IN (direction == 1): Finds the original position of the element 
      given its current position by reversing the shifts. It also checks if 
      the original position has been deleted.
    - If the position is found in the deletion tree and is marked as deleted 
      (with a timestamp greater than the transformation), the function returns -1.
    - The function uses `findDeleteNode` to verify if the position has been deleted.
*/
int RBTreeFindMapping(RBTree *sTree,RBTree *dTree,  int pos, MAGICDirection direction) {
    int shift = 0;
    RBNode *current = sTree->root;
    RBNode *candidate = NULL;

    if (!direction) { // STREAM_IN_OUT: Finding the current position
        while (current != sTree->NIL) {
            int adjustedPos = pos + shift;
            if (adjustedPos < current->pos) {
                current = current->left;
            } else {
                candidate = current;
                shift += current->lazyShift;
                current = current->right;
            }
        }
        if (candidate) {
            int newPos = pos + shift;
            
            // Check if the position has been deleted
            if(shift > 0){
                RBNode *deleteNode = findDeleteNode(dTree, newPos);
                if (deleteNode && deleteNode->timestamp >= candidate->timestamp) {
                    return -1; // Position is deleted
                }
            }
            return (newPos >= candidate->pos) ? newPos : -1;
        } else {
            return pos; // Position is not found
        }

    } else { // STREAM_OUT_IN: Finding the original position
        current = sTree->root;
        shift = 0;
        candidate = NULL;

        while (current != sTree->NIL) {
            int adjustedPos = current->pos + shift;

            if (pos < adjustedPos) {
                if (current->pos <= pos) {
                    candidate = current;
                    shift += current->lazyShift;
                }
                current = current->left;
            } else {
                candidate = current;
                shift += current->lazyShift;
                current = current->right;
            }
        }

        if (candidate && pos >= candidate->pos && pos < candidate->pos + shift) {
            return -1; // Position was added and doesn't have an original position
        }

        if (candidate) {
            int originalPos = pos - shift;

             // Check if the original position has been deleted
            RBNode *deleteNode = findDeleteNode(dTree, originalPos);
            if (deleteNode && deleteNode->timestamp > candidate->timestamp) {
                return -1; // Original position is deleted
            }
            return (originalPos >= 0) ? originalPos : -1;
        } else {
            return pos; // Position is not found
        }
    }
}

/*
    Recursively frees all nodes in the Red-Black Tree.

    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree.
    - node : Current node being processed for freeing. This is typically
             the root node initially passed to start the recursion.

    Behavior:
    ---------
    - The function performs a post-order traversal (left, right, node) and
      frees each node in the tree.
    - It ensures that no node, including the NIL sentinel node, is left allocated.
*/
void RBTreeFreeNodes(RBTree *tree, RBNode *node) {
    if (node != tree->NIL) {
        RBTreeFreeNodes(tree, node->left); // Recursively free left subtree
        RBTreeFreeNodes(tree, node->right); // Recursively free right subtree
        free(node); free(node);  // Free the current node
    }
}

/*
    Destroys the entire Red-Black Tree, freeing all allocated memory.

    Arguments:
    ----------
    - tree : Pointer to the Red-Black Tree to be destroyed.

    Behavior:
    ---------
    - The function first frees all the nodes in the tree using the 
      `RBTreeFreeNodes` function, then it frees the NIL sentinel node
      and finally the tree structure itself.
    - If the tree pointer is NULL, it does nothing.
*/
void RBTreeDestroy(RBTree *tree) {
    if (!tree) return; // If the tree is NULL, do nothing

    RBTreeFreeNodes(tree, tree->root); // Free all nodes in the tree
    free(tree->NIL); // Free the sentinel NIL node
    free(tree); // Free the tree structure itself
}

/*
    Represents a MAGIC structure that maintains two Red-Black Trees and a timestamp.

    Members:
    --------
    - shiftTree : Pointer to the Red-Black Tree that stores the shift operations.
    - deleteTree : Pointer to the Red-Black Tree that stores deleted nodes.
    - timestamp : The current timestamp associated with the MAGIC instance.

    Description:
    ------------
    The MAGIC structure is used to manage shift and delete operations within
    a sequence of data transformations.
*/
struct magic{
    RBTree *shiftTree; // Tree to track position shifts (used for mapping).
    RBTree *deleteTree; // Tree to track deleted positions.
    int timestamp; // Current timestamp of the MAGIC instance.
};


/*
    A global variable used to track the system-wide timestamp across MAGIC instances.

    Description:
    ------------
    This global timestamp is used to provide a versioning mechanism for tracking
    changes in the MAGIC system.
*/
static long global_timestamp = 0;

/*
    Increments the global timestamp.

    Arguments:
    ----------
    None.

    Behavior:
    ---------
    - Increases the global timestamp by 1.
    - This function ensures that each transformation operation is associated with
      a unique timestamp, supporting versioning and mapping in the MAGIC system.
*/
void incrementTimestamp(void) {
    global_timestamp++;
}

/*
    Initializes a MAGIC structure with two Red-Black Trees (shift and delete trees) and an initial timestamp.

    Arguments:
    ---------
    None.

    Return:
    -------
    - A pointer to the initialized MAGIC structure or NULL on failure.
*/
MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    m->shiftTree = RBTreeInit();
    m->deleteTree = RBTreeInit();
    m->timestamp = 0;
    if (!m->shiftTree || !m->deleteTree){
        free(m);
        return NULL;
    }
    return m;
}

/*
    Removes a sequence from the MAGIC structure by updating both shift and delete trees.

    Arguments:
    ----------
    - m : The MAGIC structure.
    - pos : The starting position of the sequence to remove.
    - length : The length of the sequence to remove.
*/
void MAGICremove(MAGIC m, int pos, int length) {
    if (!m || length <= 0) return;

    incrementTimestamp();

    RBTreeInsert(m->deleteTree, pos, -length, global_timestamp);

    RBTreeInsert(m->shiftTree, pos, -length, global_timestamp);

}

/*
    Adds a sequence to the MAGIC structure by updating the shift tree.

    Arguments:
    ----------
    - m : The MAGIC structure.
    - pos : The starting position of the sequence to add.
    - length : The length of the sequence to add.
*/
void MAGICadd(MAGIC m, int pos, int length) {
    if (!m || length <= 0) return;
    if(pos < 0) return;

    incrementTimestamp();

    RBTreeInsert(m->shiftTree, pos, length, global_timestamp);

}


/*
    Maps a position in the source stream to the destination stream, considering shifts and deletions.

    Arguments:
    ----------
    - m : The MAGIC structure.
    - direction : The mapping direction.
    - pos : The position to map.

    Return:
    -------
    - Mapped position or -1 if no mapping is found.
*/
int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;

    int shiftedPos = RBTreeFindMapping(m->shiftTree, m->deleteTree, pos,direction);
    
    return shiftedPos;
}

/*
    Destroys the MAGIC structure and frees all associated resources.

    Arguments:
    ----------
    - m : The MAGIC structure to destroy.
*/
void MAGICdestroy(MAGIC m){
    if (!m)
        return;

    RBTreeDestroy(m->shiftTree);
    RBTreeDestroy(m->deleteTree);
    free(m);
}
