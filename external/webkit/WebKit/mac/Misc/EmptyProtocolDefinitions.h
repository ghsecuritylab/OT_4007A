

#if defined(__OBJC__)

#if defined(BUILDING_ON_TIGER) || defined(BUILDING_ON_LEOPARD)
#define DELEGATES_DECLARED_AS_FORMAL_PROTOCOLS 0
#else
#define DELEGATES_DECLARED_AS_FORMAL_PROTOCOLS 1
#endif

#if !DELEGATES_DECLARED_AS_FORMAL_PROTOCOLS

#define EMPTY_PROTOCOL(NAME) \
@protocol NAME <NSObject> \
@end

EMPTY_PROTOCOL(NSTableViewDataSource)
EMPTY_PROTOCOL(NSTableViewDelegate)
EMPTY_PROTOCOL(NSWindowDelegate)

#undef EMPTY_PROTOCOL

#endif /* !DELEGATES_DECLARED_AS_FORMAL_PROTOCOLS */

#endif /* defined(__OBJC__) */
