#####################################################################
# Makefile to build libbio3660.so!
#####################################################################

TARGET  := libbio3660.so
CFLAGS  := $(CFLAGS)  -I../../../inc -I../../../inc/linux -I../../../inc/user
LDFLAGS := $(LDFLAGS) -L../../../lib -lbiouserbase
RM 	  := rm -rf

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, obj/%.o, $(SRCS))

$(shell mkdir -p obj)

ARCH := ARM#$(shell uname -m)

# All Target
all : $(TARGET)

-include $(OBJS:.o=.d)

# Tool invocations
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	arm-linux-gnueabihf-g++ -shared -o $@ $(OBJS) $(LDFLAGS)
	-cp $@ ../../../bin
	@echo 'Finished building target: $@'
	@echo ' '

obj/%.o : %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -fPIC -Wall -c $(CFLAGS) -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
#	g++ -O0 -fPIC -g3 -Wall -c $(CFLAGS) -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
# Other Targets
clean:
	-$(RM) $(OBJS) $(TARGET) $(OBJS:.o=.d)
	-$(RM) ./obj
	-@echo ' '
install:
ifeq ($(ARCH),i386)
	install -c -m 644 $(TARGET) /usr/lib/$(TARGET)
else
ifeq ($(ARCH), i686)
	install -c -m 644 $(TARGET) /usr/lib/$(TARGET)
else	
	install -c -m 644 $(TARGET) /usr/lib64/$(TARGET)
	install -c -m 644 $(TARGET) /usr/lib/$(TARGET)
endif
endif

.PHONY: all clean install


