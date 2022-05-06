#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <arpa/inet.h> /*for inet_ntop & inet_pton*/
#include "mtrie.h"
#include "../../stack/stack.h"

#define LSHIFT(N, n) \
    if (n != 32) N << n;  \
    else                          \
    N = 0;

#define RSHIFT(N, n) \
    if (n != 32) N >> n;  \
    else                          \
    N = 0;
	
static uint16_t node_id = 1;

static inline uint16_t 
mtrie_get_new_node_id() {
    return (node_id++);
}

static void
mtrie_move_children(mtrie_node_t *src_node, mtrie_node_t *dst_node) {

    assert(!dst_node->child[ZERO]);
    assert(!dst_node->child[ONE]);
    assert(!dst_node->child[DONT_CARE]);
    dst_node->child[ZERO]  = src_node->child[ZERO] ;
    dst_node->child[ONE]  = src_node->child[ONE] ;
    dst_node->child[DONT_CARE]  = src_node->child[DONT_CARE] ;
     src_node->child[ZERO]  = NULL;
     src_node->child[ONE]  = NULL;
     src_node->child[DONT_CARE] = NULL;
     if (dst_node->child[ZERO])
         dst_node->child[ZERO]->parent = dst_node;
     if (dst_node->child[ONE])
         dst_node->child[ONE]->parent = dst_node;
     if (dst_node->child[DONT_CARE])
         dst_node->child[DONT_CARE]->parent = dst_node;
}

void
mtrie_print_node(mtrie_node_t *node) {

    int i;
    bit_type_t bit;

    printf ("ID : %d\n", node->node_id);
    printf ("Prefix/Len : ");

    for (i = 0; i < node->prefix_len; i++) {

        bit = EFFECTIVE_BIT_AT(node->prefix, node->mask, i);
        switch (bit)
        {
        case DONT_CARE:
            printf("X");
            break;
        case ONE:
            printf("1");
            break;
        case ZERO:
            printf("0");
            break;
        default:;
        }
    }

    printf ("/%d\n", node->prefix_len);
    printf ("Parent Node = %d\n", node->parent ? node->parent->node_id : 0);
    printf ("children = %d %d %d\n", 
        node->child[ZERO] ? node->child[ZERO]->node_id : 0,
        node->child[ONE] ? node->child[ONE]->node_id : 0,
        node->child[DONT_CARE] ? node->child[DONT_CARE]->node_id : 0);
    printf ("data = %p\n", node->data);
}

static void 
mtrie_node_split (mtrie_node_t *node, uint8_t split_offset) {

    mtrie_node_t *new_node;
    bit_type_t new_child_pos;

    /* Split offset cannot be zero, if it is zero, we shall grow
    the mtrie on root side */
    assert(split_offset);

    /* Create a new mtrie node */
    new_node = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
    new_node->node_id = mtrie_get_new_node_id();

    /* COPY Prefix : copy node->prefix_len - split_offset + 1 bits 
        from parent node starting from split_offset to end of the prefix */
    bit_copy(&node->prefix, &new_node->prefix, split_offset, 0, node->prefix_len - split_offset );
    /* COPY mask in the same way as above*/
    bit_copy(&node->mask, &new_node->mask, split_offset, 0, node->prefix_len - split_offset );
    /* Set prefix len in new Node */
    new_node->prefix_len = node->prefix_len - split_offset;

    /* Now move all the Children from parent node to new node */
    mtrie_move_children(node, new_node);
    new_node->data = node->data;
    node->data = NULL;

    /* Establish parent Child Relationship */
    new_node->parent = node;

    if (BIT_AT(node->mask, split_offset)) {
        new_child_pos = DONT_CARE;
    }
    else if (BIT_AT(node->prefix, split_offset)) {
        new_child_pos = ONE;
    }
    else {
        new_child_pos = ZERO;
    }
    node->child[new_child_pos] = new_node;

    /* Update the parent node Prefix len/Prefix/mask*/

    /* Update prefix and mask first. 
        Though our mtrie will be constructed accurately, not getting rid
        of out of scope bits in prefix and mask would create issue in display of 
        data in show or in gdb. It may not have functional impact though because updating prefix len is enough*/
    uint32_t temp = bit_generate_ones(split_offset, node->prefix_len - 1);
    temp = ~temp;
    node->prefix &= temp;
    node->mask &= temp;

    /* now update prefix len */
    node->prefix_len = split_offset;
}

