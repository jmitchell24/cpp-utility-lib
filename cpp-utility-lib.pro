#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T15:51:44
#
#-------------------------------------------------

QT       -= core gui

TARGET = cpp-utility-lib
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

# isn't already defined in release builds for some reason
CONFIG(release, debug|release) { DEFINES += NDEBUG }

DEFINES -= UNICODE

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    src/cml/CmlDocument.hpp \
    src/cml/CmlGroup.hpp \
    src/cml/CmlItem.hpp \
    src/cml/CmlReader.hpp \
    src/cml/CmlSerialize.hpp \
    src/cml/Types.hpp \
    src/func/Delegate.hpp \
    src/func/Event.hpp \
    src/func/EventOnce.hpp \
    src/func/EventSorted.hpp \
    src/func/Signal.hpp \
    src/math/collision/Bound2.hpp \
    src/math/collision/Bound3.hpp \
    src/math/collision/BoundN.hpp \
    src/math/collision/Box2.hpp \
    src/math/collision/Box3.hpp \
    src/math/collision/BoxN.hpp \
    src/math/collision/Circle.hpp \
    src/math/collision/Plane.hpp \
    src/math/collision/Ray.hpp \
    src/math/collision/Sphere.hpp \
    src/math/collision/Triangle.hpp \
    src/math/ColorRGBA.hpp \
    src/math/Matrix4x4.hpp \
    src/math/MatrixNxN.hpp \
    src/math/Quaternion.hpp \
    src/math/Types.hpp \
    src/math/Vector2D.hpp \
    src/math/Vector3D.hpp \
    src/math/Vector4D.hpp \
    src/misc/Algorithms.hpp \
    src/misc/Integer.hpp \
    src/misc/NoCopy.hpp \
    src/misc/Optional.hpp \
    src/misc/ParameterPack.hpp \
    src/misc/Random.hpp \
    src/misc/SortedVector.hpp \
    src/misc/Timer.hpp \
    src/misc/TypeName.hpp \
    src/misc/UniqueID.hpp \
    src/ply/PlyDefinition.hpp \
    src/ply/PlyDocument.hpp \
    src/ply/PlyNumber.hpp \
    src/ply/PlyReader.hpp \
    src/ply/PlyTypes.hpp \
    src/ply/PlyWriter.hpp \
    src/string/print/PrintAlignment.hpp \
    src/string/print/PrintFormat.hpp \
    src/string/print/PrintParameter.hpp \
    src/string/print/PrintPart.hpp \
    src/string/print/PrintSet.hpp \
    src/string/print/PrintUtil.hpp \
    src/string/print/Types.hpp \
    src/string/tokenize/Closed.hpp \
    src/string/tokenize/Open.hpp \
    src/string/tokenize/Tokenizer.hpp \
    src/string/tokenize/Types.hpp \
    src/string/Format.hpp \
    src/string/Hash.hpp \
    src/string/Nest.hpp \
    src/string/Parse.hpp \
    src/string/ParseList.hpp \
    src/string/ParseTuple.hpp \
    src/string/Print.hpp \
    src/string/Segment.hpp \
    src/string/Tokenize.hpp \
    src/string/Trim.hpp \
    src/string/Types.hpp \
    src/All.hpp \
    src/Assertion.hpp \
    src/Cml.hpp \
    src/Collision.hpp \
    src/Exception.hpp \
    src/Func.hpp \
    src/Logger.hpp \
    src/Math.hpp \
    src/Misc.hpp \
    src/Ply.hpp \
    src/String.hpp \
    src/Types.hpp \
    src/math/VectorND.hpp \
    src/math/MathFunctions.hpp \
    src/math/VectorFunctions.hpp \
    src/misc/typetraits/CallTraits.hpp \
    src/misc/typetraits/EnableIf.hpp \
    src/misc/typetraits/HasMemberFunction.hpp \
    src/misc/typetraits/IsArithmetic.hpp \
    src/misc/typetraits/IsPointer.hpp \
    src/misc/typetraits/IsSame.hpp \
    src/misc/typetraits/Types.hpp \
    src/TypeTraits.hpp \
    src/misc/typetraits/HasOstreamOut.hpp \
    src/typetraits/CallTraits.hpp \
    src/typetraits/EnableIf.hpp \
    src/typetraits/HasMemberFunction.hpp \
    src/typetraits/HasOstreamOut.hpp \
    src/typetraits/IsArithmetic.hpp \
    src/typetraits/IsPointer.hpp \
    src/typetraits/IsSame.hpp \
    src/typetraits/Types.hpp \
    src/misc/File.hpp \
    src/misc/StaticBuffer.hpp

SOURCES += \
    src/cml/CmlGroup.cpp \
    src/cml/CmlItem.cpp \
    src/cml/CmlReader.cpp \
    src/math/ColorRGBA.cpp \
    src/misc/Integer.cpp \
    src/misc/Timer.cpp \
    src/ply/PlyDefinition.cpp \
    src/ply/PlyDocument.cpp \
    src/ply/PlyNumber.cpp \
    src/ply/PlyReader.cpp \
    src/ply/PlyWriter.cpp \
    src/string/print/PrintAlignment.cpp \
    src/string/print/PrintFormat.cpp \
    src/string/Format.cpp \
    src/string/Print.cpp \
    src/Assertion.cpp \
    src/Exception.cpp \
    src/Logger.cpp \
    src/TemplateInstantiations.cpp \
