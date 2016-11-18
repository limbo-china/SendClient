PROJ = vLogServer
SUBDIR = ./src
EXEC = vLogServer

.PHONY : all dist clean

all :
	make -C $(SUBDIR)

dist :
	cd .. && tar cvzf $(PROJ).tar.gz $(PROJ)

clean :
	$(RM) $(EXEC)
	make clean -C $(SUBDIR)