void
mtrie_insert_prefix (mtrie_t *mtrie, 
    							  uint32_t prefix,
								  uint32_t mask,
								  uint8_t prefix_len,
                                  void *data) {

    int i = 0, j = 0;
    bit_type_t bit1, bit2;
    mtrie_node_t *node;

    assert(mtrie->root && prefix_len);
    
    bit1 = EFFECTIVE_BIT_AT(prefix, mask, 0);

    if (!mtrie->root->child[bit1]) {

        mtrie->root->child[bit1] = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t ));
        mtrie->root->child[bit1]->node_id = mtrie_get_new_node_id();
        mtrie->root->child[bit1]->prefix = prefix;
        mtrie->root->child[bit1]->mask = mask;
        mtrie->root->child[bit1]->prefix_len = prefix_len;
        mtrie->root->child[bit1]->data = data;
        mtrie->root->child[bit1]->parent = mtrie->root;
        return;
    }

    node = mtrie->root->child[bit1];

    uint16_t node_prefix_len = node->prefix_len;

    BIT_MASK_ITERATE_BEGIN(prefix, mask, prefix_len, i, bit1) {

        if (j == node_prefix_len ) {
            if (node->child[bit1]) {
                node = node->child[bit1];
                node_prefix_len = node->prefix_len;
                j = 1;
                continue;
            }
            break;
        }

        bit2 = EFFECTIVE_BIT_AT(node->prefix, node->mask, j);
        if (bit1 == bit2) {
            j++;
            continue;
        }
        mtrie_node_split(node, j);
        assert(node->child[bit1] == NULL);
        break;
    }
    BIT_MASK_ITERATE_END;

    if (i == prefix_len) {
        if (j == node_prefix_len) {
            printf("Duplicate TCAM entry\n");
        }
        else {
             printf("Input TCAM entry exhausted\n");
             /* All entries are of same size. Input entry cannot be of 
             any arbitrary size  */
             assert(0);
            //mtrie_node_split(node, j);
        }
        return;
    }

    node->child[bit1] = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
    node->child[bit1]->node_id = mtrie_get_new_node_id();
    node->child[bit1]->parent = node;
    node = node->child[bit1];
    bit_copy(&prefix, &node->prefix, i, 0, prefix_len - i);
    bit_copy(&mask, &node->mask, i, 0, prefix_len - i );
    node->prefix_len = prefix_len - i;
    node->data = data;
}

void
init_mtrie(mtrie_t *mtrie) {

    assert(!mtrie->root);
    mtrie->root = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
    mtrie->root->node_id = mtrie_get_new_node_id();
    mtrie->N = 1;
    mtrie->stack = get_new_stack();
}

static inline void 
stack_push_node (stack_t *stack, mtrie_node_t *node, uint32_t prefix, 
                                    uint8_t stacked_prefix_matched_bits_count) {

    if (!node) return;
    node->stacked_prefix = prefix;
    node->stacked_prefix_matched_bits_count = stacked_prefix_matched_bits_count;
    push(stack , (void *)node);
}

mtrie_node_t *
mtrie_longest_prefix_match_search(mtrie_t *mtrie, uint32_t prefix) {

    uint8_t cum_n_bits_matched = 0;
    uint32_t n_back_tracks = 0, 
                  n_comparisons = 0;
    mtrie_node_t *node, *next_node;
    
    reset_stack(mtrie->stack);

    node = mtrie->root->child[BIT_AT(prefix, 0) ? ONE : ZERO];

    if (node) {
        stack_push_node(mtrie->stack, mtrie->root->child[DONT_CARE], prefix, cum_n_bits_matched);
    }
    else {
        node = mtrie->root->child[DONT_CARE];
    }

    if (!node) return NULL;

    while(true) {

        n_comparisons++;
        if (!prefix_match(prefix, node->prefix, node->mask, node->prefix_len)) {

            node = (mtrie_node_t *)pop(mtrie->stack);

            if (node) {

                n_back_tracks++;
                prefix = node->stacked_prefix;
                cum_n_bits_matched = node->stacked_prefix_matched_bits_count;
                node->stacked_prefix = 0;
                node->stacked_prefix_matched_bits_count = 0;
                stack_push_node(mtrie->stack, mtrie->root->child[DONT_CARE], prefix, cum_n_bits_matched);
                continue;
            }
            return NULL;
        }

        cum_n_bits_matched += node->prefix_len;

        if (cum_n_bits_matched == 32) {
                assert(node->data);
                node->n_comparisons = n_comparisons;
                node->n_backtracks = n_back_tracks;
                assert(!node->child[ZERO] && 
                          !node->child[ONE] && 
                          !node->child[DONT_CARE]);
                return node;
        }
        
        /* Shifts with data type width is not defined */
        if (node->prefix_len != 32) {
            prefix = (prefix << node->prefix_len);
        }
        else {
            prefix = 0;
        }

        next_node = node->child[BIT_AT(prefix, 0) ? ONE : ZERO];

        if (next_node) {
            stack_push_node(mtrie->stack, next_node->child[DONT_CARE], prefix, cum_n_bits_matched);
        }
        else {
            next_node = node->child[DONT_CARE];
        }

        if (!next_node) return NULL;
        node = next_node;
    }
}

