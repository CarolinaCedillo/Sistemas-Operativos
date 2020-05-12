/* dir.c
 * Muestra archivo en hexadecimal
 * Ejecutar: gcc ejemploSO1.c eje1v3.h -lncurses -o ./ejemploSO1
 * Autorees: Rasa González, Jovanny Ulloa, Carolina Cedillo
 */

#include <stdio.h>
#include <dirent.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include "editor.h"

int abrirArchivo(char parametro[]);

struct s_dir{
  int tipo;
  char *nombre;
} res[128];

//
int leeDir(char *direc){
  DIR *dir = opendir(direc);

  struct dirent *dp;
   int i=0;
  while((dp=readdir(dir)) != NULL){
    res[i].tipo = dp->d_type;
    res[i].nombre = strdup(dp->d_name);
    i++;
  }
  closedir(dir);
  return i;//-1;
}

int main(){
  
  char cwd[256];
  getcwd(cwd,256);
  int max = leeDir(cwd);
  int i=0;
  int c;
  int sigue=1;

  initscr();
  raw();
  noecho();

do{
  for (int j=0; j<max; j++){
    if (j == i){
      attron(A_REVERSE);
    }
    char *nom = res[j].nombre;
    mvprintw(5+j,4,nom);
    attroff(A_REVERSE);
  }
  move(5+i,4);
  refresh();
  c = getch();
  switch (c) {
    case 'A':
      i = (i>0) ? i - 1 : max-1;
      break;
    case 'B':
      i = (i<(max-1)) ? i + 1 : 0;
      break;
    case 10:
      if (strcmp(".",res[i].nombre) == 0 || res[i].tipo == DT_REG){
         // si es archivo salte
         if (res[i].tipo == DT_REG) {
           sigue=0;
           abrirArchivo(res[i].nombre);
         }
       } else {
         if(strcmp("..",res[i].nombre) == 0){
           //cambia al padre
           char *p = strrchr(cwd,'/');
           *p = 0;
           max = leeDir(cwd);
           i=0;
           clear();
         } else {
           //Cambia al otro directorio
           strncat(cwd,"/",256);
           strncat(cwd,res[i].nombre,256);
           max = leeDir(cwd);
           i=0;
           clear();
          }
        }
        break;
        default:
        //
        break;
      }
      move(2,10);
      printw("Estoy en %d: Lei: %d  Num:%d Dir:%s",i,c,max,cwd);
      //move(y,x);
      refresh();
 } while (c != 24);
 endwin();
 return 0;
}
