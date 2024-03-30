LDFLAGS = -L. -lglfw -lGL -ldl # -lpthread
CXXFLAGS = -g -Wall -Wno-write-strings -Wno-parentheses -DLINUX #-pthread

vpath %.cpp ../src
vpath %.c   ../src/glad/src

EXEC = demo1 demo2 demo3 demo4 demo5 demo6 demo7 demo8 demo9
OBJS = gpuProgram.o linalg.o glad.o

all:    $(EXEC) 

demo1:	demo1.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo2:	demo2.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo3:	demo3.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo4:	demo4.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo5:	demo5.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo6:	demo6.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo7:	demo7.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo8:	demo8.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

demo9:	demo9.o $(OBJS)
	$(CXX) -o $@ $@.o $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(EXEC) $(OBJS) Makefile.bak

depend:	
	makedepend -Y ../src/*.h ../src/*.cpp 2> /dev/null
	@echo ""
	@echo "Now edit the Makefile to remove the ../src/ prefix of all of the dependences for the .o files"
	@echo ""

# DO NOT DELETE

gpuProgram.o: ../src/headers.h ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
headers.o: ../src/glad/include/glad/glad.h
headers.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo1.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo1.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo2.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo2.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo2.o: ../src/gpuProgram.h
demo3.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo3.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo3.o: ../src/gpuProgram.h
demo4.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo4.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo4.o: ../src/gpuProgram.h
demo5.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo5.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo5.o: ../src/gpuProgram.h
demo6.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo6.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo6.o: ../src/gpuProgram.h
demo7.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo7.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo7.o: ../src/gpuProgram.h
demo8.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo8.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo8.o: ../src/gpuProgram.h
demo9.o: ../src/headers.h ../src/glad/include/glad/glad.h
demo9.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
demo9.o: ../src/gpuProgram.h
gpuProgram.o: ../src/gpuProgram.h ../src/headers.h
gpuProgram.o: ../src/glad/include/glad/glad.h
gpuProgram.o: ../src/glad/include/KHR/khrplatform.h ../src/linalg.h
linalg.o: ../src/linalg.h
