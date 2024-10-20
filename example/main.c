/*  *--<Preface>--*  //

 -=-  Author Details  -=-
 Blair Edwards
 My own shenanigans.

 -=-  Dates  -=-
 Started 2024-09-13

 -=-  Description  -=-
 This programme provides a working example for implementing the 'sp-gtk4-loading-logos.git' library.
 There is no input.
 Output is primarily through a GTK4 GUI.
 Config is through compile-time '#define's.

 -=-  Credit where Credit is Due  -=-
 -=>  Library / code snippet etc.

 -=-  Notes  -=-
 -=>  I've developed my own commenting notation for things that "aren't done" one way or another.  Such as:
	 -  //#  TODO
	 -  //?  Not sure / query
	 -  //!  Important note / relevant as technology advances
 -=>  Logging with `_LOG ()` takes a 'logLevel' argument, which roughly indicates:
	 -  0:  Critical, major errors, should Always be printed.
	 -  1:	Important info particularly critical functions, minor / user errors.
	 -  2:	Useful info / general programme flow.
	 -  3:	Debug info, steps throughout a function.
	 -  4:  Useful spam - printed often such as in a loop.
	 -  5:  Debug spam - printed often such as in a loop.
 -=>

//  *--</Preface>--*  */



//  *--<Preparations>--*  //

//  Includes
#include <stdio.h>
#include <stdint.h>
#include <gtk/gtk.h>
#include "../sp-gtk4-loading-logos/sp-gtk4-loading-logos.h"

//  Defines
#define GUI_TITLE "SP GTK4 Loading Logos Example"
#define GUI_URL "uk.second-pair.libraries.sp-gtk4-loading-logos.example"
#define GUI_APP_FLAGS G_APPLICATION_DEFAULT_FLAGS
/*  Window Sizing
0:  No target size - as allocated.
1:  Fixed target size.
2:  Fullscreen.
*/
#define GUI_WINDOW_MODE 1
#define GUI_WINDOW_SIZE_W 640
#define GUI_WINDOW_SIZE_H 480

//  Type Definitions

//  Structures

//  Local Global Variables
static volatile uint8_t gui_keepRunning = 1;

//  Truly Global Variables

//  Prototype Functions
int main (int argc, char** argv);
GtkWidget* gui_layout_create (void);

//  Prototype Callbacks
void cb_app_main_activate (GtkApplication* theApp, gpointer data);
void cb_signal_terminate (int sigType);
void cb_btn_logoAdvance_clicked (GtkButton* theButton, gpointer data);

//  *--</Preparations>--*  //



//  *--<Main Code>--*  //

int main (int argc, char** argv)
{
	//  Register signal handlers for signals we want to trap.
	#if (defined _WIN32 || defined _WINDOWS || defined __CYGWIN__ || defined __NT__)
		//  Windows can't speak 'sigaction'.
		signal (SIGINT, cb_signal_terminate);
		signal (SIGQUIT, cb_signal_terminate);
		signal (SIGABRT, cb_signal_terminate);
		signal (SIGTERM, cb_signal_terminate);
	#else
		//  UNIX should use 'sigaction' where possible.
		struct sigaction sigHandler;
		sigHandler .sa_handler = cb_signal_terminate;
	    sigaction (SIGINT, &sigHandler, NULL);
		sigaction (SIGQUIT, &sigHandler, NULL);
		sigaction (SIGABRT, &sigHandler, NULL);
		sigaction (SIGTERM, &sigHandler, NULL);
	#endif

	//  Create and run the application.
	GtkApplication* app_main = gtk_application_new (GUI_URL, GUI_APP_FLAGS);
	g_signal_connect (app_main, "activate", G_CALLBACK (cb_app_main_activate), NULL);
	int status = g_application_run (G_APPLICATION (app_main), argc, argv);

	//  Clean up and head on out.
	g_object_unref (app_main);
	return status;
}

GtkWidget* gui_layout_create (void)
{
	//  Create the top-level container widget.
	GtkWidget* box_main = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	//  Add a title.
	gtk_box_append (GTK_BOX (box_main), gtk_label_new ("Loading Logos"));
	//  Add a test child widget.
	GtkWidget* logo_main = sp_gtk4_loading_logos_create (0);
	gtk_box_append (GTK_BOX (box_main), logo_main);
	//  Add a button to advance the logo type.
	GtkWidget* btn_logoAdvance = gtk_button_new_with_label ("Advance Logo");
	gtk_box_append (GTK_BOX (box_main), btn_logoAdvance);
	g_signal_connect (btn_logoAdvance, "clicked", G_CALLBACK (cb_btn_logoAdvance_clicked), logo_main);

	return box_main;
}

//  *--</Main Code>--*  //



//  *--<Callbacks>--*  //

void cb_app_main_activate (GtkApplication* theApp, gpointer data)
{
	//  Create the window.
	GtkWidget* window_main = gtk_application_window_new (theApp);
	gtk_window_set_title (GTK_WINDOW (window_main), GUI_TITLE);

	//  Determine how we want to size the window.
	#if (GUI_WINDOW_MODE == 1)
		gtk_window_set_default_size (GTK_WINDOW (window_main), GUI_WINDOW_SIZE_W, GUI_WINDOW_SIZE_H);
	#elif(GUI_WINDOW_MODE == 2)
		gtk_window_fullscreen (GTK_WINDOW (window_main));
	#endif

	//  Create its child and present it.
	gtk_window_set_child (GTK_WINDOW (window_main), gui_layout_create ());
	gtk_window_present (GTK_WINDOW (window_main));
}

//  Trap the signals we're interested in.
void cb_signal_terminate (int sigType)
{
	gui_keepRunning = 0;
	exit (0);
}

void cb_btn_logoAdvance_clicked (GtkButton* theButton, gpointer data)
{
	if (data == NULL)
		return;
	GtkWidget* theLogo = (GtkWidget*)data;
	SP_GTK4_LOADING_LOGOS_ENUM_TYPE currType = sp_gtk4_loading_logos_get_type (theLogo);
	if (++currType > sp_gtk4_loading_logos_max_type ())
		currType = 0;
	sp_gtk4_loading_logos_set_type (theLogo, currType);
}

//  *--</Callbacks>--*  //
