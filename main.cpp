#include <gtk/gtk.h>

int main (int argc, char **argv)
{
    gtk_init (&argc, &argv);

    GtkWidget *mainWindow;

    GtkWidget *mainFrame;
    GtkWidget *controlFrame;
    GtkWidget *viewportFrame;

    GtkWidget *mainGrid;
    GtkWidget *controlGrid;

    GtkWidget *upButton;
    GtkWidget *rightButton;
    GtkWidget *downButton;
    GtkWidget *leftButton;

    GtkWidget *zoomInButton;
    GtkWidget *zoomOutButton;

    GtkWidget *space = gtk_label_new("");

    mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (mainWindow), "Sistema Viewport");
    gtk_window_set_default_size (GTK_WINDOW (mainWindow), 800, 600);
    g_signal_connect (mainWindow, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (mainWindow), 10);

    mainFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (mainFrame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (mainWindow), mainFrame);

    mainGrid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous (GTK_GRID (mainGrid), TRUE);
    gtk_grid_set_column_homogeneous (GTK_GRID (mainGrid), TRUE);
    gtk_container_add (GTK_CONTAINER (mainFrame), mainGrid);

    controlFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (controlFrame), GTK_SHADOW_IN);
    gtk_frame_set_label(GTK_FRAME (controlFrame), "Controles");
    gtk_grid_attach (GTK_GRID (mainGrid), controlFrame, 0, 0, 2, 6);

    controlGrid = gtk_grid_new ();
    gtk_grid_set_row_homogeneous (GTK_GRID (controlGrid), TRUE);
    gtk_grid_set_column_homogeneous (GTK_GRID (controlGrid), TRUE);
    gtk_container_add (GTK_CONTAINER (controlFrame), controlGrid);

    viewportFrame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (viewportFrame), GTK_SHADOW_IN);
    gtk_frame_set_label(GTK_FRAME (viewportFrame), "Viewport");
    gtk_grid_attach (GTK_GRID (mainGrid), viewportFrame, 2, 0, 6, 6);

    upButton    = gtk_button_new_with_label ("↑");
    rightButton = gtk_button_new_with_label ("→");
    downButton  = gtk_button_new_with_label ("↓");
    leftButton  = gtk_button_new_with_label ("←");

    zoomInButton  = gtk_button_new_with_label ("+");
    zoomOutButton  = gtk_button_new_with_label ("-");

    gtk_grid_attach (GTK_GRID (controlGrid), space, 0, 1, 5, 5);
    gtk_grid_attach (GTK_GRID (controlGrid), upButton, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), rightButton, 2, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), downButton, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), leftButton, 0, 1, 1, 1);

    gtk_grid_attach (GTK_GRID (controlGrid), zoomInButton, 4, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (controlGrid), zoomOutButton, 4, 2, 1, 1);

    zoomInButton    = gtk_button_new_with_label ("Teste 5");
    zoomOutButton   = gtk_button_new_with_label ("Teste 6");

    gtk_widget_show_all (mainWindow);

    gtk_main ();

    return 0;
}
