TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        analyzer.cpp \
        dsstring.cpp \
        main.cpp \
        test.cpp \
        tweet.cpp 

HEADERS += \
    analyzer.h \
    analyzer.h \
    catch.hpp \
    dsstring.h \
    dsvector.h \
    tweet.h
