include config.mk

all: generate_lib
	gcc $(PATH_EXAMPLES)/example.c $(CFLAGS_EXAMPLES) -o example.$(EXTENSION)
	gcc $(PATH_EXAMPLES)/code.c $(CFLAGS_EXAMPLES) -o code.$(EXTENSION)

all_debug: generate_lib_debug
	gcc $(PATH_EXAMPLES)/example.c $(CFLAGS_EXAMPLES_DEBUG) -o example.$(EXTENSION)
	gcc $(PATH_EXAMPLES)/code.c $(CFLAGS_EXAMPLES_DEBUG) -o code.$(EXTENSION)

generate_lib: libdebug_c.a $(TARGET).a
	ar -t $(TARGET).a

generate_lib_debug: libdebug_c.a $(TARGET_DEBUG).a
	ar -t $(TARGET_DEBUG).a

libdebug_c.a:
	echo "generando librerias estatica... $@"
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME)

$(TARGET_DEBUG).a: $(OBJECTS_DEBUG)
	echo "generando librerias estatica... $@"
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@
	$(CP) $(TARGET_DEBUG).a $(TARGET).a

$(TARGET).a: $(OBJECTS)
	echo "generando librerias estatica... $@"
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

ecs.o: $(PATH_SRC)/ecs.c
	$(CC) $(CFLAGS) -c $^ -o $@

ecs_debug.o: $(PATH_SRC)/ecs.c
	$(CC) $(CFLAGS_DEBUG) -c $^ -o $@


cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS) $(OBJECTS_DEBUG)
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) $(TARGET).a \
	$(TARGET_DEBUG).a *.i *.s
	$(MAKE) -C . -f $(MAKE_NAME) cleanobj
	$(RM) $(RMFLAGS) example.$(EXTENSION)
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall