#include <stdio.h>
#include <stdlib.h>
#include <math.h>                          //funções matemáticas
#include <time.h>                          //tempo
#include <allegro5/allegro.h>              //allegro 5
#include <allegro5/allegro_font.h>         //fonte
#include <allegro5/allegro_ttf.h>          //parte da fonte
#include <allegro5/allegro_primitives.h>   //formas primitivas
#include <allegro5/allegro_native_dialog.h>//para dialogos
#include <allegro5/allegro_audio.h>        //tocar o audio
#include <allegro5/allegro_acodec.h>      //disponiza extensões

//-------------------------CONSTANTES DO JOGO-------------------------
const float FPS = 60;                      //quantidade de vezes que a tela vai ser redesenhada por segundo
const int largura = 1000;                  //largura da tela
const int altura = 680;                    //altura da tela
const int letras_x=150;                    //posição do tempo no X
const int letras_x2=850;                   //posição do recorde no X
const int letras_y=5;                      //posição do tempo e recorde no Y
const int letras_y2=200;                   //posição das letras final no Y
const int letras_y3=310;                   //posição das letras final no Y
const int letras_y4=400;                   //posição das letras final no Y

//------------------------ESTRUTURA DOS CIRCULOS------------------------
typedef struct Circulo {
	float x;
	float y;
	float raio;
	float vx;
	float vy;
	ALLEGRO_COLOR RGB;
} Circulo;

//--------------------------FUNÇÕES DE TELA--------------------------

//Calcula distancia entre 2 pontos
float dist(float x1, float x2, float y1, float y2) {
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

//compara a colisao com a tela 
void colisao_tela(Circulo *qualquer){
	if((qualquer->x>largura) || (qualquer->x<0)){
		qualquer->vx = -qualquer->vx;
	}else{
		if((qualquer->y>altura) || qualquer->y<0){
			qualquer->vy = -qualquer->vy;
		}
	}
}

//Desenha a bola
void desenhaBola(Circulo b){
	al_draw_filled_circle(b.x, b.y, b.raio, b.RGB);
}

//faz com que as bolas se movimentem
void andando(Circulo *qualquer){
	qualquer->x += qualquer->vx;
	qualquer->y += qualquer->vy;
}

//-----------------------FUNÇÕES DE CRIAR CIRCULOS-----------------------

void criaCirculo(Circulo *b) {
	b->y = altura/2;
	b->x = largura/2;
	b->RGB = al_map_rgb(255,0,0);
	b->raio = 20;
	b->vx = 0;
	b->vy = 0;
}

void cria_reverso(Circulo *b) {
	b->y = rand()%(altura-30)+15;
	b->x = rand()%(largura-30)+15;
	b->RGB = al_map_rgb(218,165,38);
	b->raio = 21;
	b->vx = (0.35+(rand()%10)/100)*pow(-1,(rand()%10));
	b->vy = (0.35+(rand()%10)/100)*pow(-1,(rand()%10));
}

void criaComida(Circulo *b){
	b->y = rand()%altura;
	b->x = rand()%largura;
	b->RGB = al_map_rgb(50,rand()%255,rand()%255);
	b->raio = 1+(rand()%13);
	b->vx = (0.35+(rand()%10)/100)*pow(-1,(rand()%10));
	b->vy = (0.35+(rand()%10)/100)*pow(-1,(rand()%10));
}

void cria_expelida(Circulo *expelida, Circulo *flash, float x_mouse, float y_mouse){
	
	expelida->y = flash->y - ((flash->raio/(dist(x_mouse,flash->x,y_mouse,flash->y)))*(flash->y-y_mouse)); 
	expelida->x = flash->x + ((flash->raio/(dist(x_mouse,flash->x,y_mouse,flash->y)))*(x_mouse-flash->x)); 
	expelida->RGB = al_map_rgb(50,rand()%255,rand()%255); 
	expelida->raio =  0.19* flash->raio;
	flash->raio = flash->raio * 0.9;
	if((x_mouse < (flash->x-5))&&(y_mouse < (flash->y-5))){//2
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}
	}else if((x_mouse > (flash->x+5))&&(y_mouse < (flash->y-5))){//1
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}
	}else if((x_mouse > (flash->x+5))&&(y_mouse > (flash->y+5))){//4
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}
	}else if((x_mouse < (flash->x+5))&&(y_mouse > (flash->y+5))){//3
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = flash->vx;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = -flash->vx;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = flash->vy;
		}
	}else if((x_mouse > (flash->x))&&(y_mouse > (flash->y+5))&&(y_mouse > (flash->y-5))){//frente
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}
	}else if((x_mouse < (flash->x))&&(y_mouse > (flash->y+5))&&(y_mouse > (flash->y-5))){//tras
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = flash->vx;
			expelida->vy = 0;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = -flash->vx;
			expelida->vy = 0;
		}
	}else if((x_mouse > (flash->x-5))&&(x_mouse < (flash->x+5))&&(y_mouse < (flash->y))){//cima
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}
	}else if((x_mouse > (flash->x-5))&&(x_mouse < (flash->x+5))&&(y_mouse > (flash->y))){//embaixo
		if((flash->vx>0)&&((flash->vy>0))){//direita para baixo
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}else if((flash->vx<0)&&((flash->vy>0))){ //esquerda para baixo
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}else if((flash->vx>0)&&((flash->vy<0))){ //direita para cima
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy<0))){ //esquerda para cima
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx>0)&&((flash->vy==0))){ //direita 
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx<0)&&((flash->vy==0))){ //esquerda
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy<0))){ //cima
			expelida->vx = 0;
			expelida->vy = -flash->vy;
		}else if((flash->vx==0)&&((flash->vy>0))){ //baixo
			expelida->vx = 0;
			expelida->vy = flash->vy;
		}
	}
}

