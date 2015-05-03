#include "malloc.h"

//remove following comment to get some debug output.
//#define _DEBUG_MLC_

void _output_debug(long x)
{
    long t;
    long i;
    long vec[16];

    if(x == 0)
        *(long*) STDOUT = '0';

    i = 0;
    while(x != 0)
    {
        t = x / 16;
        if(x - t * 16 < 10)
            vec[i] = x - t * 16 + '0';
        else
            vec[i] = x - t * 16 - 10 + 'A';

        x = t;
        i = i + 1;
    }
    i = i - 1;
    for(; i >= 0; i = i - 1)
    {
        *(long*)STDOUT = vec[i];
    }

    *(long*) STDOUT = 10;
    return;
}

//Shalloc Area Range = SAR
/*struct Shalloc_Area_Range {
    char *BASE;
    long SIZE; //the total size of this area range
    char *LTP; //the local top pointer
    long OPTION; //option type of this area range
};*/
#define SAR_BASE(x) (*(char **)x)
#define SAR_SIZE(x) (*(long*)(x + sizeof_uint64_t))
#define SAR_LTP(x) (*(char **)(x + sizeof_uint64_t + sizeof_long))
#define SAR_OPTION(x) (*(shalloc_option_t*)(x + sizeof_uint64_t * 2 + sizeof_long))
#define sizeof_SAR (sizeof_uint64_t * 2 + sizeof_long + sizeof_shalloc_option_t)

void _init()
{
    *(long*) _SAR_ARRAY_LAST_ELEMENT = (long) _SAR_ARRAY_BASE - sizeof_SAR;
    *(long*) _PA_TP = (long) _PA_BASE;
    *(long*) _IS_USED = 1;

    return;
}

//---------------------Shalloc----------------------------
char *_syscall_malloc(size_t size, shalloc_option_t option)
{
    //set system call id
    *(long*) SYSCALL_ID_ADDR = (long) SYSCALL_ID_MALLOC_EXT;

    //set system call input arguments
    *(size_t*) SYSCALL_COMM_AREA_ADDR = size;

    *(shalloc_option_t*) (SYSCALL_COMM_AREA_ADDR + sizeof_size_t) = option;

    //call soft interrupt of system call
    asm("int 0x80");

    //get output value of system call
    return *(char **) SYSCALL_COMM_AREA_ADDR;
}

//align size to a large number, which should always be a multiplier of PAGE_SIZE(512)
size_t _s_align_to(size_t size)
{
    return ((size - 1) / (_S_ALIGN_TO_SIZE) + 1) * (_S_ALIGN_TO_SIZE);
}

size_t _align_to_page(size_t size)
{
    return (size + PAGE_SIZE - 1) / (PAGE_SIZE) * (PAGE_SIZE);
}

