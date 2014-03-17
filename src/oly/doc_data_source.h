/* doc_data_source.h -- functions to interact with non-relational data sources. License GPL2+ {{{
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
   }}} */

#ifndef SRC_DOC_DATA_SOURCE_H
#define SRC_DOC_DATA_SOURCE_H 1

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



#endif /* SRC_DOC_DATA_SOURCE_H */

