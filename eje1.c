/* eje1.c
 * Muestra archivo en hexadecimal
 * Ejecutar: gcc eje1.c -lncurses -o eje1
 * Autorees: Rosa González, Jovanny Ulloa, Carolina Cedillo
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

#define FILEPATH "eje1.c"

/*Editar*/
// int leeChar(){
//   int chars[5];
//   int ch, i=0;
//   nodelay(stdscr, TRUE);
//   while ((ch = getch()) == ERR);
//   ungetch(ch);
//   while ((ch = getch()) != ERR){
//     chars[i++]=ch;
// }
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
//
//   int res=0;
//   for(int j=0; j<i;j++){
//     res <<=8;
//     res =chars[j];
//   }
//   return res;
// }


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
      o += sprintf(&linea[o],"%02x %02x %02x %02x", a, b, c, d);
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


int main(){
  initscr();
  raw();
  noecho(); //no mostar el caracter leido

//Abrir archivo
int fd = open(FILEPATH, O_RDONLY);
if (fd == -1){
    perror("Error abriendo el arcivo");
    exit(EXIT_FAILURE);
}

//Mapea archivo
struct stat st;
fstat(fd,&st);
int fs = st.st_size;

char *map = mmap(0, fs, PROT_READ, MAP_SHARED, fd, 0);
if(map == MAP_FAILED){
  close(fd);
  perror("Error mapeando el archivo");
  exit(EXIT_FAILURE);
}

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

do{
  ch = leeChar();
  switch (ch) {
    case 0x185841:
    r = (r>0) ? r - 1 : 24;
      break;
    case 0x185842:
    r = (r<24) ? r + 1 : 0;
      break;
    case 0x185843:
    c = (c<31) ? c + 1 : 0;
      break;
    case 0x185844:
    c = (c>0) ? c - 1 : 31;
      break;
    default:      
      if (c<16){
        char n = tolower(ch);
        if (n >= '0' && n <= '9' || (n >= 'a' && n <= 'f')){
          map[r*16+c]=n;
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
  x = (c<16) ? c*3+9 : 41+c;

  move(26,5);
  printw("Estoy en %d,%d: Lei %d",r,c,ch);
  move(y,x);

  refresh();
} while (ch != 24);

endwin();

return 0;

}