//-----------------------------FUNÇÕES DE COLISÃO-----------------------------

void colisao_comida(Circulo *comidaA, Circulo *comidaB){
	if(comidaA->raio > comidaB->raio){
		if(comidaA->raio >= dist(comidaA->x, comidaB->x, comidaA->y, comidaB->y)){
				
				comidaA->raio= sqrt((pow(comidaA->raio,2) + pow(comidaB->raio,2)));
				criaComida(comidaB);
		}
	}else{
		if(comidaA->raio >= dist(comidaB->x, comidaA->x, comidaB->y, comidaA->y)){
				
				comidaB->raio= sqrt((pow(comidaA->raio,2)+pow(comidaB->raio,2)));
				criaComida(comidaA);
		}
	}
}

void colisao_flash(Circulo *flash,Circulo *comidaA, bool *fim){
	
	if( flash->raio > comidaA->raio){
		if(flash->raio >= dist(flash->x, comidaA->x, flash->y, comidaA->y)) {
				
			flash->raio = sqrt((pow(comidaA->raio,2)+pow(flash->raio,2)));
			criaComida(comidaA);
		}
	}else{
		if(comidaA->raio >= dist(comidaA->x, flash->x, comidaA->y, flash->y)){
				
			comidaA->raio= sqrt((pow(comidaA->raio,2)+pow(flash->raio,2)));
			*fim=true;
		}
	}
}

void colisao_reverso(Circulo *reverso, Circulo *comidaA ){

	if( reverso->raio > comidaA->raio){
		if(reverso->raio >= dist(reverso->x, comidaA->x, reverso->y, comidaA->y)) {
				
			reverso->raio = sqrt((pow(comidaA->raio,2)+pow(reverso->raio,2)));
			criaComida(comidaA);
		}
	}
}

void flashXreverso(Circulo *flash,Circulo *reverso,int *ganhou,bool *fim){
	
	if( flash->raio > reverso->raio){
		if(flash->raio >= dist(flash->x, reverso->x, flash->y, reverso->y)) {
			*ganhou += 1;
			*fim = true;
		}
	}else{
		if(reverso->raio >= dist(reverso->x, flash->x, reverso->y, flash->y)){
			*fim = true;
		}
	}
}

