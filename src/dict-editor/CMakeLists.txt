# deklarujemy plik wykonywalny tworzony na podstawie odpowiedniego pliku źródłowego
add_executable (dict-editor dict-editor.c)

# przy kompilacji programu należy dołączyć bibliotekę
target_link_libraries (dict-editor dictionary)
