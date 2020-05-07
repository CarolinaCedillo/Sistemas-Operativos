/* eje1v3.c
 * Muestra archivo en hexadecimal
 * Ejecutar: gcc ejemploSO1.c eje1v3.h -lncurses -o ./ejemploSO1
 * Autorees: Rasa González, Jovanny Ulloa, Carolina Cedillo
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

//#define FILEPATH "eje1v3.c"

/*Editar*/
int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}

char * hazLinea(char *base, int dir){
    char linea[100]; //la linea es más pequeña
    int o=0;
    //Muestra 16 caracteres por cada linea
    o += sprintf(linea,"%08x", dir); //offset en hexadecimal
    for(int i=0; i<4; i++){
      unsigned char a,b,c,d;
      a = base[dir+4*i+0];
      b = base[dir+4*i+1];
      c = base[dir+4*i+2];
      d = base[dir+4*i+3];
      o += sprintf(&linea[o]," %02x %02x %02x %02x", a, b, c, d);
    }
    for (int i=0; i<16; i++){
      if(isprint(base[dir+i])){
        o += sprintf(&linea[o],"%c",base[dir+i]);
      }
      else {
        o += sprintf(&linea[o],".");
      }
    }
    sprintf(&linea[o],"\n");

    return(strdup(linea));
}


int abrirArchivo(char parametro[]){
  system(CLEAR);
  initscr();
  raw();
  noecho(); //no mostar el caracter leido

//Abrir archivo
//int fd = open(FILEPATH, O_RDONLY);
printf("%s\n",parametro);
int fd = open (parametro, O_RDWR);
if (fd == -1){
    perror("Error abriendo el archivo");
    exit(EXIT_FAILURE);
}

struct stat st;
fstat(fd,&st);
int fs = st.st_size;

//Mapeo el archivo
char *map = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /*PROT_WRITE*/
if(map == MAP_FAILED){
  close(fd);
  perror("Error mapeando el archivo");
  exit(EXIT_FAILURE);
}

// if(munmap(map,fs) == -1){
//   perror("Error un-mmapping the file");
// }


for(int i=0; i<25; i++){
  //Haz linea, base y offset
  char *l = hazLinea(map,i*16);
  mvprintw(i,0,l);
}
refresh();

int ch; //ch1,ch2,ch3;
int c=0, r=0;
int x=0, y=0;
move(c,9+r);
long b;
//char *p;
 // char *jovas[2];

do{
  ch = leeChar();
  switch (ch) {
    case 0x1b5b41:
    r = (r>0) ? r - 1 : 24;
      break;
    case 0x1b5b42:
    r = (r<24) ? r + 1 : 0;
      break;
    case 0x1b5b43:
    c = (c<31) ? c + 1 : 0;
      break;
    case 0x1b5b44:
    c = (c>0) ? c - 1 : 31;
      break;
    default:
      if (c<16){
        char n = tolower(ch);
        if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')){
          map[r*16+c]=n;
        }
      }
      if(c>16){
        char largo = tolower(ch);
        if (largo >= '0' && largo >= '9' || (largo >= 'a' && largo <= 'f')){
        b = strtol(largo, NULL, 16);
        map[r*16+c]=b;

        // printf(" %c",ch);
        // jovas[0] = tolower(ch);
        //   jovas[1] = '\0';
        //   char largo = tolower(jovas);
        //   if (largo >= '0' && largo <= '9' || (largo >= 'a' && largo <= 'f')){
        //   b = strtol(largo,NULL,16);
        //   map[r*16+c]=b;

        // char largo = tolower(ch);
        // if (largo >= '0' && largo <= '9' || (largo >= 'a' && largo <= 'f')){
        // b = strtol(largo, NULL, 16);
        // map[r*16+c]=b;
      }
    }
      else {
        if (isprint(ch)){
          map[r*16+c-16]=ch;

          char *l = hazLinea(map, r*16);
          mvprintw(r,0,l);
        }
        break;
      }
  }

  y = r;
  x = (c<16) ? c*3+9 : 40+c;

  move(26,5);
  printw("Estoy en %d,%d: Lei %0x    ",r,c,ch);
  move(y,x);

  refresh();
} while (ch != 24);

endwin();
if (munmap(map, fs) == -1){
  perror("Error");
}

close(fd);
return 0;

}




// /* eje1v3.c
//  * Muestra archivo en hexadecimal
//  * Ejecutar: gcc ejemploSO1.c eje1v3.h -lncurses -o ./ejemploSO1
//  * Autorees: Rasa González, Jovanny Ulloa, Carolina Cedillo
// */

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <ctype.h>
// #include <string.h>
// #include <curses.h>
// #include <sys/stat.h>
// #include <sys/mman.h>

