# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# OfficeLabs IPC Library Build Definition
#

$(eval $(call gb_Library_Library,officelabs_ipc))

# Source files
$(eval $(call gb_Library_add_exception_objects,officelabs_ipc,\
    officelabs/ipc/AgentConnection \
    officelabs/ipc/JsonRpc \
))

# Platform-specific sources
ifeq ($(OS),WNT)
$(eval $(call gb_Library_add_exception_objects,officelabs_ipc,\
    officelabs/ipc/NamedPipe \
))
else
$(eval $(call gb_Library_add_exception_objects,officelabs_ipc,\
    officelabs/ipc/UnixSocket \
))
endif

# Link against LibreOffice libraries
$(eval $(call gb_Library_use_libraries,officelabs_ipc,\
    sal \
    cppu \
    cppuhelper \
))

# Use external libraries
$(eval $(call gb_Library_use_externals,officelabs_ipc,\
    boost_headers \
))

# Include directories
$(eval $(call gb_Library_set_include,officelabs_ipc,\
    -I$(SRCDIR)/officelabs/ipc \
    $$(INCLUDE) \
))

# vim: set noet sw=4 ts=4:

