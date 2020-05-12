/* eje1v3.c
 * Muestra archivo en hexadecimal
 * Ejecutar: gcc ejemploSO1.c eje1v3.h -lncurses -o ./ejemploSO1
 * Autores: Rosa González, Jovanny Ulloa, Carolina Cedillo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

// limpiar pantalla en Windows o linux
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif
#define FILEPATH "editor.h"

/*Editar*/
int leeChar() {
  int chars[5];
  int ch, i = 0;
  nodelay(stdscr, TRUE);
  while ((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while ((ch = getch()) != ERR) {
    chars[i++] = ch;
  }
  /* convierte a numero con todo lo leido */
  int res = 0;
  for (int j = 0; j < i; j++) {
    res <<= 8;
    res |= chars[j];
  }
  return res;
}

char * hazLinea(char * base, int dir) {
  char linea[100]; //la linea es más pequeña
  int o = 0;
  //Muestra 16 caracteres por cada linea
  o += sprintf(linea, "%08x", dir); //offset en hexadecimal
  for (int i = 0; i < 4; i++) {
    unsigned char a, b, c, d;
    a = base[dir + 4 * i + 0];
    b = base[dir + 4 * i + 1];
    c = base[dir + 4 * i + 2];
    d = base[dir + 4 * i + 3];
    o += sprintf( & linea[o], " %02x %02x %02x %02x", a, b, c, d);
  }
  for (int i = 0; i < 16; i++) {
    if (isprint(base[dir + i])) {
      o += sprintf( & linea[o], "%c", base[dir + i]);
    } else {
      o += sprintf( & linea[o], ".");
    }
  }
  sprintf( & linea[o], "\n");

  return (strdup(linea));
}

/* Función para abrir los archivos con los modos de apertura indicados posteriormente */
int abrirArchivo(char parametro[]) {
  system(CLEAR);
  initscr();
  raw();
  noecho(); //no mostar el caracter leido

  //Abrir archivo
  printf("%s\n", parametro);
  int fdl = open(parametro, O_RDONLY);
  //int fde = open (parametro, O_RDWR);
  int fde = open(parametro, O_RDWR | O_CREAT, 0600); // Abre con permisos -rw-

  if (fdl == -1) {
    perror("Error abriendo el archivo");
    exit(EXIT_FAILURE);
  }
  if (fde == -1) {
    perror("Error abriendo el archivo");
    exit(EXIT_FAILURE);
  }

  struct stat st;
  fstat(fdl, & st);
  int fs = st.st_size;

  /*Mapeo el archivo de entrada*/
  char * mapo = mmap(0, fs, PROT_READ, MAP_SHARED, fdl, 0);
  if (mapo == MAP_FAILED) {
    close(fdl);
    perror("Error mapeando el archivo fdl");
    exit(EXIT_FAILURE);
  }
  /*Mapeo el archivo de salida*/
  const int SLACK = 512;

  char * map = mmap(0, fs + SLACK, PROT_READ | PROT_WRITE, MAP_SHARED, fde, 0);
  if (map == MAP_FAILED) {
    close(fde);
    perror("Error mapeando el archivo fde");
    exit(EXIT_FAILURE);
  }

  /*Copia uno en otro*/
  memcpy(map, mapo, fs);
  //PINTA todo y cuando
  for (int i = 0; i < 25; i++) {
    //Haz linea, base y offset
    char * l = hazLinea(map, i * 16);
    //mvprintw(i,0,l);
    move(i, 0);
    addstr(l);
  }
  refresh();

  int ch, ch2;
  int c = 0, r = 0;
  int x = 0, y = 0;
  move(c, 9 + r);
  long b;
  int repintar = 0;

  do {
    ch = leeChar();
    switch (ch) {
    case 0x1b5b41:
      r = (r > 0) ? r - 1 : 24;
      break;
    case 0x1b5b42:
      r = (r < 24) ? r + 1 : 0;
      break;
    case 0x1b5b43:
      c = (c < 31) ? c + 1 : 0;
      break;
    case 0x1b5b44:
      c = (c > 0) ? c - 1 : 31;
      break;

    // case 0x7f:
    //     printw(" ");
    //     break;

    case 0x1B:
      system(CLEAR);
      printf("Saliendo... ");
      /*Libera el map, fs tan grande como quedo el archivo es el archivo de arriba*/
      if (munmap(map, fs) == -1) {
        perror("Error");
      }
      if (munmap(mapo, fs) == -1) {
        perror("Error");
      }
      close(fde);
      close(fdl);
      return 0;
      // exit (0);
      break;
    default:
      if (c < 16) {
        char largo[3];
        char o = tolower(ch);
        ch2 = leeChar();
        char m = tolower(ch2);
        char * fin = "\0";
        largo[0] = o;
        largo[1] = m;
        largo[2] = * fin;
        /* Validamos los dos caracteres ingresados*/
        int validaChars[2] = {0,0};
        if (o >= '0' && o <= '9' || (o >= 'a' && o <= 'f')){
          validaChars[0] =1;
        }
        if (m >= '0' && m <= '9' || (m >= 'a' && m <= 'f')){
          validaChars[1] =1;
        }
        if(validaChars[0] == 1 && validaChars[1] == 1){
          b = strtol(largo, NULL, 16);
          map[r * 16 + c] = b;
        }
      }
      else {
        // char n = tolower(ch);
        if (isprint(ch)) {
           // if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')) {
             map[r * 16 + c -16] = ch;
           // }
        }
      }
     char * l = hazLinea(map, r * 16);
     //mvprintw(r, 0, l);
     repintar = 1;
     break;
    }

   if(repintar == 1){
    //REPINTA todo
    for (int i = 0; i < 25; i++) {
      //Haz linea, base y offset
      char * l = hazLinea(map, i * 16);
      //mvprintw(i,0,l);
      move(i, 0);
      addstr(l);
    }
    refresh();
   }
    y = r;
    x = (c < 16) ? c * 3 + 9 : 40 + c;

    move(26, 5);
    printw("Estoy en %d,%d: Lei %0x    ", r, c, ch);
    move(y, x);

    refresh();
  } while (ch != 24);

  endwin();
  /*Libera el map, fs tan grande como quedo el archivo es el archivo de arriba*/
  if (munmap(map, fs) == -1) {
    perror("Error");
  }
  if (munmap(mapo, fs) == -1) {
    perror("Error");
  }
  close(fde);
  close(fdl);
  return 0;
}
