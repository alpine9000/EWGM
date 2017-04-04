include common.mk

all: $(LOCAL)/bin/wget

$(LOCAL)/bin/wget:
	cp wget $(LOCAL)/bin/wget
	chmod +x $(LOCAL)/bin/wget