void expelida_comida(Circulo *expelida, Circulo *comidaA){
	if(comidaA->raio > expelida->raio){
		if(comidaA->raio >= dist(comidaA->x, expelida->x, comidaA->y, expelida->y)){
				
				comidaA->raio= sqrt((pow(comidaA->raio,2) + pow(expelida->raio,2)));
				criaComida(expelida);
		}
	}else{
		if(comidaA->raio >= dist(expelida->x, comidaA->x, expelida->y, comidaA->y)){
				
				expelida->raio= sqrt((pow(comidaA->raio,2)+pow(expelida->raio,2)));
				criaComida(comidaA);
		}
	}
}

void expelida_flash(Circulo *flash, Circulo *expelida, bool *fim){
	
	if( flash->raio > expelida->raio){
		if(flash->raio >= dist(flash->x, expelida->x, flash->y, expelida->y)) {
				
			flash->raio = sqrt((pow(expelida->raio,2)+pow(flash->raio,2)));
			criaComida(expelida);
		}
	}else{
		if(expelida->raio >= dist(expelida->x, flash->x, expelida->y, flash->y)){
				
			expelida->raio= sqrt((pow(expelida->raio,2)+pow(flash->raio,2)));
			*fim=true;
		}
	}
}

void expelida_reverso(Circulo *reverso, Circulo *expelida){

	if(reverso->raio > expelida->raio){
		if(reverso->raio >= dist(reverso->x, expelida->x, reverso->y, expelida->y)) {
				
			reverso->raio = sqrt((pow(expelida->raio,2)+pow(reverso->raio,2)));
			criaComida(expelida);
		}
	}
}