char *_get_new_sar(size_t size, size_t align_size, shalloc_option_t option)
{
    char *ret;
    long last_element;

    //call a system call to ask for new SAR
    ret = _syscall_malloc(align_size, option);

#ifdef _DEBUG_MLC_
    output_char('s');
    output_char('y');
    output_char('s');
    output_char('_');
    output_char('r');
    output_char('t');
    output_char(':');
    output_q_hex(ret);
    output_char(C_n);
#endif
    if((long)ret != NULL) {
        *(long*) _SAR_ARRAY_LAST_ELEMENT = (*(long*) _SAR_ARRAY_LAST_ELEMENT) + sizeof_SAR;

        //push back this new SAR as the last element of SAR_ARRAY
        last_element = *(long*) _SAR_ARRAY_LAST_ELEMENT;

        SAR_BASE(last_element) = ret;
        SAR_SIZE(last_element) = align_size;
        SAR_LTP(last_element) = ret + size;
        SAR_OPTION(last_element) = option;

#ifdef _DEBUG_MLC_
        output_q_hex(SAR_BASE(last_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_SIZE(last_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_LTP(last_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_OPTION(last_element));
        *(long*)STDOUT = 10;
        *(long*)STDOUT = 'N';
        *(long*)STDOUT = 10;
#endif

    }

    return ret;
}

//search in SAR_ARRAY
char *_get_from_sar_array(size_t size, shalloc_option_t option)
{
    long i;
    long last_element;
    long *sar_array_element;
    char *old_value;

#ifdef _DEBUG_MLC_
     _output_debug((long)_SAR_ARRAY_BASE);
     _output_debug(*(long*)(_SAR_ARRAY_LAST_ELEMENT));
     *(long*)STDOUT = 'Z';
     *(long*)STDOUT = 10;
#endif

    last_element = *(long*) _SAR_ARRAY_LAST_ELEMENT;

    for(sar_array_element = (long*)_SAR_ARRAY_BASE;
        (long)sar_array_element <= last_element;
        sar_array_element = sar_array_element + sizeof_SAR)
    {
#ifdef _DEBUG_MLC_
        output_q_hex(SAR_BASE(sar_array_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_SIZE(sar_array_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_LTP(sar_array_element));
        *(long*)STDOUT = 10;
        output_q_hex(SAR_OPTION(sar_array_element));
        *(long*)STDOUT = 10;
        *(long*) STDOUT = 'Q';
        *(long*) STDOUT = 10;
#endif

        //if we find a SAR has enough space to allocate requested memory
        //first-fit strategy
        if((long)SAR_OPTION(sar_array_element) == (long)option) 
        {
            if((long)SAR_SIZE(sar_array_element) >= (long)SAR_LTP(sar_array_element) - (long)SAR_BASE(sar_array_element) + (long)size)
            {
#ifdef _DEBUG_MLC_
                *(long*) STDOUT = 'V';
                *(long*) STDOUT = 10;
#endif
                old_value = SAR_LTP(sar_array_element);
                SAR_LTP(sar_array_element) = SAR_LTP(sar_array_element) + size;
                return  old_value;
            }
        }
    }

    //if we do not find satisfied SAR
    return (char*)NULL;
}

// allocate memory ranges according the the option
// on success, return the allocated address
// on fail, return NULL
void *shalloc_ext(size_t size, shalloc_option_t option)
{
    size_t align_size;
    char* ret;

    //if input SIZE is not positive value.
    if(size <= 0)
        return (void*)NULL;

#ifdef _DEBUG_MLC_
    output_q_hex(size);
    *(long*)STDOUT = 10;
#endif

#ifdef STANDALONE_SHALLOC
    size = _align_to_page(size);
#endif

    //align to a large area
    align_size = _s_align_to(size);

#ifdef _DEBUG_MLC_
    output_q_hex(size);
    output_char(C_n);
    output_q_hex(align_size);
    output_char(C_n);
    output_q_hex(*(long*) _IS_USED);
    output_char(C_n);
#endif

    //if SAR_ARRAY has not been initialized
    //then ask system for a shalloc area
    if((long)(*(long *) _IS_USED) == 0)
    {
        _init();

        //get new SAR from system.
        ret = _get_new_sar(size, align_size, option);
    }
    else //if SAR_ARRAY has been initialized
    {
        //search from SAR_ARRAY, to get requested memory
        ret = _get_from_sar_array(size, option);

#ifdef _DEBUG_MLC_
        _output_debug((long)ret);
        *(long*) STDOUT = 'T';
        *(long*) STDOUT = 10;
#endif

        //then ask system for a shalloc area.
        if(ret == (char*) NULL)
        {
            if ( (long)((*(long*) _SAR_ARRAY_LAST_ELEMENT) + sizeof_SAR * 2) < (long)_SAR_ARRAY_END )
            {
                //get new SAR from system.
                ret = _get_new_sar(size, align_size, option);
            }
            else
                ret = (char *)NULL;
                //TODO: not return null.
                //LATS_ELEMENT should restart from BASE, to overlap oldest element.

        }
    }

    return (void *) ret;
}

void *shalloc(size_t size) {
    return shalloc_ext(size, SR_N);
}

//----------------------Pralloc-------------------------

//align size to a large number, which should always be a multiplier of PAGE_SIZE(512)
size_t _p_align_to(size_t size)
{
#ifdef _DEBUG_MLC_
    _output_debug(size);
#endif
    return ((size - 1) / (_P_ALIGN_TO_SIZE) + 1) * (_P_ALIGN_TO_SIZE);
}

char *_get_new_pa(size_t size)
{
    char *ret;

#ifdef _DEBUG_MLC_
    _output_debug(*(long*) _PA_TP);
    *(long*) STDOUT = 'P';
    *(long*) STDOUT = 10;
#endif

    if ((long)((*(long*)_PA_TP) - size) > (long)_PA_END )
    {
        *(long*) _PA_TP = *(long*) _PA_TP - size;
        ret = *(char**) _PA_TP;
    }
    else
        ret = (char *)NULL;

#ifdef _DEBUG_MLC_
    _output_debug(*(long*) _PA_TP);
#endif

    return ret;
}

void *pralloc(size_t size)
{
    size_t align_size;
    char* ret;

    //if input SIZE is not positive value.
    if(size <= 0)
        return (void*)NULL;

    //align to a large area
    align_size = _p_align_to(size);

#ifdef _DEBUG_MLC_
    _output_debug(align_size);
    *(long*) STDOUT = 10;
    output_q(*(long*) _IS_USED);
    *(long*) STDOUT = 10;
    *(long*) STDOUT = 'K';
    *(long*) STDOUT = 10;
#endif

    //if pralloc area metadata has not been initialized
    if((long)(*(long *) _IS_USED) == 0)
    {
        _init();
    }

    //get new pralloc area from system.
    ret = _get_new_pa(align_size);

    return (void*) ret;
}

