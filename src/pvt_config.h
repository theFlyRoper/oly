
#ifndef SRC_PVT_LOAD_CONFIG_H
#define SRC_PVT_LOAD_CONFIG_H 1

#include "oly/common.h"

BEGIN_C_DECLS


"null | Null | NULL | ~ "
 /* Empty */ 	 tag:yaml.org,2002:null
 true | True | TRUE | false | False | FALSE 	 tag:yaml.org,2002:bool
 [-+]? [0-9]+ 	 tag:yaml.org,2002:int (Base 10)
 0o [0-7]+ 	 tag:yaml.org,2002:int (Base 8)
 0x [0-9a-fA-F]+ 	 tag:yaml.org,2002:int (Base 16)
 [-+]? ( \. [0-9]+ | [0-9]+ ( \. [0-9]* )? ) ( [eE] [-+]? [0-9]+ )? 	 tag:yaml.org,2002:float (Number)
 [-+]? ( \.inf | \.Inf | \.INF ) 	 tag:yaml.org,2002:float (Infinity)
 \.nan | \.NaN | \.NAN 	 tag:yaml.org,2002:float (Not a number)
 * 	 tag:yaml.org,2002:str (Default)

END_C_DECLS

#endif /* SRC_PVT_LOAD_CONFIG_H */

