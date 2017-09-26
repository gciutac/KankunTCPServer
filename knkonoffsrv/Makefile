############################################
# OpenWrt Makefile for knkonoffsrv program #
############################################

include $(TOPDIR)/rules.mk

# Package version info:
PKG_NAME:=knkonoffsrv
PKG_VERSION:=1.01.01
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

# Package information:
define Package/knkonoffsrv
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=knkonoffsrv
endef

define Package/knkonoffsrv/description
	Kankun TCP Server
endef

TARGET_LDFLAGS+=-rpath-link $(shell find $(STAGING_DIR) -name ld-uClibc.so.0)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/knkonoffsrv/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/knkonoffsrv $(1)/bin/
endef

# Build package:
$(eval $(call BuildPackage,knkonoffsrv))
