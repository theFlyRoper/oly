
#ifndef SRC_PVT_LOAD_CONFIG_H
#define SRC_PVT_LOAD_CONFIG_H 1

#include "oly/common.h"
#include <stdbool.h>

BEGIN_C_DECLS
typedef struct token_mark_struct TokenMark;
/* preflight structure for allocating config trees */
struct token_mark_struct
{
    /* counts the number of OChar slots we need to allocate */
    int     count_ochars;
    /* counts the number of nodes we will have. */
    int     count_nodes;
    /* pointer to any subnodes that come from this one. */
    TokenMark   *subnode;     
    bool    is_stream_start : 1;
    bool    is_stream_end : 1;
    bool    is_version_directive : 1;
    bool    is_tag_directive : 1;
    bool    is_document_start : 1;
    bool    is_document_end : 1;
    bool    is_block_sequence_start : 1;
    bool    is_block_mapping_start : 1;
    bool    is_block_end : 1;
    bool    is_flow_sequence_start : 1;
    bool    is_flow_sequence_end : 1;
    bool    is_flow_mapping_start : 1;
    bool    is_flow_mapping_end : 1;
    bool    is_block_entry_start : 1;
    bool    is_flow_entry_start : 1;
    bool    is_key_token : 1;
    bool    is_value_token : 1;
    bool    is_alias_token : 1;
    bool    is_anchor_token : 1;
    bool    is_tag_token : 1;
    bool    is_scalar_token : 1;
};

END_C_DECLS

#endif /* SRC_PVT_LOAD_CONFIG_H */

