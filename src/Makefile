

SYM = Symbols
PRS = Parse

# Eingabe:
#	$(SYM).m4      -- Symbole und Namen zur Ersetzung (m4).
#	$(PRS).hpp.m4  -- Parser-Header-Template.
#	$(PRS).l.m4    -- Flex-Template.
#	$(PRS).y       -- Bison-Datei

# Ausgabe:
#	$(PRS).hpp     -- Parser-Header.
#	$(PRS).l       -- Flex-Datei
#	$(PRS).y.cpp   -- Bison-Code
#	$(PRS).y.hpp   -- Bison-Header
#	$(PRS).l.cpp   -- Flex-Code
#	$(PRS).l.hpp   -- Flex-Header


parse: clean m4 bison flex


clean:
	rm -f $(SYM).hpp $(PRS).l $(PRS).y.cpp $(PRS).y.hpp $(PRS).l.cpp $(PRS).l.hpp


m4: $(SYM).m4 $(PRS).hpp.m4 $(PRS).l.m4
	m4 $(SYM).m4 $(PRS).hpp.m4 > $(PRS).hpp
	m4 $(SYM).m4 $(PRS).l.m4   > $(PRS).l


bison: $(PRS).y
	bison -o $(PRS).y.cpp -d $(PRS).y


flex: $(PRS).l
	flex -o $(PRS).l.cpp --header-file=$(PRS).l.hpp $(PRS).l