/* To be called on node which has exactly one child*/
static bool
mtrie_merge_child_node (mtrie_t *mtrie, mtrie_node_t *node) {

    uint8_t child_count = 0;
    bit_type_t bit;

    if (node->child[ZERO]) {
        child_count++;
        bit = ZERO;
    }

    if (node->child[ONE]) {
        child_count++;
        bit = ONE;
    }

    if (node->child[DONT_CARE]) {
        child_count++;
        bit = DONT_CARE;
    }

    /* A node is eligibe to merge its own child node only when
    it has exactly one child branch*/
    if (child_count != 1) return false;

    mtrie_node_t *child_node = node->child[bit];

    bit_copy_preserve(&child_node->prefix, &node->prefix, 0, node->prefix_len, child_node->prefix_len);
    bit_copy_preserve(&child_node->mask, &node->mask, 0, node->prefix_len, child_node->prefix_len);
    node->prefix_len += child_node->prefix_len;
    
    mtrie_move_children(child_node, node);
    
    if (child_node->data) free(child_node->data);
    free(child_node);
    mtrie->N--;
    return true;
}


static mtrie_node_t *
mtrie_exact_prefix_match_search(mtrie_t *mtrie, uint32_t prefix, uint32_t mask) {

    return NULL;
}

bool
mtrie_delete_prefix (mtrie_t *mtrie, uint32_t prefix, uint32_t mask) {

    mtrie_node_t *parent_node;
    mtrie_node_t *node = mtrie_exact_prefix_match_search(mtrie, prefix, mask);

    if (!node) {
        return false;
    }

    /* Must be leaf node */
    assert(!node->child[ZERO] &&
           !node->child[ONE] &&
           !node->child[DONT_CARE]);

    parent_node = node->parent;
    
    /* Break the association between parent node and child node */
    if (parent_node->child[ZERO] == node) {
        parent_node->child[ZERO] = NULL;
    }
    else if (parent_node->child[ONE] == node) {
        parent_node->child[ONE] = NULL;
    }
    else {
        parent_node->child[DONT_CARE] = NULL;
    }

    if (!mtrie_merge_child_node(mtrie, parent_node)) {
        if (node->data) free(node->data);
        free(node);
        mtrie->N--;
    }
    
    return true;
}

#if 0
int 
main(int argc, char **argv) {

    mtrie_t mtrie;
    
    init_mtrie(&mtrie);

    uint32_t prefix = 0xAA000000; // Prefix : 10101010
    uint32_t mask =  0;
    uint8_t prefix_len = 8;

    mtrie_insert_prefix(&mtrie, prefix, mask, prefix_len, NULL);

    mtrie_print_node(mtrie.root);

    prefix = 0xA8000000; // prefix : 101010*
    mask = 0x3000000; // mask :      00000011

    mtrie_insert_prefix(&mtrie, prefix, mask, prefix_len, NULL);


    prefix = 0xC8000000; // prefix :    11001*
    mask = 0x7000000;  // mask =       00000111

    mtrie_insert_prefix(&mtrie, prefix, mask, prefix_len, NULL);

    prefix = 0xC8000000;  // prefix = 11001*
    mask = 0x4000000;      // mask :     000001
    prefix_len = 6;

    mtrie_insert_prefix(&mtrie, prefix, mask, prefix_len, NULL);

    prefix = 0b00001000100000000000000000000000; // 0000 10xx 10xx xxx0
    mask = 0b11111100110000010000000000000000;   // 1111 1100 1100 0001
    mask = ~mask;
    prefix_len = 16;

    uint32_t value = 0b00001011101011000000000000000000; // match
    value = 0b00101011110111000000000000000000;

    if (prefix_match(value, prefix, mask, prefix_len)) {
        printf ("match\n");
    }
    else {
        printf ("no match\n");
    }

    return 0;
}
#endif

static void
 _mtrie_traverse(mtrie_node_t *node, uint8_t pos, uint32_t *prefix, uint32_t *mask,
                                void (*process_fn_ptr)(uint32_t , uint32_t)) {

    uint32_t prefix_temp, mask_temp;

    if (!node)
        return;

    assert(pos <= 32);

    if (node->prefix_len) {
        bit_copy_preserve(&node->prefix, prefix, 0, pos, node->prefix_len);
        bit_copy_preserve(&node->mask, mask, 0, pos, node->prefix_len);
    }

    if (node->data) {
        prefix_temp = *prefix;
        mask_temp = *mask;
        mask_temp = ~mask_temp;
        prefix_temp &= mask_temp;
        process_fn_ptr(prefix_temp, mask_temp);
    }

    _mtrie_traverse(node->child[ZERO], pos + node->prefix_len, prefix, mask, process_fn_ptr);
    _mtrie_traverse(node->child[ONE], pos + node->prefix_len, prefix, mask, process_fn_ptr);
    _mtrie_traverse(node->child[DONT_CARE], pos + node->prefix_len, prefix, mask, process_fn_ptr);
 }

void
mtrie_traverse(mtrie_t *mtrie, void (*process_fn_ptr)(uint32_t , uint32_t)) {

    uint32_t prefix = 0;
    uint32_t mask = 0;
    _mtrie_traverse(mtrie->root, 0, &prefix, &mask, process_fn_ptr);
}