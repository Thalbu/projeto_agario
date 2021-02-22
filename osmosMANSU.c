//BIBLIOTECAS

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//VARIÁVEIS GLOBAIS

const float FPS = 100;						
const int SCREEN_W = 640*1.5;
const int SCREEN_H = 540;

// TIPO BOLA

typedef struct Bola 
{
	float x;
	float y;
	float raio;
	float dx;
	float dy;
	ALLEGRO_COLOR cor;
} Bola;

//FUNÇÃO PARA DESENHAR UM CIRCULO PREENCHIDO

void desenhaBola(Bola b) {
	al_draw_filled_circle(b.x, b.y, b.raio, b.cor);
}

// FUNÇÕES PARA INICIALIZAÇÃO DOS ELEMENTOS DO JOGO

void criaheroi(Bola *b)
{
	b->y = SCREEN_H/2; 
	b->x = SCREEN_W/2; 
	b->cor = al_map_rgb(255,255,255); 
	b->raio = 20; 
	b->dx = 0; 
	b->dy = 0; 
}

void criafood(Bola *b)
{
	b->y = SCREEN_H/2; 
	b->x = SCREEN_W/2; 
	b->cor = al_map_rgb(255,255,255); 
	b->raio = 0; 
	b->dx = 0; 
	b->dy = 0; 
}

void criaenemy(Bola *a)
{
	a->y = rand()%SCREEN_H; 
	a->x = rand()%SCREEN_W; 
	a->cor = al_map_rgb(255,0,0);
	a->raio = 20; 
	a->dx = pow((-1),(1+rand()%10))*(0.35+(rand()%10)/100); 
	a->dy = pow((-1),(1+rand()%10))*(0.35+(rand()%10)/100); 
}

//FUNÇÃO PARA CALCULAR A DISTÂNCIA ENTRE DOIS PONTOS

float dist(float x1, float x2, float y1, float y2) 
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

//FUNÇÃO PARA DETECTAR COLISÃO COM LIMITES DA TELA

void colisaoparede(Bola *b)
{
	if(b->x > SCREEN_W || b->x < 0)
	{
		b->dx = -b->dx;
	}	
	if(b->y > SCREEN_H || b->y < 0)
	{
		b->dy = -b->dy;
	}
}

//FUNÇÃO PARA DESLOCAMENTO NO PLANO

void deslocamento(Bola *b)
{
	b->x += b->dx;
	b->y += b->dy;
}

//FUNÇÃO PARA DISPARAR CIRCÚLOS MENORES

void dispara(Bola *a, Bola *b, float x, float y)
{

	b->y = a->y-((a->raio/(dist(x,a->x,y,a->y)))*(a->y-y)); 
	b->x = a->x+((a->raio/(dist(x,a->x,y,a->y)))*(x-a->x)); 
	b->cor = al_map_rgb(255,255,0); 
	b->raio =  sqrt(0.0975)* a->raio; 
	b->dx = (x - a->x)/dist(x, a->x, y, a->y); 
	b->dy = (y - a->y)/dist(x, a->x, y, a->y); 
	a->raio = a->raio * 0.95;
}

//FUNÇÃO QUE GERA FOOD ALEATÓRIO
void criafoodaleatorio(Bola *a, Bola *b)
{
	a->y = rand()%SCREEN_H; 
	a->x = rand()%SCREEN_W; 
	a->cor = al_map_rgb((55+rand()%150),rand()%255,rand()%255); 
	a->raio = 5 + rand()%10; 
	a->dx = pow((-1),(1+rand()%10))*(0.35+(rand()%10)/100);
	a->dy = pow((-1),(1+rand()%10))*(0.35+(rand()%10)/100); 
}

//FUNÇÃO PARA DETECTAR FUSÕES 
void fusao(Bola *a, Bola *b)
{
	if(a->raio > b->raio)
	{
		if(dist(a->x, b->x, a->y, b->y) < a->raio)
		{
			a->raio = sqrt(pow(a->raio,2)+pow(b->raio,2));
			b->raio = 0;
		}
	}
	else if( b->raio > a-> raio)
	{
		if(dist(a->x, b->x, a->y, b->y) < b->raio)
		{
			b->raio = sqrt(pow(a->raio,2)+pow(b->raio,2));
			a->raio = 0;
		}
	}
}


