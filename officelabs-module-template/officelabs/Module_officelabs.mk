# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# OfficeLabs Module Definition
# This file integrates the OfficeLabs AI layer into LibreOffice
#

$(eval $(call gb_Module_Module,officelabs))

# Add libraries when ready
# $(eval $(call gb_Module_add_targets,officelabs,\
#     Library_officelabs_agent \
#     Library_officelabs_context \
#     Library_officelabs_ipc \
#     Library_officelabs_ui \
#     Library_officelabs_git \
# ))

# Platform-specific packages
ifneq ($(OS),WNT)
$(eval $(call gb_Module_add_targets,officelabs,\
    Package_officelabs_config \
))
endif

# vim: set noet sw=4 ts=4:

