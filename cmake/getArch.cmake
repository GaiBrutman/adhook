set(supported_archs
    i386
    x86_64
)

if (NOT DEFINED ARCH)
    message( SEND_ERROR "ARCH variable must be defined" )
elseif (NOT "${ARCH}" IN_LIST supported_archs)
    message( SEND_ERROR "${ARCH} not in ${supported_archs}" )
endif()
