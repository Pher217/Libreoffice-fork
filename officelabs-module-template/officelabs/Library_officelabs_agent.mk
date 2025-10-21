# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# OfficeLabs Agent Library Build Definition
#

$(eval $(call gb_Library_Library,officelabs_agent))

# Source files
$(eval $(call gb_Library_add_exception_objects,officelabs_agent,\
    officelabs/agent/IAgentModule \
    officelabs/agent/WriterAgent \
    officelabs/agent/CalcAgent \
    officelabs/agent/ImpressAgent \
    officelabs/agent/MacroAgent \
    officelabs/agent/AgentRouter \
))

# Link against LibreOffice libraries
$(eval $(call gb_Library_use_libraries,officelabs_agent,\
    sal \
    cppu \
    cppuhelper \
    comphelper \
    officelabs_ipc \
    officelabs_context \
))

# Use external libraries
$(eval $(call gb_Library_use_externals,officelabs_agent,\
    boost_headers \
))

# Include directories
$(eval $(call gb_Library_set_include,officelabs_agent,\
    -I$(SRCDIR)/officelabs/agent \
    -I$(SRCDIR)/officelabs/ipc \
    -I$(SRCDIR)/officelabs/context \
    $$(INCLUDE) \
))

# vim: set noet sw=4 ts=4:

