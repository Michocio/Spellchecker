# deklarujemy plik wykonywalny tworzony na podstawie odpowiedniego pliku źródłowego
add_executable (dict-check dict-check.c)

# przy kompilacji programu należy dołączyć bibliotekę
target_link_libraries (dict-check dictionary)
