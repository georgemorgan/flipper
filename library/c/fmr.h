/* fmr.h - Flipper Message Runtime macros, types, and implementation. */

#ifndef __lf_fmr_h__
#define __lf_fmr_h__

#include "defines.h"

/* The size of a single FMR packet expressed in bytes. */
#define FMR_PACKET_SIZE 64
/* The magic number that indicates the start of a packet. */
#define FMR_MAGIC_NUMBER 0xFE

/* Define types exposed by the FMR API. */

/* The variadic argument type. Used to hold argument metadata and value during parsing. */
typedef uint64_t lf_va;
/* The largest argument type. All argument values are held within a variable of this type. */
typedef uint64_t lf_arg;
/* Used to hold the index of a standard or user module in which a function counterpart exists. */
typedef uint32_t lf_module;
/* Used to hold the offset index of a function within a module. */
typedef uint8_t lf_function;
/* Used to hold the number of parameters that are to be passed during a procedure call. */
typedef uint8_t lf_argc;

/* The maximum number of arguments that can be encoded into a packet. */
#define FMR_MAX_ARGC 16
/* Used to hold encoded prameter types within invocation metadata.
   NOTE: This type must be capable of encoding the exact number of bits
         given by (FMR_MAX_ARGC * 2).
*/
typedef uint32_t lf_types;

/* Converts a C type into an unsigned lf_type. */
#define lf_utype(type) (sizeof(type) - 1)
/* Converts a C type into a signed lf_type. */
#define lf_stype(type) ((1 << 3) | lf_utype(type))

/* Enumerates the basic type signatures an argument can be classified as. */
enum {
    lf_void_t = 2,  // 2
    lf_int_t = 4,   // 4
    lf_ptr_t = 6,   // 6

    /* Unsigned types. */
    lf_uint8_t = lf_utype(uint8_t),    // 0
    lf_uint16_t = lf_utype(uint16_t),  // 1
    lf_uint32_t = lf_utype(uint32_t),  // 3
    lf_uint64_t = lf_utype(uint64_t),  // 7

    /* Signed types. */
    lf_int8_t = lf_stype(int8_t),    // 8
    lf_int16_t = lf_stype(int16_t),  // 9
    lf_int32_t = lf_stype(int32_t),  // 11
    lf_int64_t = lf_stype(int64_t),  // 15

    /* Max type is 15. */
    lf_max_t = 15
};

/* A type used to reference the values in the enum above. */
typedef uint8_t lf_type;

/* Calculates the length of an FMR type. */
int lf_sizeof(lf_type type);

/* Parameter list building macros. */

