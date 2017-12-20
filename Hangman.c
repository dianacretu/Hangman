#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define WIDTH 30
#define HEIGHT 10

char *menu[3] = {
					"New Game   ",
					"Resume Game",
					"Quit       ",
				};

char *show_time() //functie pentru a arata timpul
{
	time_t timp;
    time(&timp);
    struct tm *tms;
    tms = localtime(&timp);
    return asctime(tms);

}				

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;
	x = 23;
	y = 5;
	box(menu_win, 0, 0);
	for(i=0; i < 3; i++)
	{
		if(highlight == i+1)
		{
			wattron(menu_win, A_STANDOUT);
			mvwprintw(menu_win, y, x, "%s", menu[i]);
			wattroff(menu_win, A_STANDOUT);
		}
		else
			mvwprintw(menu_win, y, x, "%s", menu[i]);
		y++;
	}
	wrefresh(menu_win);
}

void draw_hanging(WINDOW *game_win)
{
	int i, h;
	for(i = 5; i < 19; i++){
		mvwaddch(game_win, i, 5, '|');
		if (i == 18) 
		{
			mvwaddch(game_win, i, 4, '_');
			mvwaddch(game_win, i, 6, '_');
		}
	}
	for(h = 5; h < 17; h++){
		mvwaddch(game_win, 5, h, '_');
		if (h == 16)
		{
			mvwaddch(game_win, 6, h, '|');
			mvwaddch(game_win, 7, h, '|');
		}
	}
	wrefresh(game_win);
}


char *codificare(char v[])
{
	char *i=v;
	char copy[1000];
	strcpy(copy, v);
	int spatii = 0;
	int flag[24];
	int z;
	for (z=0;z<24;z++)
		flag[z]=0;
	while (*i!=0)
	{
	//	strcat(copy, i);
		if ( *i == ' ') spatii ++;
		i++;
	}
	char *a;
	a = strtok(copy, " ");
	char *new;
	new = calloc (strlen(a), sizeof(char));
	//printf("%s\n",copy);
  	while(a!=NULL)
  	{  	
  		int old_size=strlen(new);
  		new = realloc (new, strlen(a)*sizeof(char));
   		if(strlen(a)==1)
   		{
      			strcat(new, "_");
    	}
    	else if(strlen(a)==2)
    	{
      		new[old_size]=a[0];
      		strcat(new, "_");
    	}
    	else
    	{
      		new[old_size]=a[0];
      		int i;
      		for (i=0;i<strlen(a)-2;i++){
        	strcat(new, "_");
      	}
      		new[strlen(new)]=a[strlen(a)-1];
    	}
    	if (spatii > 0)
    	{
    		strcat( new," ");
    		spatii --;
   	 	}
    
    	a=strtok(NULL," ");
  		}
  		char *m=new;
  		char *j=copy;
		while(*m != 0)
  		{
  			if (flag[*j-'a'] == 1 ){
  					*m=*j;
  			}
  			if (*m != '_' && *m != ' ')
  			{
  				flag[*m -'a']=1;
  			}
  			m++;
  			j++;
  		}

  		return new;
		}	


void panel_info(WINDOW* game_win, int index, int caractere)
{
    mvwprintw(game_win, 1, 30, "%s", show_time());
	mvwprintw(game_win, 2, 30, "Instructiuni:");
    mvwprintw(game_win, 3, 30, "Nr de expresii este %d, iar nr de caractere %d", index, caractere);
}


int main(int argc, char* argv[])
{
	WINDOW *menu_win, *game_win;
	FILE *fisier;
	int highlight = 1;
	int i;
	char *v;
	v = (char*) malloc(200 * sizeof(char));
	char **input = (char**) malloc(1000 * sizeof(char*));
	int index = 0;
	int j;
	int c;
    int resume_ok = 0;
	int caractere = 0;
	if ( argc == 1) 
	{
		fprintf(stderr, "Nu s-au dat argumente de comanda.\n");
		return 1;
	}
	else
	{
		for (i = 1; i < argc; i++) //pentru toate fisierele
		{
			fisier = fopen(argv[i], "r");
			if (fisier == NULL)
			{
				fprintf(stderr, "Fisierul %s nu poate fi deschis.\n", argv[i]);
				return 1;
			}
			else
			{
				while ( fgets ( v, 128, fisier) != NULL ) //cat timp citeste din fisier
				{
					j = 0;
					input[index] = (char*) calloc(strlen(v) , sizeof(char ));
					for(int k=0; k< strlen(v); k++) 
					{
						if (isprint(v[k]))
						{
							if (isalpha(v[k]))
							{
								input[index][j] = tolower(v[k]);
								j++;
								caractere ++;
							}
							else
							{
								input[index][j] = v[k];
								j++;
								caractere ++;
							}
						}
					}
					if (j>0)
					{
						index++; //numar expresii
					}
				}
			}
			fclose(fisier);
		}
	}

	initscr();
	clear();
	noecho();
	cbreak();
	curs_set(0);
    menu_win = newwin(15, 60, 13, 17);
	keypad(menu_win, TRUE);
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{
		print_menu(menu_win, highlight);
		c = wgetch(menu_win);
		if (highlight == 3 && c == 10) //pentru Quit
            break;
		switch(c)
		{
			case KEY_UP:
				if (highlight == 1)
					{ 
						highlight = 3;
					}
				else 
					{
						highlight--;
					}
				break;
			case KEY_DOWN:
				if (highlight == 3)
				{
					highlight = 1;
				}
				else 
				{
					highlight++;
				}
				break;
		}
        
		if (highlight == 1 && c == 10) // pentru New Game
		{
                resume_ok = 1;
				game_win = newwin(0, 0, 0, 0);
		        box(game_win, 0, 0);
				draw_hanging(game_win);
				wrefresh(game_win);
                panel_info(game_win, index, caractere);
                
         //       ceva:
				while(1)			
				{
				    c = wgetch(game_win);
				    if (c == 'Q') 
				    {
					FILE *wfile = fopen("Resume.txt", "w");

					putwin(game_win, wfile);

					fclose(wfile);					

                        	   	wclear(game_win);
                        		wrefresh(game_win);
                            		
					break;
				    }
				    else
				    {
					panel_info(game_win, index, caractere);
				    }
				}             
		}

		if (highlight == 2 && c == 10 && resume_ok == 1) //pentru Resume Game
		{
			FILE *rfile = fopen("Resume.txt", "r");
			
			game_win = getwin(rfile);
			fclose(rfile);

                	resume_ok = 1;
                        box(game_win, 0, 0);
                        draw_hanging(game_win);
                                wrefresh(game_win);
                panel_info(game_win, index, caractere);

         //       ceva:
                                while(1)
                                {
                                    c = wgetch(game_win);
                                    if (c == 'Q')
                                    {
                                        FILE *wfile = fopen("Resume.txt", "w");

                                        putwin(game_win, wfile);

                                        fclose(wfile);

                                        wclear(game_win);
                                        wrefresh(game_win);

                                        break;
                                    }
                                    else
                                    {
                                        panel_info(game_win, index, caractere);
                                    }
                                }
	
		}
	}
	clrtoeol();
	refresh();
	endwin();
	return 0;
}
