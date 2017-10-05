if (CMOCKA)
enable_testing()
    # dodajemy plik wykonywalny z testem    
    add_executable (trie_test ../dictionary/vector.c ../dictionary/trie.c ../dictionary/alphabet.c trie_test.c)
    set_target_properties(
        trie_test
        PROPERTIES
        COMPILE_DEFINITIONS UNIT_TESTING=1)	

    add_executable (dictionary_tests ../dictionary/vector.c ../dictionary/trie.c ../dictionary/alphabet.c 
	../dictionary/dictionary.c ../dictionary/word_list.c dictionary_tests.c)
    set_target_properties(
       dictionary_tests
        PROPERTIES
        COMPILE_DEFINITIONS UNIT_TESTING=1)

    add_executable (vector_tests ../dictionary/vector.c ../dictionary/trie.c ../dictionary/alphabet.c vector_tests.c)
    set_target_properties(
        vector_tests
        PROPERTIES
        COMPILE_DEFINITIONS UNIT_TESTING=1)	

   add_executable (alphabet_tests ../dictionary/vector.c ../dictionary/trie.c ../dictionary/alphabet.c alphabet_tests.c)
   set_target_properties(
        alphabet_tests
        PROPERTIES
        COMPILE_DEFINITIONS UNIT_TESTING=1)	

    # i linkujemy go z biblioteką do testowania
    target_link_libraries (trie_test ${CMOCKA})
    target_link_libraries (dictionary_tests ${CMOCKA})
    target_link_libraries (vector_tests ${CMOCKA})
    target_link_libraries (alphabet_tests ${CMOCKA})
    # wreszcie deklarujemy, że to test
    add_test (alphabet_tests alphabet_tests)
    add_test (vector_tests  vector_tests)
    add_test (trie_test trie_test)
    add_test (dictionary_tests dictionary_tests)
endif (CMOCKA) 
