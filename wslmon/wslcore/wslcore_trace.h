#ifndef _WSLCORE_TRACE_H_
#define _WSLCORE_TRACE_H_

#define WPP_CONTROL_GUIDS                                                   \
    WPP_DEFINE_CONTROL_GUID(                                                \
        wslCoreGuid, (EB8AB225,17D4,4E39,88D1,69224505F87F),                \
        WPP_DEFINE_BIT(TRACE_FLAG_WSLCORE)        /* bit  0 = 0x00000001 */ \
    )

#define WPP_LEVEL_FLAGS_LOGGER(lvl,flags) \
           WPP_LEVEL_LOGGER(flags)

#define WPP_LEVEL_FLAGS_ENABLED(lvl, flags) \
           (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= lvl)

//
// This comment block is scanned by the trace preprocessor to define our
// TraceEvents function.
//
// begin_wpp config
//
// FUNC WslCoreLogTrace{LEVEL=TRACE_LEVEL_VERBOSE, FLAGS=TRACE_FLAG_WSLCORE}(MSG, ...);
// FUNC WslCoreLogInfo{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=TRACE_FLAG_WSLCORE}(MSG, ...);
// FUNC WslCoreLogWarning{LEVEL=TRACE_LEVEL_WARNING, FLAGS=TRACE_FLAG_WSLCORE}(MSG, ...);
// FUNC WslCoreLogError{LEVEL=TRACE_LEVEL_ERROR, FLAGS=TRACE_FLAG_WSLCORE}(MSG, ...);
// FUNC WslCoreLogCritical{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=TRACE_FLAG_WSLCORE}(MSG, ...);
//
// end_wpp

#endif // _WSLCORE_TRACE_H_
