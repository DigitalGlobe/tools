$(DIRS):
	$(MAKE) -C $@


%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
          sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
          rm -f $@.$$$$

-include $(DEP_FILES)


clean::
	for dir in $(DIRS); do $(MAKE) -C $$dir clean; done
	$(RM) $(OBJ) $(TARGET) # $(DEP_FILES)


.PHONY: all $(DIRS) clean
