#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T23:51:01
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = S5
TEMPLATE = app

SOURCES += \
    src/main.cpp\
    src/window.cpp \
    src/glwidget.cpp \
    src/core/scenegraph/node.cpp \
    src/core/scenegraph/scenegraph.cpp \
    src/core/properties/propertyset.cpp \
    src/core/properties/iproperty.cpp \
    src/core/maths/trigo.cpp \
    src/core/maths/frustum.cpp \
    src/core/maths/boundingsphere.cpp \
    src/core/maths/aabb.cpp \
	src/core/scenegraph/scenegraphview.cpp \
    src/core/scenegraph/scenegraphmodel.cpp

HEADERS  += \
    include/window.h \
    include/glwidget.h \
    include/qtlogo.h \
    include/core/utils/parenting.h \
    include/core/utils/parenting.hpp \
    include/core/scenegraph/node.h \
    include/core/scenegraph/scenegraph.h \
    include/core/properties/propertyset.h \
    include/core/properties/iproperty.h \
    include/core/maths/vector4.hpp \
    include/core/maths/vector4.h \
    include/core/maths/vector3.hpp \
    include/core/maths/vector3.h \
    include/core/maths/vector2.hpp \
    include/core/maths/vector2.h \
    include/core/maths/trigo.h \
    include/core/maths/transform.hpp \
    include/core/maths/transform.h \
    include/core/maths/plane.h \
    include/core/maths/matrix4.hpp \
    include/core/maths/matrix4.h \
    include/core/maths/matrix3.hpp \
    include/core/maths/matrix3.h \
    include/core/maths/frustum.h \
    include/core/maths/constants.h \
    include/core/maths/boundingvolume.h \
    include/core/maths/boundingsphere.h \
    include/core/maths/aabb.h \
	include/core/scenegraph/scenegraphmodel.h \
	include/core/scenegraph/scenegraphview.h

FORMS    +=