/* Counts the number of arguments within a variadic argument macro. */
#define __fmr_count_implicit(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _a, _b, _c, _d, _e, _f, _10, n, ...) n
#define __fmr_count(...) \
    __fmr_count_implicit(_, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/* Generates and returns a pointer to an 'fmr_parameters' given a list of variadic arguments. */
#define lf_args(...) fmr_build((__fmr_count(__VA_ARGS__) / 2), ##__VA_ARGS__)

/* Parser macros for variables. */

/* Creates an 'lf_va' from an 'lf_type' and an immediate value. */
#define lf_intx(type, arg) (lf_type)type, (lf_arg)(uintptr_t)arg
/* Gives the 'lf_va' for a given integer's value. */
#define lf_int(arg) lf_intx(lf_int_t, arg)
/* Gives the 'lf_va' for a given 8-bit integer's value. */
#define lf_int8(arg) lf_intx(lf_int8_t, (uint8_t)arg)
/* Gives the 'lf_va' for a given 16-bit integer's value. */
#define lf_int16(arg) lf_intx(lf_int16_t, (uint16_t)arg)
/* Gives the 'lf_va' for a given 32-bit integer's value. */
#define lf_int32(arg) lf_intx(lf_int32_t, (uint32_t)arg)
/* Gives the 'lf_va' for a given 8-bit integer's value. */
#define lf_uint8(arg) lf_intx(lf_uint8_t, (uint8_t)arg)
/* Gives the 'lf_va' for a given 16-bit integer's value. */
#define lf_uint16(arg) lf_intx(lf_uint16_t, (uint16_t)arg)
/* Gives the 'lf_va' for a given 32-bit integer's value. */
#define lf_uint32(arg) lf_intx(lf_uint32_t, (uint32_t)arg)
/* Gives the 'lf_va' for a pointer. */
#define lf_ptr(arg) lf_intx(lf_ptr_t, (uintptr_t)arg)
/* Creates an 'lf_va' from a C variable. */
#define lf_infer(variable) lf_intx(lf_utype(variable), variable)

/* Exposes all message runtime packet classes. */
enum {
    /* executes a function on the device */
    fmr_rpc_class,
    /* moves data from the host's memory to the device's memory */
    fmr_push_class,
    /* moves data from the device's memory to the host's memory */
    fmr_pull_class,
    /* interfaces with the dynamic loader */
    fmr_dyld_class,
    /* allocates memory on the device */
    fmr_malloc_class,
    /* frees memory on the device */
    fmr_free_class,
    /* total count */
    FMR_CLASS_COUNT
};

/* A type used to reference the values in the enum above. */
typedef uint8_t fmr_class;

/* Contains the information required to obtain, verify, and parse a packet. */
struct LF_PACKED _fmr_header {
    /* A magic number indicating the start of the packet. */
    uint8_t magic;
    /* The checksum of the packet's contents. */
    lf_crc_t crc;
    /* The length of the packet expressed in bytes. */
    uint16_t len;
    /* The packet's type. */
    fmr_class type;
};

/* Standardizes the notion of an argument. */
struct LF_PACKED _lf_arg {
    /* The type of the argument. */
    lf_type type;
    /* The value of the argument. */
    lf_arg value;
};

/* Generic packet data type that can be passed around by packet parsing equipment. */
struct LF_PACKED _fmr_packet {
    /* The header shared by all packet classes. */
    struct _fmr_header hdr;
    /* A generic payload that is designed to be casted against the class specific data structures. */
    uint8_t payload[(FMR_PACKET_SIZE - sizeof(struct _fmr_header))];
};

/* Procedure call metadata carried by a packet. */
struct LF_PACKED _fmr_call {
    /* The index of the module in which the target routine resides. */
    uint8_t module;
    /* The index of the function within the module. */
    uint8_t function;
    /* The return type. */
    lf_type ret;
    /* The types of the encoded parameters. */
    lf_types argt;
    /* The number of encoded parameters. */
    lf_argc argc;
    /* The encoded values of the parameters to be passed to the callee. */
    uint8_t argv[];
};

/* Contains metadata needed to perform a remote procedure call on a device. */
struct LF_PACKED _fmr_call_packet {
    /* The packet header programmed with 'fmr_rpc_class'. */
    struct _fmr_header hdr;
    /* The procedure call information of the invocation. */
    struct _fmr_call call;
};

/* Contains metadata needed to perform a push/pull operation. */
struct LF_PACKED _fmr_push_pull_packet {
    /* The packet header programmed with 'fmr_push_class' or 'fmr_pull_class'. */
    struct _fmr_header hdr;
    /* The amount of data to be transferred. */
    uint32_t len;
    /* The src/dst on the device. */
    uint64_t ptr;
};

/* Asks the dynamic loader for a module index. */
struct LF_PACKED _fmr_dyld_packet {
    /* The packet header programmed with 'fmr_dyld_class'. */
    struct _fmr_header hdr;
    /* The module name. */
    char module[];
};

/* Allocates memory on the device. */
struct LF_PACKED _fmr_memory_packet {
    /* The packet header programmed with 'fmr_dyld_class'. */
    struct _fmr_header hdr;
    /* The size. */
    uint32_t size;
    /* The pointer. */
    uint64_t ptr;
};

/* A generic datastructure that is sent back following any message runtime trancsaction. */
struct LF_PACKED _fmr_result {
    /* The return value of the function called (if any). */
    lf_return_t value;
    /* The error code generated on the device. */
    uint8_t error;
    /* NOTE: Add bitfield indicating the need to poll for updates. */
};

/* Appends an argument to the linked list. */
int lf_append(struct _lf_ll *list, lf_type type, lf_arg value);

/* Generates the appropriate data structure needed for the remote procedure call of 'function' in 'module'. */
int lf_create_call(lf_module module, lf_function function, lf_type ret, struct _lf_ll *args, struct _fmr_header *header,
                   struct _fmr_call *call);

/* Creates a struct _lf_arg * type. */
struct _lf_arg *lf_arg_create(lf_type type, lf_arg value);

/* Builds an fmr_parameters from a set of variadic arguments provided by the fmr_parameters macro. */
struct _lf_ll *fmr_build(int argc, ...);

/* Executes an fmr_packet and stores the result of the operation in the result buffer provided. */
int fmr_perform(struct _lf_device *device, struct _fmr_packet *packet);

/* platform specific internal functions implemented in assembly */
extern lf_return_t fmr_call(lf_return_t (*const function)(void), lf_type ret, uint8_t argc, uint16_t argt, const void *argv);

#endif
