CPP=g++

#CPP_FILES= testcv.cpp
#CPP_FILES= hoge.cpp
#CPP_FILES= image.cpp
CPP_FILES= grayimage.cpp
#CPP_FILES= testgl.cpp
#CPP_FILES= main.cpp

CFLAG=`pkg-config --cflags opencv`
LDFLAG=`pkg-config --libs opencv` -lGL -lGLU -lglut

all : $(CPP_FILES)
	$(CPP) -o run $(CPP_FILES) $(CFLAG) $(LDFLAG)
