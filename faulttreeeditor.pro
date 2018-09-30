QT += widgets
CONFIG += c++11 console

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    graphicsnode.cpp \
    graphicsscene.cpp \
    relation.cpp

HEADERS += \
    mainwindow.h \
    graphicsnode.h \
    graphicsscene.h \
    ellipseitem.h \
    arrow.h \
    relation.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    rec.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/external/libs/ -lfaulttree
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/external/libs/debug/ -lfaulttree
else:unix: LIBS += -L$$PWD/external/libs/ -lfaulttree

INCLUDEPATH += $$PWD/external/include
DEPENDPATH += $$PWD/external/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/external/libs/release/libfaulttree.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/external/libs/debug/libfaulttree.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/external/libs/release/faulttree.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/external/libs/debug/faulttree.lib
else:unix: PRE_TARGETDEPS += $$PWD/external/libs/libfaulttree.a


win32 {
    #copy config files
    config_src_dir = $$PWD\config\*.*
    CONFIG(debug, debug|release) {
        config_dst_dir = $$OUT_PWD\\debug\\config\\
    } else {
        config_dst_dir = $$OUT_PWD\\release\\config\\
    }
    # dst_dir  # Replace slashes in paths with backslashes for Windows
    config_src_dir ~= s,/,\\,g
    config_dst_dir ~= s,/,\\,g
    system(xcopy $$config_src_dir $$config_dst_dir /y /e)

}

DISTFILES += \
    config/config.json
