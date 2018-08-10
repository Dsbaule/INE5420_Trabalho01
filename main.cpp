#include <gtk/gtk.h>

int main (int argc, char **argv)
{
    gtk_init (&argc, &argv);

    GtkWidget *mainWindow;

    GtkWidget *mainFrame;
    GtkWidget *controlFrame;
    GtkWidget *objectFrame;
    GtkWidget *viewportFrame;
    GtkWidget *consoleFrame;

    GtkWidget *mainGrid;
    GtkWidget *controlGrid;

    GtkWidget *upButton;
    GtkWidget *rightButton;
    GtkWidget *downButton;
    GtkWidget *leftButton;

    GtkWidget *zoomInButton;
    GtkWidget *zoomOutButton;

    GtkWidget *space = gtk_label_new("*");

    // Window creation
    mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (mainWindow), "Sistema Viewport");
    gtk_window_set_default_size (GTK_WINDOW (mainWindow), 800, 600);
    g_signal_connect (mainWindow, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (mainWindow), 2);

    // Main Frame creation
    mainFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (mainFrame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (mainWindow), mainFrame);

    // Main Grid creation
    mainGrid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous (GTK_GRID (mainGrid), TRUE);
    gtk_grid_set_column_homogeneous (GTK_GRID (mainGrid), TRUE);
    gtk_container_add (GTK_CONTAINER (mainFrame), mainGrid);

    /* --------------------------------------------------
     *  Main frame objects
    -------------------------------------------------- */

    // Objects Frame
    objectFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (objectFrame), GTK_SHADOW_IN);
    gtk_container_set_border_width (GTK_CONTAINER (objectFrame), 2);
    gtk_frame_set_label(GTK_FRAME (objectFrame), "Objetos");
    gtk_grid_attach (GTK_GRID (mainGrid), objectFrame, 0, 0, 3, 2);

    // Control Frame
    controlFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (controlFrame), GTK_SHADOW_IN);
    gtk_container_set_border_width (GTK_CONTAINER (controlFrame), 2);
    gtk_frame_set_label(GTK_FRAME (controlFrame), "Controles");
    gtk_grid_attach (GTK_GRID (mainGrid), controlFrame, 0, 2, 3, 4);

    // Viewport Frame
    viewportFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (viewportFrame), GTK_SHADOW_IN);
    gtk_container_set_border_width (GTK_CONTAINER (viewportFrame), 2);
    gtk_frame_set_label(GTK_FRAME (viewportFrame), "Viewport");
    gtk_grid_attach (GTK_GRID (mainGrid), viewportFrame, 3, 0, 5, 5);

    // Console frame
    consoleFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (consoleFrame), GTK_SHADOW_IN);
    gtk_container_set_border_width (GTK_CONTAINER (consoleFrame), 2);
    gtk_frame_set_label(GTK_FRAME (consoleFrame), "Console");
    gtk_grid_attach (GTK_GRID (mainGrid), consoleFrame, 3, 5, 5, 1);


    /* --------------------------------------------------
     *  Control objects
    -------------------------------------------------- */

    controlGrid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous (GTK_GRID (controlGrid), TRUE);
    gtk_grid_set_row_spacing (GTK_GRID (controlGrid), 0);
    gtk_grid_set_column_homogeneous (GTK_GRID (controlGrid), TRUE);
    gtk_grid_set_column_spacing (GTK_GRID (controlGrid), 0);
    gtk_container_add (GTK_CONTAINER (controlFrame), controlGrid);
    gtk_grid_attach (GTK_GRID (controlGrid), space, 0, 0, 5, 5);

    upButton    = gtk_button_new_with_label ("↑");
    rightButton = gtk_button_new_with_label ("→");
    downButton  = gtk_button_new_with_label ("↓");
    leftButton  = gtk_button_new_with_label ("←");

    zoomInButton  = gtk_button_new_with_label ("+");
    zoomOutButton  = gtk_button_new_with_label ("-");

    gtk_grid_attach (GTK_GRID (controlGrid), upButton, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), rightButton, 2, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), downButton, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), leftButton, 0, 1, 1, 1);

    gtk_grid_attach (GTK_GRID (controlGrid), zoomInButton, 4, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), zoomOutButton, 4, 2, 1, 1);

    /* --------------------------------------------------
     *  Window Display
    -------------------------------------------------- */

    gtk_window_resize (GTK_WINDOW (mainWindow), 800, 600);
    gtk_widget_show_all (mainWindow);

    gtk_main ();

    return 0;
}