// // limpiar pantalla en Windows o linux
// #ifdef _WIN32
// #define CLEAR "cls"
// #else
// #define CLEAR "clear"
// #endif

// //#define FILEPATH "eje1v3.c"

// /*Editar*/
// int leeChar() {
//   int chars[5];
//   int ch,i=0;
//   nodelay(stdscr, TRUE);
//   while((ch = getch()) == ERR); /* Espera activa */
//   ungetch(ch);
//   while((ch = getch()) != ERR) {
//     chars[i++]=ch;
//   }
//   /* convierte a numero con todo lo leido */
//   int res=0;
//   for(int j=0;j<i;j++) {
//     res <<=8;
//     res |= chars[j];
//   }
//   return res;
// }

// char * hazLinea(char *base, int dir){
//     char linea[100]; //la linea es más pequeña
//     int o=0;
//     //Muestra 16 caracteres por cada linea
//     o += sprintf(linea,"%08x", dir); //offset en hexadecimal
//     for(int i=0; i<4; i++){
//       unsigned char a,b,c,d;
//       a = base[dir+4*i+0];
//       b = base[dir+4*i+1];
//       c = base[dir+4*i+2];
//       d = base[dir+4*i+3];
//       o += sprintf(&linea[o]," %02x %02x %02x %02x", a, b, c, d);
//     }
//     for (int i=0; i<16; i++){
//       if(isprint(base[dir+i])){
//         o += sprintf(&linea[o],"%c",base[dir+i]);
//       }
//       else {
//         o += sprintf(&linea[o],".");
//       }
//     }
//     sprintf(&linea[o],"\n");

//     return(strdup(linea));
// }


// int abrirArchivo(char parametro[]){
//   system(CLEAR);
//   initscr();
//   raw();
//   noecho(); //no mostar el caracter leido

// //Abrir archivo
// //int fd = open(FILEPATH, O_RDONLY);
// printf("%s\n",parametro);
// int fd = open (parametro, O_RDWR);
// if (fd == -1){
//     perror("Error abriendo el archivo");
//     exit(EXIT_FAILURE);
// }

// struct stat st;
// fstat(fd,&st);
// int fs = st.st_size;

// //Mapeo el archivo
// char *map = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /*PROT_WRITE*/
// if(map == MAP_FAILED){
//   close(fd);
//   perror("Error mapeando el archivo");
//   exit(EXIT_FAILURE);
// }

// // if(munmap(map,fs) == -1){
// //   perror("Error un-mmapping the file");
// // }


// for(int i=0; i<25; i++){
//   //Haz linea, base y offset
//   char *l = hazLinea(map,i*16);
//   mvprintw(i,0,l);
// }
// refresh();

// int ch; //ch1,ch2,ch3;
// int c=0, r=0;
// int x=0, y=0;
// move(c,9+r);
// long b;
// char jovas[2];

// do{
//   ch = leeChar();
//   switch (ch) {
//     case 0x1b5b41:
//     r = (r>0) ? r - 1 : 24;
//       break;
//     case 0x1b5b42:
//     r = (r<24) ? r + 1 : 0;
//       break;
//     case 0x1b5b43:
//     c = (c<31) ? c + 1 : 0;
//       break;
//     case 0x1b5b44:
//     c = (c>0) ? c - 1 : 31;
//       break;
//     default:
//       if (c<16){
//         char n = tolower(ch);
//         if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')){
//           map[r*16+c]=n;
//         }
//       }
//       if(c>16){
//         jovas[0] = tolower(ch);
//         jovas[1] = '\0';
//         char largo = tolower(jovas);
//         if (largo >= '0' && largo <= '9' || (largo >= 'a' && largo <= 'f')){
//         b = strtol(largo,NULL,16);
//         map[r*16+c]=b;
//       }
//     }
//       else {
//         if (isprint(ch)){
//           map[r*16+c-16]=ch;

//           char *l = hazLinea(map, r*16);
//           mvprintw(r,0,l);
//         }
//         break;
//       }
//   }

//   y = r;
//   x = (c<16) ? c*3+9 : 40+c;

//   move(26,5);
//   printw("Estoy en %d,%d: Lei %0x    ",r,c,ch);
//   move(y,x);

//   refresh();
// } while (ch != 24);

// endwin();
// if (munmap(map, fs) == -1){
//   perror("Error");
// }

// close(fd);
// return 0;

// }