//MAIN
int main() 
{
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *fonte = NULL;
	
	srand((unsigned)time(NULL)); //geração de números aleatórios independentes
	
	//INICIALIZAÇÃO DAS BIBLIOTECAS DO ALLEGRO
	
	if(!al_init()) 
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

    if(!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) 
	{
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if(!timer) 
	{
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}


	event_queue = al_create_event_queue();
    if(!event_queue) 
	{
		fprintf(stderr, "failed to create event_queue!\n");
		return -1;
	}
	
	if(!al_install_mouse())
	{
		fprintf(stderr, "failed to initialize mouse!\n");
	}
   	
	//inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();
	//inicializa o modulo allegro que entende arquivos tff de fontes
    al_init_ttf_addon();

	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);
	
	//registra na fila de eventos que eu quero identificar quando a tela foi alterada
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	// Registra mouse na fila de eventos:
	al_register_event_source(event_queue, al_get_mouse_event_source());

	ALLEGRO_COLOR BKG_COLOR = al_map_rgb(0,0,0);
	//avisa o allegro que agora eu quero modificar as propriedades da tela
	al_set_target_bitmap(al_get_backbuffer(display));
	//colore a tela de preto (rgb(0,0,0))
	al_clear_to_color(BKG_COLOR);
	
	//DECLARAÇÃO DE VARIÁVEIS LOCAIS
	
	int playing = 1, aux1 = 50, aux2 = 0, vitoria = 0;
	float tempo = 0, tempoR;
	char my_text[50], my_text2[20];
	Bola hero, enemy, food[100];
	
	//LEITURA DO TEMPO RECORDE
	
	FILE *recorde = fopen("recorde.txt","a+");
	fscanf(recorde, "%f",&tempoR);
	fclose(recorde);
	
	criaenemy(&enemy);
	criaheroi(&hero);
	for(int i = 0; i < 100; i++)
	{
		criafood(&food[i]);
	}
	
	ALLEGRO_EVENT ev;
  	
	//INICIA O TEMPORIZADOR
	
	al_start_timer(timer);
	
	while(playing) 
	{
		al_wait_for_event(event_queue, &ev);
	  
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
		{
			playing = 0;
		}	  
		else if(ev.type == ALLEGRO_EVENT_TIMER) 
		{
			
			//PONTUAÇÃO: TEMPO
			
			tempo += 0.01;
			
			//DETECÇÃO DE COLISÃO COM A TELA
			
			colisaoparede(&hero);
			colisaoparede(&enemy);
			for(int i = 0; i < 100; i++)
			{
				colisaoparede(&food[i]);
			}
			
			//DESLOCAMENTO 
			deslocamento(&hero);
			deslocamento(&enemy);
			for(int i = 0; i < 100; i++)
			{	
				deslocamento(&food[i]);
			}
			
			//GERAÇÃO ALEATÓRIA
			if((rand()%10 > 5) && (rand()%10 > 5)&& (rand()%10 > 5)&& (rand()%10 > 5))
			{
				criafoodaleatorio(&food[aux2], &hero);
				aux2++;
				if(aux2 == 50)
				{
					aux2 = 0;
				}
			}
			
			//FUSÕES
			
			for(int i = 0; i < 100; i++)
			{
				for(int j = 0; j < 100; j++)
				{
					if(i != j)
					{
						fusao(&food[i],&food[j]);
					}
				}
			}
			for(int i = 0; i < 100; i++)
			{
				fusao(&hero,&food[i]);
			}
			for(int i = 0; i < 100; i++)
			{
				fusao(&enemy,&food[i]);
			}
			fusao(&hero, &enemy);
			
			//VITÓRIA OU DERROTA 
			
			if(hero.raio == 0)
			{
				playing = 0;
			}
			
			if(enemy.raio == 0)
			{
				playing = 0;
				vitoria = 1;
			}

			sprintf(my_text, "Recorde: %.2fs      Tempo atual: %.2fs",tempoR, tempo);
			
			al_clear_to_color(BKG_COLOR); 
			
			al_draw_text(size_32, al_map_rgb(0, 0, 255), 10, 10, 0, my_text);
			
			desenhaBola(hero);
			desenhaBola(enemy);
			for(int i = 0; i < 100; i++)
			{
				desenhaBola(food[i]);
			}
			al_flip_display();
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			hero.dx += -(ev.mouse.x - hero.x)/dist(ev.mouse.x, hero.x, ev.mouse.y, hero.y);
			hero.dy += -(ev.mouse.y - hero.y)/dist(ev.mouse.x, hero.x, ev.mouse.y, hero.y);
			
			dispara(&hero, &food[aux1], ev.mouse.x, ev.mouse.y);
			
			aux1++;
			if(aux1 == 100)
			{
				aux1 = 50;
			}	
		}
	}
	
	if(vitoria == 1)
	{
		if(tempo < tempoR)
		{
			fopen("recorde.txt","w");
			fprintf(recorde ,"%.2f ",tempo);
			fclose(recorde);
			sprintf(my_text2,"NOVO RECORDE: %.2fs", tempo);
			al_clear_to_color(BKG_COLOR);
			al_draw_text(size_32, al_map_rgb(0, 255, 0), 400, 250, 0, "VITÓRIA");
			al_draw_text(size_32, al_map_rgb(0, 255, 0), 300, 200, 0, my_text2);
			al_flip_display();
			al_rest(3.0);
		}
		else
		{
			al_clear_to_color(BKG_COLOR);
			al_draw_text(size_32, al_map_rgb(0, 255, 0), 400, 250, 0, "VITÓRIA");
			al_flip_display();
			al_rest(3.0);
		}
	}
	else
	{
		al_clear_to_color(BKG_COLOR);
		al_draw_text(size_32, al_map_rgb(255, 0, 0), 400, 250, 0, "DERROTA");
		al_flip_display();
		al_rest(3.0);
	}
	al_destroy_font(fonte);
    al_destroy_display(display);

	return 0;
}