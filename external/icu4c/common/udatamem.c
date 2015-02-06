



#include "unicode/utypes.h"
#include "cmemory.h"
#include "unicode/udata.h"

#include "udatamem.h"

void UDataMemory_init(UDataMemory *This) {
    uprv_memset(This, 0, sizeof(UDataMemory));
    This->length=-1;
}


void UDatamemory_assign(UDataMemory *dest, UDataMemory *source) {
    /* UDataMemory Assignment.  Destination UDataMemory must be initialized first.  */
    UBool mallocedFlag = dest->heapAllocated;
    uprv_memcpy(dest, source, sizeof(UDataMemory));
    dest->heapAllocated = mallocedFlag;
}

UDataMemory *UDataMemory_createNewInstance(UErrorCode *pErr) {
    UDataMemory *This;

    if (U_FAILURE(*pErr)) {
        return NULL;
    }
    This = uprv_malloc(sizeof(UDataMemory));
    if (This == NULL) {
        *pErr = U_MEMORY_ALLOCATION_ERROR; }
    else {
        UDataMemory_init(This);
        This->heapAllocated = TRUE;
    }
    return This;
}


const DataHeader *
UDataMemory_normalizeDataPointer(const void *p) {
    /* allow the data to be optionally prepended with an alignment-forcing double value */
    const DataHeader *pdh = (const DataHeader *)p;
    if(pdh==NULL || (pdh->dataHeader.magic1==0xda && pdh->dataHeader.magic2==0x27)) {
        return pdh;
    } else {
#ifdef OS400
        /*
        TODO: Fix this once the compiler implements this feature. Keep in sync with genccode.c

        This is here because this platform can't currently put
        const data into the read-only pages of an object or
        shared library (service program). Only strings are allowed in read-only
        pages, so we use char * strings to store the data.

        In order to prevent the beginning of the data from ever matching the
        magic numbers we must skip the initial double.
        [grhoten 4/24/2003]
        */
        return (const DataHeader *)*((const void **)p+1);
#else
        return (const DataHeader *)((const double *)p+1);
#endif
    }
}


void UDataMemory_setData (UDataMemory *This, const void *dataAddr) {
    This->pHeader = UDataMemory_normalizeDataPointer(dataAddr);
}


U_CAPI void U_EXPORT2
udata_close(UDataMemory *pData) {
    if(pData!=NULL) {
        uprv_unmapFile(pData);
        if(pData->heapAllocated ) {
            uprv_free(pData);
        } else {
            UDataMemory_init(pData);
        }
    }
}

U_CAPI const void * U_EXPORT2
udata_getMemory(UDataMemory *pData) {
    if(pData!=NULL && pData->pHeader!=NULL) {
        return (char *)(pData->pHeader)+udata_getHeaderSize(pData->pHeader);
    } else {
        return NULL;
    }
}

U_CAPI int32_t U_EXPORT2
udata_getLength(const UDataMemory *pData) {
    if(pData!=NULL && pData->pHeader!=NULL && pData->length>=0) {
        /*
         * subtract the header size,
         * return only the size of the actual data starting at udata_getMemory()
         */
        return pData->length-udata_getHeaderSize(pData->pHeader);
    } else {
        return -1;
    }
}

U_CAPI const void * U_EXPORT2
udata_getRawMemory(const UDataMemory *pData) {
    if(pData!=NULL && pData->pHeader!=NULL) {
        return pData->pHeader;
    } else {
        return NULL;
    }
}

UBool  UDataMemory_isLoaded(UDataMemory *This) {
    return This->pHeader != NULL;
}
