RM := rm -rf


OBJS += \
./parse_soy_data.o


# All Target
all: ParseSoyData

# Tool invocations
ParseSoyData: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc `xml2-config --libs` -o "ParseSoyData" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

%.o: %.c
	@echo 'Building file: $<'
	gcc -Wall -c `xml2-config --cflags` -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) ParseSoyData
	-@echo ' '

.PHONY: all clean

