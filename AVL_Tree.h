#ifndef UNTITLED_AVL_TREE_H
#define UNTITLED_AVL_TREE_H
#include <iostream>
#include <cmath>

//example for node is at the bottom of this file

typedef enum {
    SUCCESS_AVL = 0,
    FAILURE_AVL = -1,
    ALLOCATION_ERROR_AVL = -2,
    INVALID_INPUT_AVL = -3
} AVLStatusType;

template<class Node>
class AVL_Tree {
public:
    Node* head;
    Node* max;
    unsigned int size;
    bool compare_by_id;

    AVL_Tree():head(nullptr),max(nullptr), size(0), compare_by_id(true){};
    ~AVL_Tree();

    AVLStatusType insert(Node* to_add);
    Node* find(Node* to_find);
    AVLStatusType remove(Node* to_remove);
    void fixMax();
    void deleteAVLTree();
    void AVLTreeToArray(Node** array);

    friend AVLStatusType mergeTwoAVLTrees(AVL_Tree<Node>* tree_source,Node** source_array, AVL_Tree<Node>* tree_dest, Node** dest_array, Node** merged_array);
    friend void replaceNodeToRemove(AVL_Tree<Node>& avl_tree, Node*& to_remove);
    friend void performAppropriateRoll(AVL_Tree<Node>& avl_tree, Node* previous_head);

    friend void turnoverLL(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head);
    friend void turnoverRR(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head);
    friend void turnoverRL(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head, Node* moving_around_node);
    friend void turnoverLR(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head, Node* moving_around_node);

};

template<class Node>
void AVL_Tree<Node>::fixMax() {
    if(!head){
        max = nullptr;
        return;
    }
    Node* temp = head;
    while (temp->right){
        temp = temp->right;
    }
    max = temp;
};

template<class Node>
Node* AVL_Tree<Node>::find(Node* to_find) {

    int result;
    if(this->size == 0){
        return nullptr;
    }
    Node* temp = this->head;

    while (temp){

        result = temp->compare(to_find, compare_by_id);
        if (result == 0) {
            return temp;
        }
        else if(result <0) { // left is bigger, means to_find>temp
            temp = temp->right;
        }else{ // right is bigger, means to_find<temp
            temp = temp->left;
        }
    }
    return nullptr;
};

template<class Node>
void replaceNodeToRemove(AVL_Tree<Node>& avl_tree, Node*& to_remove){

    if(to_remove->right && to_remove->left){ //two sons
        Node* travel = to_remove->right;
        while (travel->left) {
            travel = travel->left;
        }
        travel->ReplaceData(to_remove);
        to_remove = travel;
    }
    if(!(to_remove->right || to_remove->left)){ //no sons
        if(to_remove == avl_tree.head){
            avl_tree.head=nullptr;
        }else if(to_remove->papa->right == to_remove){
            to_remove->papa->right = nullptr;
        }else{
            to_remove->papa->left = nullptr;
        }
    }else { //one sons
        if(to_remove == avl_tree.head){
            if(to_remove->right){
                avl_tree.head=to_remove->right;
                avl_tree.head->papa = nullptr;
            }else{
                avl_tree.head=to_remove->left;
                avl_tree.head->papa = nullptr;
            }
        }else if(to_remove->right){
            if (to_remove->papa->right == to_remove){
                to_remove->papa->right = to_remove->right;
                to_remove->right->papa = to_remove->papa;
            }else{
                to_remove->papa->left = to_remove->right;
                to_remove->right->papa = to_remove->papa;
            }
        }else{
            if (to_remove->papa->right == to_remove){
                to_remove->papa->right = to_remove->left;
                to_remove->left->papa = to_remove->papa;
            }else{
                to_remove->papa->left = to_remove->left;
                to_remove->left->papa = to_remove->papa;
            }
        }
    }
};

template<class Node>
void recursiveTreeToArray(Node* node, Node** array, int* location) {
    if (!node){
        return;
    }
    recursiveTreeToArray(node->left, array, location);
    array[*location] = node;
    *location +=1;
    recursiveTreeToArray(node->right, array, location);
};

template<class Node>
void AVL_Tree<Node>::AVLTreeToArray(Node** array){
    if(this->size ==0){
        return;
    }
    int current_location =0;
    recursiveTreeToArray(this->head, array, &current_location);
};

