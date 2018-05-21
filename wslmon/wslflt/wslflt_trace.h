#ifndef _WSLFLT_TRACE_H_
#define _WSLFLT_TRACE_H_

#define WPP_CONTROL_GUIDS                                               \
    WPP_DEFINE_CONTROL_GUID(                                            \
        wslfltGuid, (06C2591E,B39C,4F9F,96F2,9D4D58BE507C),             \
        WPP_DEFINE_BIT(TRACE_FLAG_WSLFLT)        /* bit  0 = 0x00000001 */    \
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
// FUNC WslLogTrace{LEVEL=TRACE_LEVEL_VERBOSE, FLAGS=TRACE_FLAG_WSLFLT}(MSG, ...);
// FUNC WslLogInfo{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=TRACE_FLAG_WSLFLT}(MSG, ...);
// FUNC WslLogWarning{LEVEL=TRACE_LEVEL_WARNING, FLAGS=TRACE_FLAG_WSLFLT}(MSG, ...);
// FUNC WslLogError{LEVEL=TRACE_LEVEL_ERROR, FLAGS=TRACE_FLAG_WSLFLT}(MSG, ...);
// FUNC WslLogCritical{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=TRACE_FLAG_WSLFLT}(MSG, ...);
//
// end_wpp

#endif // _WSLFLT_TRACE_H_