TARGET_DIRECTORY=target
CPPFLAGS=`pkg-config --cflags Qt6WebEngineWidgets Qt6WebEngineCore Qt6Core Qt6Gui`
LDFLAGS=`pkg-config --libs Qt6WebEngineWidgets Qt6WebEngineCore Qt6Core Qt6Gui Qt6Multimedia` -fPIC
CC=g++