template<class Node>
void turnoverRR(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head){

    previous_head->right = new_head->left;
    if(new_head->left){
        new_head->left->papa = previous_head;
    }
    new_head->left = previous_head;
    new_head->papa = previous_head->papa;
    previous_head-> papa = new_head;

    previous_head->updateBySonsOnce();
    new_head->updateBySonsOnce();

    if(new_head-> papa == nullptr){
        avl_tree.head = new_head;
        return;
    }
    if(new_head->papa->right == previous_head){
        new_head->papa->right = new_head;
    }else{
        new_head->papa->left = new_head;
    }
};

template<class Node>
void turnoverLL(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head){

    previous_head->left = new_head->right;
    if(new_head->right){
        new_head->right->papa = previous_head;
    }

    new_head->right = previous_head;
    new_head->papa = previous_head->papa;
    previous_head-> papa = new_head;

    previous_head->updateBySonsOnce();
    new_head->updateBySonsOnce();

    if(new_head-> papa == nullptr){
        avl_tree.head = new_head;
        return;
    }

    if(new_head->papa->right == previous_head){
        new_head->papa->right = new_head;
    }else{
        new_head->papa->left = new_head;
    }
};

template<class Node>
void turnoverLR(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head, Node* moving_around_node){
    previous_head->left = new_head->right;

    if(new_head->right){
        new_head->right->papa = previous_head;
    }
    moving_around_node->right = new_head->left;
    if(new_head->left){
        new_head->left->papa= moving_around_node;
    }
    new_head->left=moving_around_node;
    moving_around_node->papa=new_head;
    new_head->right=previous_head;
    new_head->papa = previous_head->papa;
    previous_head-> papa = new_head;

    previous_head->updateBySonsOnce();
    moving_around_node->updateBySonsOnce();
    new_head->updateBySonsOnce();

    if(new_head-> papa == nullptr){
        avl_tree.head = new_head;
        return;
    }
    if(new_head->papa->right == previous_head){
        new_head->papa->right = new_head;
    }else{
        new_head->papa->left = new_head;
    }
};

template<class Node>
void turnoverRL(AVL_Tree<Node>& avl_tree, Node* previous_head, Node* new_head, Node* moving_around_node){
    previous_head->right = new_head->left;
    if(new_head->left){
        new_head->left->papa = previous_head;
    }
    moving_around_node->left = new_head->right;
    if(new_head->right){
        new_head->right->papa= moving_around_node;
    }
    new_head->right=moving_around_node;
    moving_around_node->papa=new_head;
    new_head->left=previous_head;
    new_head->papa = previous_head->papa;
    previous_head-> papa = new_head;

    previous_head->updateBySonsOnce();
    moving_around_node->updateBySonsOnce();
    new_head->updateBySonsOnce();

    if(new_head-> papa == nullptr){
        avl_tree.head = new_head;
        return;
    }
    if(new_head->papa->right == previous_head){
        new_head->papa->right = new_head;
    }else{
        new_head->papa->left = new_head;
    }
};

template<class Node>
void performAppropriateRoll(AVL_Tree<Node>& avl_tree, Node* previous_head){
    int previous_head_left_height = 0, previous_head_right_height = 0, inside_height_left = 0, inside_height_right = 0;
    if(previous_head->right){
        previous_head_right_height = previous_head->right->height +1;
    }
    if(previous_head->left){
        previous_head_left_height = previous_head->left->height +1;
    }
    if(previous_head_right_height - previous_head_left_height >= 2){ //R
        if(previous_head->right->right){
            inside_height_right = previous_head->right->right->height +1;
        }
        if(previous_head->right->left){
            inside_height_left = previous_head->right->left->height +1;
        }
        if(inside_height_right - inside_height_left >= 1){
            turnoverRR<Node>(avl_tree,previous_head, previous_head->right);
        }else{
            turnoverRL<Node>(avl_tree,previous_head,previous_head->right->left, previous_head->right);
        }
    }
    else { // L
        if(previous_head->left->right){
            inside_height_right = previous_head->left->right->height +1;
        }
        if(previous_head->left->left){
            inside_height_left = previous_head->left->left->height +1;
        }
        if(inside_height_right - inside_height_left <= -1){
            turnoverLL<Node>(avl_tree,previous_head, previous_head->left);
        }else{
            turnoverLR<Node>(avl_tree,previous_head,previous_head->left->right, previous_head->left);
        }
    }
};

