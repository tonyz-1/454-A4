# Use this if you do not have 'freetype2' installed.  There will be no text on the screen

#LDFLAGS = -L. -lglfw -lGL -ldl
#CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX

# Use this if you DO have 'freetype2' installed.

LDFLAGS = -L. -lglfw -lGL -ldl -lfreetype
CXXFLAGS = -g -I/usr/include/freetype2 -Wall -Wno-write-strings -Wno-parentheses -DLINUX -DHAVE_FONTS

vpath %.cpp ../src
vpath %.c   ../src/glad/src

PROG = shader
OBJS = shader.o gpuProgram.o linalg.o wavefront.o axes.o font.o glerror.o glverts.o glad.o

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS) 

clean:
	rm -f *.o *~ $(PROG) $(OBJS) Makefile.bak

depend:	
	makedepend -Y ../src/*.h ../src/*.cpp 2> /dev/null
	@echo ""
	@echo "Now edit the Makefile to remove the ../src/ prefix of all of the dependences for the .o files"
	@echo ""

# DO NOT DELETE

axes.o: ../src/linalg.h ../src/gpuProgram.h ../src/headers.h
axes.o: ../src/glad/include/glad/glad.h
axes.o: ../src/glad/include/KHR/khrplatform.h ../src/seq.h
glverts.o: ../src/headers.h ../src/glad/include/glad/glad.h
glverts.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
glverts.o: ../src/seq.h ../src/gpuProgram.h
gpuProgram.o: ../src/headers.h ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/seq.h
headers.o: ../src/glad/include/glad/glad.h
headers.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
seq.o: ../src/headers.h ../src/glad/include/glad/glad.h
seq.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/headers.h ../src/glad/include/glad/glad.h
wavefront.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/seq.h ../src/shadeMode.h ../src/gpuProgram.h
axes.o: ../src/headers.h ../src/glad/include/glad/glad.h
axes.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
axes.o: ../src/axes.h ../src/gpuProgram.h ../src/seq.h
font.o: ../src/headers.h ../src/glad/include/glad/glad.h
font.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
glerror.o: ../src/headers.h ../src/glad/include/glad/glad.h
glerror.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
glerror.o: ../src/glerror.h
glverts.o: ../src/glverts.h ../src/headers.h
glverts.o: ../src/glad/include/glad/glad.h
glverts.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
glverts.o: ../src/seq.h ../src/gpuProgram.h
gpuProgram.o: ../src/gpuProgram.h ../src/headers.h
gpuProgram.o: ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
gpuProgram.o: ../src/seq.h
linalg.o: ../src/linalg.h
shader.o: ../src/headers.h ../src/glad/include/glad/glad.h
shader.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
shader.o: ../src/wavefront.h ../src/seq.h ../src/shadeMode.h
shader.o: ../src/gpuProgram.h ../src/axes.h ../src/font.h
shader.o: ../src/glerror.h ../src/glverts.h
wavefront.o: ../src/headers.h ../src/glad/include/glad/glad.h
wavefront.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/gpuProgram.h ../src/seq.h ../src/wavefront.h
wavefront.o: ../src/shadeMode.h
