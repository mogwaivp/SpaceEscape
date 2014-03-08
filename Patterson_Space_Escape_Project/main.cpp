// FILE: main.cpp
// PROG: Mahogany Patterson
// PURP: 

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <cstdio>                           //For char * functions gotten from close encounters main
#include <ctime>
#include "spaceEscapeDefs.h"

//Function prototypes
bool initializeAllegro();
void handleKeyUp(int, bool &);
void handleKeyDown(int, bool &);
//void loadAsteroidImages();


//Global bitmap pointers & essential Allegro pointers so they can be used anywhere
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *arial14;
ALLEGRO_BITMAP *backgroundStars;
bool keys[] = {false, false, false, false, false};

int main()
{
	//int ct;                                  //Generic counter
	int collisionCounter = 0;                     //Counter to control fred's behaviour after collision
	int collision = 0;                            //Number of collisions
	int backgroundImageWidth = 0, backgrooundImageHeight = 0;

	bool redrawRequired = true, quit = false;

	//Initialize allegro, etc in separate function
	if(!initializeAllegro()) return -1;

	srand((unsigned) time(NULL));

	//Load the font
	arial14 = al_load_font("arial.ttf", 14, 0);
	if(!arial14) return -3;

	//Load background image used to paint the background
	if(!(backgroundStars = al_load_bitmap("starbackground.bmp"))) return -2;

	//Get dimensions of the background image
	backgroundImageWidth = al_get_bitmap_width(backgroundStars);
	backgrooundImageHeight = al_get_bitmap_height(backgroundStars);

	//Initialize event queue
	eventQueue = al_create_event_queue();
	if(!eventQueue)
	{
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	//Register events as arrving from these sources: display, timer, keyboard
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	//load asteroid images
	//loadAsteroidImages();

	//Instantiate sprites, make sure to delete them at end

	//Load sprite images if needed
	//Set width and height of images

	//Set the back buffer as the drawing bitmap for the display
	al_set_target_bitmap(al_get_backbuffer(display));

	al_hide_mouse_cursor(display);                                                           //if mouse should be hidden

	//Start timer just before game start
	al_start_timer(timer);

	//Game Loop
	while(!quit)
	{
		ALLEGRO_EVENT evt;
		//wait for event
		al_wait_for_event(eventQueue, &evt);

		//an event was generated. check for all possible event types
		switch(evt.type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			handleKeyDown(evt.keyboard.keycode, quit);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			handleKeyUp(evt.keyboard.keycode, quit);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			quit = true;
			break;
		case ALLEGRO_EVENT_TIMER:
			//if no collisions, check for one. stops any collision detection while one is already detected
			if(!collisionCounter)
			{
				//for loop checking each sprite for collision
				//if there is a collision, handle collision sprite movement
				collisionCounter = 60;
				++collision;
				//kill for loop by setting ct = numSprites
				//END if collision

			}//END collision checking

			if(collisionCounter)
			{
				--collisionCounter;
			}
			else
			{
				//update Fred position
				//Fred->handleArrowKeys();
				//Fred->updatePosition();
				//Fred->warpSprite();
			}//END Fred collision handling

			//for(ct = 0; ct < numsprites: ++ct)
			//{
			//Update exterior sprite positions and frame #s
			//sprites->updateSpriteFrame();
			//sprites->updateSpritePosition();
			//sprites->warpSprite();
			//}

			redrawRequired = true;
			break;
		}//END switch evt.type

		//Rerender entire scene
		if(redrawRequired && al_is_event_queue_empty(eventQueue))
		{
			redrawRequired = false;
			al_clear_to_color(BLACK);

			//Draw background
			al_draw_scaled_bitmap(backgroundStars, 0, 0, backgroundImageWidth, backgrooundImageHeight, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

			//Text statements
			al_draw_textf(arial14,WHITE, 0, 585, 0, "Press esc to quit...");
			al_draw_textf(arial14, WHITE, 0, 0, 0, "Collision: %d", collision);
			al_draw_textf(arial14, WHITE, 0, 15, 0, "Timer (secs): %4d", al_get_timer_count(timer)/ FPS);

			al_flip_display();

		}//END render

	}//End game loop

	//Release all dynanmically allocated memory
	al_destroy_bitmap(backgroundStars);
	al_destroy_font(arial14);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(eventQueue);

	return 0;
}

void handleKeyDown(int keyCode, bool &quit)
{
	switch(keyCode)
	{
	case ALLEGRO_KEY_ESCAPE:
		quit = true;
		break;
	case ALLEGRO_KEY_LEFT:
		keys[KEY_LEFT] = true;
		break;
	case ALLEGRO_KEY_RIGHT:
		keys[KEY_RIGHT] = true;
		break;
	case ALLEGRO_KEY_UP:
		keys[KEY_UP] = true;
		break;
	case ALLEGRO_KEY_DOWN:
		keys[KEY_DOWN] = true;
		break;
	case ALLEGRO_KEY_SPACE:
		keys[SPACEBAR] = true;
		break;
	}
}//END handleKeyDown

void handleKeyUp(int keyCode, bool &quit)
{
		switch(keyCode)
	{
	case ALLEGRO_KEY_ESCAPE:
		quit = true;
		break;
	case ALLEGRO_KEY_LEFT:
		keys[KEY_LEFT] = false;
		break;
	case ALLEGRO_KEY_RIGHT:
		keys[KEY_RIGHT] = false;
		break;
	case ALLEGRO_KEY_UP:
		keys[KEY_UP] = false;
		break;
	case ALLEGRO_KEY_DOWN:
		keys[KEY_DOWN] = false;
		break;
	case ALLEGRO_KEY_SPACE:
		keys[SPACEBAR] = false;
		break;
	}
}//END handleKeyUp

bool initializeAllegro()
{
	bool success = true;

	//Init basic environment
	if(!al_init())
	{
		al_show_native_message_box(NULL, "ERROR", "Allegro failed to initialize!", NULL, NULL, NULL);
		success = false;
	}

	//Init keyboard
	if(!al_install_keyboard())
	{
		al_show_native_message_box(NULL, "ERROR", "Keyboard failed to initialize!", NULL, NULL, NULL);
		success = false;
	}

	//Init timer
	timer = al_create_timer(1.0 / FPS);
	if(!timer)
	{
		al_show_native_message_box(NULL, "ERROR", "Timer failed to initialize!", NULL, NULL, NULL);
		success = false;
	}

	//Init display
		display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!display)
	{
		al_show_native_message_box(NULL, "ERROR", "Display failed to initialize!", NULL, NULL, NULL);
		success = false;
	}

	//Init image mechanism
	if(!al_init_image_addon())
	{
		al_show_native_message_box(NULL, "ERROR", "Image addon failed to initialize!", NULL, NULL, NULL);
		success = false;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	return success;
}//END initializeAllegro