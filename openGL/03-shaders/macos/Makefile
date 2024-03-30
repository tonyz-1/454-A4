LDFLAGS = -L. -lglfw -ldl # -lpthread
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DMACOS #-pthread

vpath %.cpp ../src
vpath %.c   ../src/glad/src

PROG = shader
OBJS = shader.o gpuProgram.o linalg.o wavefront.o axes.o glad.o 

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
axes.o: ../src/glad/include/KHR/khrplatform.h
gpuProgram.o: ../src/headers.h ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
headers.o: ../src/glad/include/glad/glad.h
headers.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
seq.o: ../src/headers.h ../src/glad/include/glad/glad.h
seq.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/headers.h ../src/glad/include/glad/glad.h
wavefront.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/seq.h ../src/shadeMode.h
axes.o: ../src/headers.h ../src/glad/include/glad/glad.h
axes.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
axes.o: ../src/axes.h ../src/gpuProgram.h
gpuProgram.o: ../src/gpuProgram.h ../src/headers.h
gpuProgram.o: ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
linalg.o: ../src/linalg.h
shader.o: ../src/headers.h ../src/glad/include/glad/glad.h
shader.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
shader.o: ../src/wavefront.h ../src/seq.h ../src/shadeMode.h
shader.o: ../src/gpuProgram.h ../src/axes.h
wavefront.o: ../src/headers.h ../src/glad/include/glad/glad.h
wavefront.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
wavefront.o: ../src/wavefront.h ../src/seq.h ../src/shadeMode.h
