
TEMPLATE = subdirs

SUBDIRS = src \
          test

src.file = src/radon.pro

test.file    = test/radontest.pro
test.depends = src

