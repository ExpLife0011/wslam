#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <fltKernel.h>
#include "TypeTraits.hpp"
#include "FilterTags.hpp"
//#include "PagedObject.hpp"

namespace WslFlt
{

    typedef VOID
    (*PFUNC_CONTEXT_INIT) (
        _In_ PVOID Context,
        _In_ FLT_CALLBACK_DATA Data
    );

    template <typename _Ty>
    struct ContextTraits
    {
        static constexpr FLT_CONTEXT_TYPE Type = 0;
        static constexpr USHORT Flags = 0;
        static constexpr PFLT_CONTEXT_CLEANUP_CALLBACK ContextCleanupCallback = nullptr;
        static constexpr SIZE_T Size = 0;
        static constexpr ULONG PoolTag = 0;
    };


    class FileContext
    {
    public:
        _Function_class_(PFLT_CONTEXT_CLEANUP_CALLBACK)
        static
        void
        FLTAPI
        Cleanup(
            _In_ PFLT_CONTEXT Context,
            _In_ FLT_CONTEXT_TYPE ContextType
        );
    };

    template<>
    struct TypeTraits<FileContext>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_FILE_CONTEXT;
        static constexpr POOL_TYPE PoolType = PagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };


    template <>
    struct ContextTraits<FileContext>
    {
        static constexpr FLT_CONTEXT_TYPE Type = FLT_FILE_CONTEXT;
        static constexpr USHORT Flags = 0;
        static constexpr PFLT_CONTEXT_CLEANUP_CALLBACK ContextCleanupCallback = FileContext::Cleanup;
        static constexpr SIZE_T Size = sizeof(FileContext);
    };


    class StreamContext
    {
    public:
        _Function_class_(PFLT_CONTEXT_CLEANUP_CALLBACK)
        static
        void
        FLTAPI
        Cleanup(
            _In_ PFLT_CONTEXT Context,
            _In_ FLT_CONTEXT_TYPE ContextType
        );
    };


    template<>
    struct TypeTraits<StreamContext>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_STREAM_CONTEXT;
        static constexpr POOL_TYPE PoolType = PagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };


    template <>
    struct ContextTraits<StreamContext>
    {
        static constexpr FLT_CONTEXT_TYPE Type = FLT_STREAM_CONTEXT;
        static constexpr USHORT Flags = 0;
        static constexpr PFLT_CONTEXT_CLEANUP_CALLBACK ContextCleanupCallback = StreamContext::Cleanup;
        static constexpr SIZE_T Size = sizeof(StreamContext);
    };


    class StreamHandleContext
    {
    public:
        _Function_class_(PFLT_CONTEXT_CLEANUP_CALLBACK)
        static
        void
        FLTAPI
        Cleanup(
            _In_ PFLT_CONTEXT Context,
            _In_ FLT_CONTEXT_TYPE ContextType
        );
    };

    template<>
    struct TypeTraits<StreamHandleContext>
    {
        static constexpr ULONG Tag = WSLFLT_TAG_STREAMHANDLE_CONTEXT;
        static constexpr POOL_TYPE PoolType = PagedPool;
        static constexpr LONG NumberOfCachableElements = DEFAULT_NUMBER_OF_CACHABLE_ELEMENTS;
    };

    template <>
    struct ContextTraits<StreamHandleContext>
    {
        static constexpr FLT_CONTEXT_TYPE Type = FLT_STREAMHANDLE_CONTEXT;
        static constexpr USHORT Flags = 0;
        static constexpr PFLT_CONTEXT_CLEANUP_CALLBACK ContextCleanupCallback = StreamHandleContext::Cleanup;
        static constexpr SIZE_T Size = sizeof(StreamHandleContext);
    };

    struct WslFileContexts
    {
        FileContext* WslFileContext;
        StreamContext* WslStreamContext;
        StreamHandleContext* WslStreamHandleContext;
    };

    class WslCompletionContext
    {
    public:
        static constexpr LONG NumberOfCachableElements = 100;

        ULONG Requestor;
        WslFileContexts* Contexts;
    };
}
   

#endif // _CONTEXT_HPP_