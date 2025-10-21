# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# OfficeLabs Configuration Package
#

$(eval $(call gb_Package_Package,officelabs_config,$(SRCDIR)/officelabs/config))

$(eval $(call gb_Package_add_files,officelabs_config,$(LIBO_ETC_FOLDER),\
    officelabs.conf \
))

# vim: set noet sw=4 ts=4:

