#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define LINECOLOR al_map_rgb(0, 0, 255)
#define LINEWIDTH -1
#define BGCOLOR al_map_rgb(0, 0, 0)

#define LOGDIE(label, str) { errorcode = EXIT_FAILURE; errormsg = strdup(str); goto label; }
char *errormsg = NULL;
int errorcode = EXIT_SUCCESS;

void sierpinski(int n, float x1, float y1, float side) {
	// Turn this into a recursive function!
	al_draw_triangle(x1, y1,
			x1 + side, y1,
			x1 + side/2, y1 - side*sqrt(3.0)/2,
			LINECOLOR, LINEWIDTH);
}

void usage(int argc, char **argv)
{
	printf("usage: %s <side> <nest>\n", argv[0]);
	printf("\n");
	printf("Draw a Sierpinski triangle with a side of <side> pixels\n");
	printf("nested <n> times.  Requires:\n");
	printf("\n");
	printf("100 <= side <= 2000\n");
	printf("1 <= side <= 16\n");
	printf("\n");
	printf("Press 'q' to quit the program.\n");
	exit(EXIT_FAILURE);
}

void init_params(int argc, char **argv, int *side, int *n)
{
	if(argc < 3) usage(argc, argv);
	*side = atoi(argv[1]);
	*n = atoi(argv[2]);
	if(*side < 100 || *side > 2000 || *n < 1 || *n > 16) usage(argc, argv);
}

int main(int argc, char **argv)
{
	int side = 0;
	int n = 0;
	init_params(argc, argv, &side, &n);

	if(!al_init())
		LOGDIE(out_failure, "failed: al_init");
	if(!al_init_primitives_addon())
		LOGDIE(out_failure, "failed: as_init_primitives_addon");
	if(!al_install_keyboard())
		LOGDIE(out_primitives, "failed: al_install_keyboard");

	ALLEGRO_DISPLAY *display = al_create_display(side, side);
	if(!display) LOGDIE(out_keyboard, "failed: al_create_display");
	ALLEGRO_EVENT_QUEUE *evqueue = al_create_event_queue();
	if(!evqueue) LOGDIE(out_display, "failed: al_create_event_queue");

	al_register_event_source(evqueue, al_get_display_event_source(display));
	al_register_event_source(evqueue, al_get_keyboard_event_source());

	al_flip_display();
	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(BGCOLOR);
	sierpinski(n, 5, side-5, side-10);
	al_flip_display();

	int waiting = 1;
	while(waiting) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(evqueue, &ev);
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_Q:
					waiting = 0;
					break;
			}
		} else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			waiting = 0;
		}
	}

	al_destroy_event_queue(evqueue);
	out_display:
	al_destroy_display(display);
	out_keyboard:
	al_uninstall_keyboard();
	out_primitives:
	al_shutdown_primitives_addon();
	out_failure:
	if(errormsg) {
		fprintf(stderr, "%s\n", errormsg);
		free(errormsg);
	}
	exit(errorcode);
}