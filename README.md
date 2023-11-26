# ISOBMFF_parser

<h3> Italiano 🇮🇹</h3>

Il programma è un parser di [MPEG4 Part 12 ISO Base Media File Format](https://en.wikipedia.org/wiki/ISO_base_media_file_format).

La richiesta è quella di mostrare (stampando a schermo) la struttura del file passato al programma (mostrando le "box" che lo compongono e come sono annidate).

Il programma stampa la dimensione e il nome di ogni box o file e il contenuto di MDAT, dando per scontato che sia una stringa XML UTF8 e salvando in dei file le immagini codificate in base64.

Sono partita con l'idea di leggere il file e stampare dimensione e nome di ogni box. Una volta fatto ciò, mi sono accorta che le dimensioni dei file non tornavano, quindi ho dovuto implementare una conversione da big endian a little endian. Successivamente, ho gestito le sub-boxes e letto il contenuto di MDAT, stampandolo.

All'interno di MDAT, ho trovato i metadati delle immagini codificate. Li ho separati dal resto del contenuto, li ho convertiti e salvati in file con i nomi indicati nei metadati stessi.


### COME FAR PARTIRE IL PROGRAMMA
```
make
./narwhal text0.mp4
oppure
./narwhal
```

### TO DO

- [x] Bonus 1
- [x] Bonus 2
- [ ] Compilare su tutti i sistemi operativi
- [ ] Ottimizzare il codice
- [ ] Finire di implementare le mie funzioni
- [ ] Check del formato del file in input
- [ ] Versione in python ?

![Screenshot from 2023-11-26 01-47-36](https://github.com/BvBiancaa/ISOBMFF_parser/assets/111921780/62ca7742-dbd1-400f-9185-2e30a791f714)

Testato su Ubuntu 22.04.

-------------------

<h3> English 🇬🇧</h3>

The program is a parser for the [MPEG4 Part 12 ISO Base Media File Format](https://en.wikipedia.org/wiki/ISO_base_media_file_format).

The request is to display (by printing to the screen) the structure of the file passed to the program, showing the "boxes" that compose it and how they are nested.

The program prints the size and name of each box or file, and the content of MDAT, assuming it is a UTF8 XML string, while saving encoded base64 images to files.

I started by reading the file and printing the size and name of each box. Once that was done, I noticed that the file sizes didn't match, so I had to implement a conversion from big endian to little endian. Later, I handled the sub-boxes and read the content of MDAT, printing it.

Within MDAT, I found the metadata of the encoded images. I separated them from the rest of the content, converted them, and saved them in files with names indicated in the metadata itself.

### HOW TO RUN THE PROGRAM
```
make
./narwhal text0.mp4
or
./narwhal
```

### TO DO

- [x] Bonus 1
- [x] Bonus 2
- [ ] Compile on all operating systems
- [ ] Optimize the code
- [ ] Finish implementing my functions
- [ ] Check the input file format
- [ ] Python version?


![Screenshot from 2023-11-26 01-47-36](https://github.com/BvBiancaa/ISOBMFF_parser/assets/111921780/0818de88-7d42-4793-910d-a8fb328b3c03)

Tested on Ubuntu 22.04.