// // /* eje1.c
// //  * Muestra archivo en hexadecimal
// //  * Ejecutar: gcc eje1.c -lncurses -o eje1
// //  * Autorees: Rasa González, maxpowerUlloa, Catolina Cedillo
// //  */
// //
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <fcntl.h>
// // #include <ctype.h>
// // #include <string.h>
// // #include <curses.h>
// // #include <sys/stat.h>
// // #include <sys/mman.h>
// //
// // // limpiar pantalla en Windows o linux
// // #ifdef _WIN32
// // #define CLEAR "cls"
// // #else
// // #define CLEAR "clear"
// // #endif
// //
// // //#define FILEPATH "eje1v3.c"
// //
// // /*Editar*/
// // int leeChar() {
// //   int chars[5];
// //   int ch,i=0;
// //   nodelay(stdscr, TRUE);
// //   while((ch = getch()) == ERR); /* Espera activa */
// //   ungetch(ch);
// //   while((ch = getch()) != ERR) {
// //     chars[i++]=ch;
// //   }
// //   /* convierte a numero con todo lo leido */
// //   int res=0;
// //   for(int j=0;j<i;j++) {
// //     res <<=8;
// //     res |= chars[j];
// //   }
// //   return res;
// // }
// //
// // char * hazLinea(char *base, int dir){
// //     char linea[100]; //la linea es más pequeña
// //     int o=0;
// //     //Muestra 16 caracteres por cada linea
// //     o += sprintf(linea,"%08x", dir); //offset en hexadecimal
// //     for(int i=0; i<4; i++){
// //       unsigned char a,b,c,d;
// //       a = base[dir+4*i+0];
// //       b = base[dir+4*i+1];
// //       c = base[dir+4*i+2];
// //       d = base[dir+4*i+3];
// //       o += sprintf(&linea[o]," %02x %02x %02x %02x", a, b, c, d);
// //     }
// //     for (int i=0; i<16; i++){
// //       if(isprint(base[dir+i])){
// //         o += sprintf(&linea[o],"%c",base[dir+i]);
// //       }
// //       else {
// //         o += sprintf(&linea[o],".");
// //       }
// //     }
// //     sprintf(&linea[o],"\n");
// //
// //     return(strdup(linea));
// // }
// //
// //
// // int abrirArchivo(char parametro[]){
// //   system(CLEAR);
// //   initscr();
// //   raw();
// //   noecho(); //no mostar el caracter leido
// //
// // //Abrir archivo
// // //int fd = open(FILEPATH, O_RDONLY);
// // printf("%s\n",parametro);
// // int fd = open (parametro, O_RDWR);
// // if (fd == -1){
// //     perror("Error abriendo el archivo");
// //     exit(EXIT_FAILURE);
// // }
// //
// // struct stat st;
// // fstat(fd,&st);
// // int fs = st.st_size;
// //
// // //Mapeo el archivo
// // char *map = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /*PROT_WRITE*/
// // if(map == MAP_FAILED){
// //   close(fd);
// //   perror("Error mapeando el archivo");
// //   exit(EXIT_FAILURE);
// // }
// //
// // // if(munmap(map,fs) == -1){
// // //   perror("Error un-mmapping the file");
// // // }
// //
// //
// // for(int i=0; i<25; i++){
// //   //Haz linea, base y offset
// //   char *l = hazLinea(map,i*16);
// //   mvprintw(i,0,l);
// // }
// // refresh();
// //
// // int ch; //ch1,ch2,ch3;
// // int c=0, r=0;
// // int x=0, y=0;
// // move(c,9+r);
// //
// // do{
// //   ch = leeChar();
// //   switch (ch) {
// //     case 0x1b5b41:
// //     r = (r>0) ? r - 1 : 24;
// //       break;
// //     case 0x1b5b42:
// //     r = (r<24) ? r + 1 : 0;
// //       break;
// //     case 0x1b5b43:
// //     c = (c<31) ? c + 1 : 0;
// //       break;
// //     case 0x1b5b44:
// //     c = (c>0) ? c - 1 : 31;
// //       break;
// //     default:
// //       if (c<16){
// //         char n = tolower(ch);
// //         if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')){
// //           map[r*16+c]=n;
// //         }
// //       }
// //       else {
// //         if (isprint(ch)){
// //           map[r*16+c-16]=ch;
// //
// //           char *l = hazLinea(map, r*16);
// //           mvprintw(r,0,l);
// //         }
// //         break;
// //       }
// //   }
// //
// //   y = r;
// //   x = (c<16) ? c*3+9 : 40+c;
// //
// //   move(26,5);
// //   printw("Estoy en %d,%d: Lei %0x",r,c,ch);
// //   move(y,x);
// //
// //   refresh();
// // } while (ch != 24);
// //
// // endwin();
// // if (munmap(map, fs) == -1){
// //   perror("Error");
// // }
// //
// // close(fd);
// // return 0;
// //
// // }
// //