template<class Node>
AVLStatusType AVL_Tree<Node>::insert(Node* to_add_node) {

    Node *to_add = new Node(0);
    to_add->UpdateData(to_add_node);

    if(this->size == 0){
        this->head = to_add;
        this->size = 1;
        this->max = to_add;
        return SUCCESS_AVL;
    }
    Node* temp = this->head;
    int result, insert_to_right = 0;
    while (temp){
        result = temp->compare(to_add, compare_by_id);
        if (result == 0) {
            delete to_add;
            return FAILURE_AVL;
        }
        else if(result <0) { // left is bigger, means to_find>temp
            if(!(temp->right)){
                insert_to_right = 1;
                break;
            }
            temp = temp->right;

        }else{ // right is bigger, means to_find<temp
            if(!(temp->left)){
                insert_to_right = 0;
                break;
            }
            temp = temp->left;
        }
    }
    this->size += 1;
    if (insert_to_right){
        temp->right = to_add;
        to_add->papa = temp;
    }else{
        temp->left = to_add;
        to_add->papa = temp;
    }
    temp = to_add;
    Node* res = temp->updateBySons();
    while (res != nullptr){
        performAppropriateRoll<Node>(*this, res);
        res = res->papa->updateBySons();
    }
    fixMax();
    return SUCCESS_AVL;
};

template<class Node>
AVLStatusType AVL_Tree<Node>::remove(Node* node_to_remove) {
    if (this->size == 0){
        return FAILURE_AVL;
    }
    Node* to_remove = this->find(node_to_remove);
    if (!to_remove){
        return FAILURE_AVL;
    }
    replaceNodeToRemove<Node>(*this, to_remove);
    Node* temp = to_remove->papa;

    delete to_remove;
    fixMax();

    this->size -=1;
    if(!temp){
        return SUCCESS_AVL;
    }
    Node* result = temp->updateBySons();
    while (result != nullptr){
        performAppropriateRoll<Node>(*this, result);
        result = result->papa->updateBySons();
    }
    return SUCCESS_AVL;
};

template<class Node>
void mergeArrays(Node** array1, int size1, Node** array2, int size2 ,Node** array_for_merge){

    int a1,a2,location;
    for(a1 = a2 = location= 0; a1<size1 && a2<size2 ;location++){
        if (array1[a1]->compare(array2[a2], false)>0){ // a1 is bigger
            array_for_merge[location]->UpdateData(array2[a2]);
            //array_for_merge[location]->up array2[a2];
            a2++;
        }else{
            array_for_merge[location]->UpdateData(array1[a1]);
            //array_for_merge[location] = array1[a1];
            a1++;
        }
    }
    for(; a1<size1 ;a1++, location++){array_for_merge[location]->UpdateData(array1[a1]);}
    for(; a2<size2 ;a2++,location++){array_for_merge[location]->UpdateData(array2[a2]);}

};

template<class Node>
void createTree(Node* head, int current_depth, int overall_depth ,int do_not_create) {
    int depth_difference = overall_depth-current_depth, right_height =0, left_height = 0, min2, non_negative_difference;
    int sub_tree_sons_amount = pow(2, depth_difference-1);
    if(!head){
        return;
    }
    Node* right = nullptr;
    Node* left= nullptr;
    if(depth_difference>1){
        right = new Node(0);
        left = new Node(0);
    }else if(depth_difference==1){
        if(do_not_create<2){
            left = new Node(0);
            head->left = left;
            left->papa = head;
            head->height=1;
            if (do_not_create!=1) {
                right = new Node(0);
                head->right = right;
                right->papa = head;
            }
        }
        return;
    }
    head->right = right;
    head->left = left;
    min2 = sub_tree_sons_amount - do_not_create > 0 ? do_not_create : sub_tree_sons_amount;
    non_negative_difference =  do_not_create-sub_tree_sons_amount > 0 ? do_not_create-sub_tree_sons_amount: 0;
    createTree<Node>(head->right, current_depth+1, overall_depth, min2);
    createTree<Node>(head->left, current_depth+1, overall_depth, non_negative_difference);
    if(right != nullptr){
        right->papa = head;
        right_height=right->height;
    }
    if(left != nullptr){
        left->papa = head;
        left_height=left->height;
    }
    head->height = fmax(right_height, left_height)+1;
};