int main() {
	
	ALLEGRO_SAMPLE 	*trilha_sonora = NULL;
	ALLEGRO_SAMPLE 	*vaias = NULL;
	ALLEGRO_SAMPLE 	*aplausos = NULL;
	ALLEGRO_SAMPLE 	*aplausos_gritos = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_trilha_sonora = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_vaias = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_aplausos = NULL;
	ALLEGRO_SAMPLE_INSTANCE *inst_aplausos_gritos = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;	
	int i,j,ganhou=0, n= (-1);;
	float tempo=0,recorde;
	FILE *record = fopen("record.txt", "r");
	fscanf(record, "%f", &recorde);
	//gera um numero aleatorio de acordo com a mudanca do relogio, numero sempre positivo
	srand((unsigned)time(NULL));
	bool tocar_aplausos = false;
	bool fim = false;
	if(!al_init()) {
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao inicializar o Allegro", NULL , ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
   
    if(!al_init_primitives_addon()){
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao criar as formas primitivas", NULL , ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }   
   
	display = al_create_display(largura, altura);
	if(!display) {
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao criar o display", NULL , ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	//cria o temporizador, tendo 60 quadros por segundo
	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao criar o temporizador", NULL , ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
 
   
	event_queue = al_create_event_queue();
	if(!event_queue) {
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao criar a fila de eventos", NULL , ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
   
   al_install_mouse();
	if(!al_install_mouse()){
		al_show_native_message_box(NULL , "Aviso!" , "Error" , "Falha ao inicializar o mouse", NULL , ALLEGRO_MESSAGEBOX_ERROR);
	}
	
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();//instalar audio
	al_init_acodec_addon();//iniciar a disponibilidade das extensões
	al_reserve_samples(10);//canais de som
   	//registra na fila de eventos que eu quero identificar quando a tela foi alterada
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra o tempo na fila de eventos
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	// Registra mouse na fila de eventos:
	al_register_event_source(event_queue, al_get_mouse_event_source());
	// registra o teclado na fila de eventos
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//cria a fonte
	ALLEGRO_FONT *fonte_32 = al_load_font("arial.ttf", 32, 1);
	ALLEGRO_FONT *fonte_40 = al_load_font("arial.ttf", 40, 1);
	ALLEGRO_FONT *fonte_20 = al_load_font("arial.ttf", 20, 1);
	ALLEGRO_COLOR BKG_COLOR = al_map_rgb(0,0,0);
	al_draw_textf(fonte_40,al_map_rgb(255,255,255),largura/2,altura/2,1,"Carregando... Por favor,aguarde");
	al_flip_display();//desenha na tela 
	
	trilha_sonora = al_load_sample("flash.ogg");//carrega o som
	aplausos = al_load_sample("aplausos.ogg");
	aplausos_gritos = al_load_sample("aplausos_gritos.ogg");
	vaias = al_load_sample("perdeu.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);//palsa o som
	inst_aplausos = al_create_sample_instance(aplausos);
	inst_aplausos_gritos = al_create_sample_instance(aplausos_gritos);
	inst_vaias = al_create_sample_instance(vaias);
	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());//misar o som
	al_attach_sample_instance_to_mixer(inst_aplausos, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_aplausos_gritos, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_vaias, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_aplausos,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_aplausos_gritos,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_vaias,ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora,0.8);//volume do som
	al_set_sample_instance_gain(inst_aplausos,0.8);
	al_set_sample_instance_gain(inst_aplausos_gritos,0.8);
	al_set_sample_instance_gain(inst_vaias,0.8);
	//avisa o allegro que agora eu quero modificar as propriedades da tela
	al_set_target_bitmap(al_get_backbuffer(display));
	//colore a tela de preto(rgb(0,0,0))
	al_clear_to_color(BKG_COLOR);   
	al_play_sample_instance(inst_trilha_sonora);
	al_rest(2.5);
	Circulo flash;
	criaCirculo(&flash);
	Circulo reverso;
	cria_reverso(&reverso);
   
	Circulo comida[50];
	Circulo expelida[51000];
	for(int i = 0; i < 50; i++){
		criaComida(&comida[i]);
	}
	ALLEGRO_EVENT ev;//Declara fila de eventos
	al_start_timer(timer);//inicia o temporizador
	
	//---------------------------LOOP PRINCIPAL---------------------------
	while(!fim){
		/*so permite que o while seja execultado se algum evento acontecer
		caso contrario ficara ocioso*/
		
		al_wait_for_event(event_queue, &ev);
		
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN){//Apertar Esc o jogo fecha
			if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				fim = true;
			}
		}else{
			if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {//Clicar no X da janela o jogo fecha
			fim = true;
			}else{
				if(ev.type == ALLEGRO_EVENT_TIMER) {//Evento de tempo
	  
					colisao_tela(&flash);
					colisao_tela(&reverso);
					for(i = 0; i < 50; i++){
						colisao_tela(&comida[i]);
					}
					andando(&flash);
					andando(&reverso);
					for(i = 0; i < 50; i++){
						andando(&comida[i]);
					}
					
					al_clear_to_color(BKG_COLOR);//Limpa tela
					desenhaBola(flash);
					desenhaBola(reverso);
					for(i = 0; i < 50; i++){
						desenhaBola(comida[i]);
					}
					for(i=0; i<50 ; i++){
						for(j=0; j<50 ; j++){
							if(i!=j){
								colisao_comida(&comida[i], &comida[j]);
							}
						}
					}
					for(i=0; i<50 ; i++){	
						colisao_flash(&flash, &comida[i], &fim);
					}
					for(i=0; i<50 ; i++){
						colisao_reverso(&reverso,&comida[i]);
					}
					if(n>=0){
						for(i=0;(i<=n) && (n<50);i++){
							colisao_tela(&expelida[i]);
							andando(&expelida[i]);
							desenhaBola(expelida[i]);
						}
						for(i=0; i<50 ; i++){
							for(j=0; (j<=n) && (n<51000) ; j++){
								expelida_comida(&expelida[j],&comida[i]);
								expelida_flash(&flash,&expelida[j],&fim);
								expelida_reverso(&reverso, &expelida[j]);
							}
						}
						for(i=n;i>(-1);i--){
							for(j=n;j>(-1);j--){
								if(i!=j){
									expelida_comida(&expelida[i],&expelida[j]);
								}
							}
						}
					}
					flashXreverso(&flash,&reverso,&ganhou,&fim);
					al_draw_textf(fonte_32,al_map_rgb(255,0,0),letras_x,letras_y,1,"Tempo atual: %.2f",tempo);
					al_draw_textf(fonte_32,al_map_rgb(255,0,0),letras_x2,letras_y,1,"Tempo atual: %.2f",recorde);
					tempo = tempo+(1/FPS);
					al_flip_display();
	   
				}else{
					if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){//Evento do mouse
						flash.vx += -(ev.mouse.x - flash.x)/dist(ev.mouse.x, flash.x, ev.mouse.y, flash.y);
						flash.vy += -(ev.mouse.y - flash.y)/dist(ev.mouse.x, flash.x, ev.mouse.y, flash.y);
						cria_expelida(&expelida[n+1],&flash,ev.mouse.x,ev.mouse.y);
						n++;
						flash.raio=flash.raio * 0.9;
					}
				}
			}
		}
		if(fim == true){
			al_stop_sample_instance(inst_trilha_sonora);
		}
	}
	if(ganhou>0){//Se ganhar
		al_clear_to_color(BKG_COLOR);
		if(tempo<recorde){
			al_play_sample_instance(inst_aplausos_gritos);
			fclose(record);
			FILE *record = fopen("record.txt", "w");
			al_draw_textf(fonte_40, al_map_rgb(0, 255, 255), largura/2, letras_y2, 1, "Vitória :D");
			al_draw_textf(fonte_40, al_map_rgb(218,165,38), largura/2, letras_y3, 1, "Novo recorde: %.2fs",tempo);
			al_draw_textf(fonte_40, al_map_rgb(255,255, 255), largura/2, letras_y4, 1, "Antigo Recorde: %.2fs",recorde);
			fprintf(record,"%.2f",tempo);
		}else{
			al_play_sample_instance(inst_aplausos);
			al_draw_textf(fonte_40, al_map_rgb(0, 255, 255), largura/2, letras_y2, 1, "Vitória :D");
			al_draw_textf(fonte_40, al_map_rgb(218,165,38), largura/2, letras_y3, 1, "Recorde: %.2fs",recorde);
			al_draw_textf(fonte_40, al_map_rgb(255, 255, 255), largura/2, letras_y4, 1, "Seu tempo: %.2fs",tempo);
		}
		al_flip_display();
		al_rest(5.0);
		al_stop_sample_instance(inst_aplausos);
		al_stop_sample_instance(inst_aplausos_gritos);
	}else{//Se perder
		al_play_sample_instance(inst_vaias);
		al_clear_to_color(BKG_COLOR);
		al_draw_textf(fonte_40, al_map_rgb(0, 255, 255), largura/2, letras_y2, 1, "Perdeu :(");
		al_draw_textf(fonte_40, al_map_rgb(218,165,38), largura/2, letras_y3, 1, "Recorde: %.2fs",recorde);
		al_draw_textf(fonte_40, al_map_rgb(255, 255, 255), largura/2, letras_y4, 1, "Seu tempo de jogo: %.2fs",tempo);
		al_flip_display();
		al_rest(5.0);
		al_stop_sample_instance(inst_vaias);
	}
	fclose(record);
	al_destroy_font(fonte_40); //destruir os ponteiros
	al_destroy_font(fonte_32);
	al_destroy_font(fonte_20);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_sample(trilha_sonora);
	al_destroy_sample(aplausos);
	al_destroy_sample(aplausos_gritos);
	al_destroy_sample(vaias);
	al_destroy_sample_instance(inst_trilha_sonora);
	al_destroy_sample_instance(inst_aplausos);
	al_destroy_sample_instance(inst_aplausos_gritos);
	al_destroy_sample_instance(inst_vaias);
	return 0;
}
