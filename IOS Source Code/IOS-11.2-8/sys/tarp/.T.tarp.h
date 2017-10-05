
tarp.h,4342
#define __TARP_H__43,1444
#define TARP_PABUGINF 45,1464
#define TARP_EVBUGINF 46,1513
typedef struct tarp_pdutype_ tarp_pdutype_53,1657
} tarp_pdutype;tarp_pdutype77,2815
#define TARP_URC_MASK 79,2836
#define TARP_TYPE_CODE_MASK 80,2907
#define TARP_GET_URC(TARP_GET_URC82,2984
#define TARP_GET_CODE(TARP_GET_CODE83,3045
#define tarp_urc 85,3114
#define tarp_code 86,3145
#define TARP_PDU_HEADER_SIZE 95,3408
#define TARP_MAX_TID_LEN 97,3470
#define TARGET_TID(TARGET_TID103,3613
#define SOURCE_TID(SOURCE_TID104,3651
#define SOURCE_PROT_ADDR(SOURCE_PROT_ADDR105,3709
#define TARP_SUPPORTED_PROTOCOL(TARP_SUPPORTED_PROTOCOL113,3918
#define TARP_RQST_TID_L1_PDU 132,4717
#define TARP_RQST_TID_L2_PDU 133,4749
#define TARP_RESPONSE_PDU 134,4781
#define TARP_NOTIFY_PDU 135,4813
#define TARP_RQST_PROT_PDU 136,4845
#define TARP_DEF_TYPE1_TIMEOUT 158,5616
#define TARP_DEF_TYPE2_TIMEOUT 159,5689
#define TARP_DEF_ARP_RQST_TIMEOUT 160,5762
#define TARP_DEF_POST_TYPE2_TIMEOUT 161,5835
#define TARP_DEF_LDB_FLUSH_TIMER 162,5908
#define TARP_DEF_LDB_ZERO_SEQ_TIMER 163,5981
#define TARP_DEF_TID_CACHE_TIMER 164,6054
#define TARP_DEF_LIFETIME 166,6128
#define TARP_NSAP_PROT_ADDR_TYPE 167,6202
#define TARP_DEF_NSEL_TYPE 169,6290
#define TARP_MIN_TYPE1_TIMEOUT 172,6383
#define TARP_MIN_TYPE2_TIMEOUT 173,6424
#define TARP_MIN_ARP_RQST_TIMEOUT 174,6465
#define TARP_MIN_POST_TYPE2_TIMEOUT 175,6541
#define TARP_MIN_LDB_FLUSH_TIMER 176,6582
#define TARP_MIN_LDB_ZERO_SEQ_TIMER 177,6623
#define TARP_MIN_TID_CACHE_TIMER 178,6692
#define TARP_MIN_SEQUENCE_NUMBER 179,6762
#define TARP_MIN_LIFETIME 180,6803
#define TARP_MIN_PROTOCOL_TYPE 181,6849
#define TARP_MIN_NSEL_TYPE 182,6892
#define TARP_MAX_TYPE1_TIMEOUT 185,6950
#define TARP_MAX_TYPE2_TIMEOUT 186,7023
#define TARP_MAX_ARP_RQST_TIMEOUT 187,7096
#define TARP_MAX_POST_TYPE2_TIMEOUT 188,7169
#define TARP_MAX_LDB_FLUSH_TIMER 189,7242
#define TARP_MAX_LDB_ZERO_SEQ_TIMER 190,7317
#define TARP_MAX_TID_CACHE_TIMER 191,7390
#define TARP_MAX_SEQUENCE_NUMBER 192,7465
#define TARP_MAX_LIFETIME 193,7544
#define TARP_MAX_PROTOCOL_TYPE 195,7636
#define TARP_MAX_NSEL_TYPE 197,7732
enum {TARP_INPUT,202,7849
enum {TARP_INPUT, TARP_OUTPUT,202,7849
enum {TARP_INPUT, TARP_OUTPUT, TARP_HDRSYNTAX,202,7849
enum {TARP_INPUT, TARP_OUTPUT, TARP_HDRSYNTAX, TARP_NOMEM,202,7849
enum {TARP_INPUT, TARP_OUTPUT, TARP_HDRSYNTAX, TARP_NOMEM, TARP_BADPACKET,202,7849
      TARP_LIFETIME_EXCEEDED,203,7924
      TARP_MAXTRAFFIC}205,7955
#define TARP_PERIODIC_TIMER 207,7980
#define TARP_SEND_TYPE4_TIMER 208,8040
typedef struct tarp_tid_cache_type_ tarp_tid_cache_type_214,8118
} tarp_tid_cache_type;tarp_tid_cache_type221,8556
#define TARP_STATIC_FLAG 226,8612
#define TARP_MY_ENTRY_FLAG 227,8643
enum TARP_TID_NET_KEY TARP_TID_NET_KEY232,8720
enum TARP_TID_NET_KEY {TARP_TID_KEY,232,8720
enum TARP_TID_NET_KEY {TARP_TID_KEY, TARP_NET_KEY}232,8720
enum TARP_SRC_DST TARP_SRC_DST237,8820
enum TARP_SRC_DST {TARP_SOURCE,237,8820
enum TARP_SRC_DST {TARP_SOURCE, TARP_DEST}237,8820
typedef struct tarp_ldb_type_ tarp_ldb_type_248,8999
} tarp_ldb_type;tarp_ldb_type255,9391
enum TARP_TABLE_TYPE TARP_TABLE_TYPE263,9493
enum TARP_TABLE_TYPE {TARP_TID_CACHE,263,9493
enum TARP_TABLE_TYPE {TARP_TID_CACHE, TARP_LDB_CACHE}263,9493
typedef struct tarp_static_adjQ_type_ tarp_static_adjQ_type_277,10150
} tarp_static_adjQ_type;tarp_static_adjQ_type282,10407
enum TARP_ADJ_ENTRY_TYPE TARP_ADJ_ENTRY_TYPE284,10433
enum TARP_ADJ_ENTRY_TYPE {TARP_BLACKLIST,284,10433
enum TARP_ADJ_ENTRY_TYPE {TARP_BLACKLIST, TARP_STATIC_MAP,284,10433
enum TARP_ADJ_ENTRY_TYPE {TARP_BLACKLIST, TARP_STATIC_MAP, TARP_STATIC_ADJ,284,10433
			  TARP_MAX_QUEUE}285,10509
enum TARP_RESPONSE_TYPE TARP_RESPONSE_TYPE305,11436
enum TARP_RESPONSE_TYPE {TARP_RESP_FAILURE,305,11436
			 TARP_RESP_SUCCESS,306,11480
			 TARP_RESP_SUCCESS, TARP_RESP_TIMEOUT,306,11480
			 TARP_RESP_INTERRUPTED}307,11523
typedef struct tarp_rqstQ_type_ tarp_rqstQ_type_309,11552
} tarp_rqstQ_type;tarp_rqstQ_type316,11823
enum TARP_HOST_TYPE TARP_HOST_TYPE321,11884
enum TARP_HOST_TYPE {TARP_NULL_HOST_TYPE,321,11884
enum TARP_HOST_TYPE {TARP_NULL_HOST_TYPE, TARP_IS_TYPE,321,11884
enum TARP_HOST_TYPE {TARP_NULL_HOST_TYPE, TARP_IS_TYPE, TARP_ES_TYPE}321,11884