// // /* eje1.c
// //  * Muestra archivo en hexadecimal
// //  * Ejecutar: gcc eje1.c -lncurses -o eje1
// //  * Autorees: Rasa González, maxpowerUlloa, Catolina Cedillo
// //  */
// //
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <fcntl.h>
// // #include <ctype.h>
// // #include <string.h>
// // #include <curses.h>
// // #include <sys/stat.h>
// // #include <sys/mman.h>
// //
// // //#define FILEPATH "eje1v3.c"
// //
// // /*Editar*/
// // int leeChar() {
// //   int chars[5];
// //   int ch,i=0;
// //   nodelay(stdscr, TRUE);
// //   while((ch = getch()) == ERR); /* Espera activa */
// //   ungetch(ch);
// //   while((ch = getch()) != ERR) {
// //     chars[i++]=ch;
// //   }
// //   /* convierte a numero con todo lo leido */
// //   int res=0;
// //   for(int j=0;j<i;j++) {
// //     res <<=8;
// //     res |= chars[j];
// //   }
// //   return res;
// // }
// //
// // char * hazLinea(char *base, int dir){
// //     char linea[100]; //la linea es más pequeña
// //     int o=0;
// //     //Muestra 16 caracteres por cada linea
// //     o += sprintf(linea,"%08x", dir); //offset en hexadecimal
// //     for(int i=0; i<4; i++){
// //       unsigned char a,b,c,d;
// //       a = base[dir+4*i+0];
// //       b = base[dir+4*i+1];
// //       c = base[dir+4*i+2];
// //       d = base[dir+4*i+3];
// //       o += sprintf(&linea[o]," %02x %02x %02x %02x", a, b, c, d);
// //     }
// //     for (int i=0; i<16; i++){
// //       if(isprint(base[dir+i])){
// //         o += sprintf(&linea[o],"%c",base[dir+i]);
// //       }
// //       else {
// //         o += sprintf(&linea[o],".");
// //       }
// //     }
// //     sprintf(&linea[o],"\n");
// //
// //     return(strdup(linea));
// // }
// //
// //
// // int abrirArchivo(char parametro[]){
// //   initscr();
// //   raw();
// //   noecho(); //no mostar el caracter leido
// //
// // //Abrir archivo
// // //int fd = open(FILEPATH, O_RDONLY);
// // int fd = open (parametro, O_RDWR);
// // if (fd == -1){
// //     perror("Error abriendo el archivo");
// //     exit(EXIT_FAILURE);
// // }
// //
// // struct stat st;
// // fstat(fd,&st);
// // int fs = st.st_size;
// //
// // //Mapeo el archivo
// // char *map = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /*PROT_WRITE*/
// // if(map == MAP_FAILED){
// //   close(fd);
// //   perror("Error mapeando el archivo");
// //   exit(EXIT_FAILURE);
// // }
// //
// // // if(munmap(map,fs) == -1){
// // //   perror("Error un-mmapping the file");
// // // }
// //
// //
// // for(int i=0; i<25; i++){
// //   //Haz linea, base y offset
// //   char *l = hazLinea(map,i*16);
// //   mvprintw(i,0,l);
// // }
// // refresh();
// //
// // int ch; //ch1,ch2,ch3;
// // int c=0, r=0;
// // int x=0, y=0;
// // move(c,9+r);
// //
// // do{
// //   ch = leeChar();
// //   switch (ch) {
// //     case 0x1b5b41:
// //     r = (r>0) ? r - 1 : 24;
// //       break;
// //     case 0x1b5b42:
// //     r = (r<24) ? r + 1 : 0;
// //       break;
// //     case 0x1b5b43:
// //     c = (c<31) ? c + 1 : 0;
// //       break;
// //     case 0x1b5b44:
// //     c = (c>0) ? c - 1 : 31;
// //       break;
// //     default:
// //       if (c<16){
// //         char n = tolower(ch);
// //         if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')){
// //           map[r*16+c]=n;
// //         }
// //       }
// //       else {
// //         if (isprint(ch)){
// //           map[r*16+c-16]=ch;
// //
// //           char *l = hazLinea(map, r*16);
// //           mvprintw(r,0,l);
// //         }
// //         break;
// //       }
// //   }
// //
// //   y = r;
// //   x = (c<16) ? c*3+9 : 40+c;
// //
// //   move(26,5);
// //   printw("Estoy en %d,%d: Lei %0x",r,c,ch);
// //   move(y,x);
// //
// //   refresh();
// // } while (ch != 24);
// //
// // endwin();
// // if (munmap(map, fs) == -1){
// //   perror("Error");
// // }
// //
// // close(fd);
// // return 0;
// //
// // }
