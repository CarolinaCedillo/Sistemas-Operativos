#include <stdio.h>
#include <dirent.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>

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
  return i-1;
}


int main() {

  char *lista[] = {};//{"Uno", "Dos", "Tres", "Cuatro" };
   int i= 0;
  int c;
  initscr();
  raw();
  noecho(); /* No muestres el caracter leido */
//   cbreak();
  do {
     for (int j=0; j < 4; j++) {
        if (j == i) {
          attron(A_REVERSE);
        }
        mvprintw(5+j,5,lista[j]);
        attroff(A_REVERSE);
     }
     move(5+i,5);
     refresh();
     c = getch();
     switch(c) {
        case 'A':
           i = (i>0) ? i - 1 : 3;
           break;
        case 'B':
           i = (i<3) ? i + 1 : 0;
           break;
        default:
           // Nothing
           break;
     }
     move(10,5);
     printw("Estoy en %d: Lei %d",i,c);
  } while (c != 'q');




//
	DIR *dir=opendir(".");
	 struct dirent *dp;
	int i=0;
	while((dp=readdir(dir)) != NULL) {
		if(DT_DIR == dp->d_type) {
			printf("D ");
		}
		else {
			printf("F ");
		}
		printf("%s\n",dp->d_name);
}

	closedir(dir);
	return 0;
	}




_________________________________________________________________________




#include <stdio.h>
#include <dirent.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>

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

int main() {
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
