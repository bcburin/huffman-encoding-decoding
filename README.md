# huffman-encoding-decoding

Para compilar o codificador e o decodificador usou-se o g++ 9.2.0 com as seguintes linhas de comando:

```md
g++ src/encoder.cpp -o encoder.exe

g++ src/decoder.cpp -o decoder.exe
```

O codificador(encoder.exe) requer dois parêmetros: o nome do arquivo a ser codificado e o nome do arquivo onde serão armazenados os dados binários. Caso esse último não exista, ele será criado. Similarmente, o decodificador aceita como parâmetros o nome do arquivo binário com os dados codificados e o nome do arquivo onde deve ser armazenado o texto decodificado.

Um exemplo de uso usando os arquivos na pasta "data" deste repositório seria

```md
encoder "data/bible.txt" "data/bible.bin"
```

O que codifica o conteúdo do arquivo "bible.txt". Para decodificá-lo, basta fazer

```md
decoder "data/bible.bin" "data/bible_dec.txt"
```

Os executáveis foram testados no git bash para windows e no promt de comando para windows. Realizam-se os testes tanto conforme descrito acima, quanto em diretórios distintos por cópia e cola no sistema, quanto em diretórios distintos após ter transmitido o arquivo binário na internet.

Autor: Bruno Camacho Burin.
