#####################################################################
# Makefile to build libbiouserbase.a!
#####################################################################

TARGET  := libbiouserbase.a
CFLAGS  := $(CFLAGS) -I../../../inc -I../../../inc/linux -I../../../inc/user
#LDFLAGS := $(LDFLAGS)
RM 	  := rm -rf

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, obj/%.o, $(SRCS))

$(shell mkdir -p obj)

# All Target
all : $(TARGET)

-include $(OBJS:.o=.d)

# Tool invocations
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	arm-linux-gnueabihf-ar -r $@ $(OBJS) #$(LDFLAGS)
	-cp $@ ../../../lib
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
	install -c -m 644 $(TARGET) /usr/lib/$(TARGET)

.PHONY: all clean install