template<class Node>
Node* createEmptyTree(int size){
    Node* head = new Node(0);
    int depth = (int)log2(size);
    int do_not_create;
    do_not_create = (int)pow(2, depth+1)-1- size;
    createTree<Node>(head, 0,depth, do_not_create);
    return head;
};

template<class Node>
void recursiveInOrderFillAVLTree(Node* node, Node ** merged_arr, int *counter) {
    if (!node){
        return;
    }
    recursiveInOrderFillAVLTree<Node>(node->left, merged_arr, counter);
    //todo check this
    node->UpdateData(merged_arr[*counter]);
    *counter+=1;
    recursiveInOrderFillAVLTree<Node>(node->right, merged_arr, counter);
};

template<class Node>
AVLStatusType mergeTwoAVLTrees(AVL_Tree<Node>* tree_source,Node** source_array, AVL_Tree<Node>* tree_dest, Node** dest_array, Node** merged_array ){
    int all_nodes = tree_source->size+tree_dest->size;
    if(all_nodes == 0){
        return SUCCESS_AVL;
    }
    tree_dest->AVLTreeToArray(dest_array);
    tree_source->AVLTreeToArray(source_array);

    mergeArrays<Node>(dest_array,tree_dest->size,source_array, tree_source->size, merged_array);
    recursiveDeleteAVLTree(tree_dest->head);
    tree_dest->max = nullptr;
    tree_dest->head = nullptr;

    tree_dest->size = all_nodes;
    tree_dest->head = createEmptyTree<Node>(all_nodes);
    int counter = 0;
    recursiveInOrderFillAVLTree<Node>(tree_dest->head,merged_array, &counter);
    tree_dest->fixMax();
    return SUCCESS_AVL;
};

template<class Node>
void recursiveDeleteAVLTree(Node* node ) {
    if (!node){
        return;
    }
    recursiveDeleteAVLTree(node->left);
    recursiveDeleteAVLTree(node->right);
    delete node;
};

template<class Node>
void AVL_Tree<Node>::deleteAVLTree() {
    if (this->head){
        recursiveDeleteAVLTree<Node>(this->head);
    }
    this->max = nullptr;
};

template<class Node>
AVL_Tree<Node>::~AVL_Tree() {
    this->deleteAVLTree();
};


//nodes

class GroupIdNode {
public:
    int group_id;
    int height;
    GroupIdNode* papa;
    GroupIdNode* left;
    GroupIdNode* right;

    explicit GroupIdNode(int new_group_id): group_id(new_group_id), height(0), papa(nullptr), left(nullptr), right(nullptr){};
    ~GroupIdNode() = default;

    GroupIdNode(const GroupIdNode &node) {
        group_id = node.group_id;
        height = node.height;
        papa = node.papa;
        left = node.left;
        right = node.right;
    }

    void ReplaceData(GroupIdNode* to_replace){
        int old_id = group_id;
        group_id = to_replace->group_id;
        to_replace->group_id = old_id;
    }

    void UpdateData(GroupIdNode*& to_update){
        group_id = to_update->group_id;
    }

    GroupIdNode* updateBySons(){
        GroupIdNode* temp = this;
        bool first = true;
        int old_temp_height, temp_right_height, temp_left_height;

        while(temp){
            temp_right_height = -1;
            temp_left_height = -1;

            if(temp->right){
                temp_right_height = temp->right->height ;
            }
            if(temp->left){
                temp_left_height = temp->left->height ;
            }
            old_temp_height = temp->height;
            temp->height = int(fmax(temp_right_height,temp_left_height))+1;

            if(abs(temp_right_height - temp_left_height) >= 2){
                return temp;
            }
            if(old_temp_height == temp->height && !first){
                return nullptr;
            }
            temp = temp->papa;
            first = false;
        }
        return nullptr;
    }

    void updateBySonsOnce(){
        int temp_right_height = -1, temp_left_height = -1;

        if(this->right){
            temp_right_height = this->right->height ;
        }
        if(this->left){
            temp_left_height = this->left->height ;
        }
        if(temp_right_height == temp_left_height){
            this->height = temp_left_height+1;
        }else{
            this->height = int(fmax(temp_right_height,temp_left_height))+1;
        }

    }

    int compare(GroupIdNode* b, bool){
        return this->group_id - b->group_id;
    }
};


#endif //UNTITLED_AVL_TREE_H
