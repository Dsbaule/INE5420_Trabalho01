#include <iostream>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

//Objetos da main window
GtkBuilder *builder;
Viewport* viewport;
Coordinates polygon_coords;
Coordinates curve_coords;
Coordinates surface_coords;
gint rows_s;
gint columns_s;
bool isObject3D = false;
std::vector<Transformation> accumulator;
std::vector<Polygon> faces_object3D;

GObject *main_w;
GtkListStore  *store;
GtkTreeIter iter;
GtkTreeView* objects_tree;
GtkWidget *view;
GtkTreeSelection* objects_select;

GtkWidget* draw_viewport;
GtkButton* zoom_in;
GtkButton* zoom_out;
GtkButton* move_down;
GtkButton* move_up;
GtkButton* move_right;
GtkButton* move_left;
GtkButton* add_geometric_shape;
GtkButton* change_object;
GtkButton* rotate_left;
GtkButton* rotate_right;
GtkEntry* step_entry;
GtkEntry* angle_entry;

//Enum para TreeView
enum {
  COL_ID = 0,
  COL_NAME,
  COL_TYPE,
  NUM_COLS
};

int count_obj = 0;

//Objetos da janela de adicionar forma geometrica
GObject* add_geometric_shape_w;

//Objetos da janela de adicionar ponto, linha e poligono
GObject* add_point_w;
GtkEntry* name_point_entry;
GtkEntry* x1_point_entry;
GtkEntry* y1_point_entry;
GtkEntry* z1_point_entry;
GtkButton* add_point;

GObject* add_line_w;
GtkEntry* name_line_entry;
GtkEntry* x1_line_entry;
GtkEntry* y1_line_entry;
GtkEntry* z1_line_entry;
GtkEntry* x2_line_entry;
GtkEntry* y2_line_entry;
GtkEntry* z2_line_entry;
GtkButton* add_line;

GObject* add_poly_w;
GtkEntry* name_poly_entry;
GtkEntry* x_poly_entry;
GtkEntry* y_poly_entry;
GtkEntry* z_poly_entry;
GtkButton* add_point_poly;
GtkButton* add_poly;
GtkToggleButton* filled;

/* CALLBACKS */
/* MAIN_W */
void on_zoom_in_button_clicked (GtkWidget *widget, gpointer data);
void on_zoom_out_button_clicked (GtkWidget *widget, gpointer data);
void on_up_button_clicked (GtkWidget *widget, gpointer data);
void on_down_button_clicked (GtkWidget *widget, gpointer data);
void on_left_button_clicked (GtkWidget *widget, gpointer data);
void on_right_button_clicked (GtkWidget *widget, gpointer data);
void on_back_button_clicked (GtkWidget *widget, gpointer data);
void on_forward_button_clicked (GtkWidget *widget, gpointer data);
void on_rotate_right_clicked (GtkWidget *widget, gpointer data);
void on_rotate_left_clicked (GtkWidget *widget, gpointer data);
void on_add_object_button_clicked (GtkWidget *widget, gpointer data);
void on_change_object_clicked (GtkWidget *widget, gpointer data);
/* ADD_GEOMETRIC_W */
/* ADD_POINT_W */
void on_add_point_clicked (GtkWidget *widget, gpointer data);
/* ADD_LINE_W */
void on_add_line_clicked (GtkWidget *widget, gpointer data);
/* ADD_POLY */
void on_add_point_poly_clicked (GtkWidget *widget, gpointer data);
void on_add_poly_clicked (GtkWidget *widget, gpointer data);

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data);

void fov_scale_event();
int get_index_selected();

void fill_treeview (const char* name, const char* type);

void create_treeview (void);

int main (int argc, char **argv)
{
    gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "interface.glade", NULL);

    /* Connect signal handlers to the constructed widgets. */
    main_w = gtk_builder_get_object (builder, "main_w");
    g_signal_connect (main_w, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_window_set_resizable (GTK_WINDOW(main_w),  false);

    /* Tamanho da viewport */
	viewport = new Viewport(510,515);
	/* Connect Treeview*/
	objects_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "object_tree"));
	create_treeview();

    draw_viewport = GTK_WIDGET(gtk_builder_get_object(builder, "draw_viewport"));
	g_signal_connect(draw_viewport, "draw", G_CALLBACK(draw_objects), NULL);

/* --------------------------------------------------
    Buttons
-------------------------------------------------- */

    /* Zoom */
	zoom_out = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_out"));
	g_signal_connect (zoom_out, "clicked", G_CALLBACK (on_zoom_out_button_clicked), NULL);

	zoom_in = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_in"));
	g_signal_connect (zoom_in, "clicked", G_CALLBACK (on_zoom_in_button_clicked), NULL);

    /* Movement */
	move_up = GTK_BUTTON(gtk_builder_get_object(builder, "move_up"));
	g_signal_connect (move_up, "clicked", G_CALLBACK (on_up_button_clicked), NULL);

	move_down = GTK_BUTTON(gtk_builder_get_object(builder, "move_down"));
	g_signal_connect (move_down, "clicked", G_CALLBACK (on_down_button_clicked), NULL);

	move_left = GTK_BUTTON(gtk_builder_get_object(builder, "move_left"));
	g_signal_connect (move_left, "clicked", G_CALLBACK (on_left_button_clicked), NULL);

	move_right = GTK_BUTTON(gtk_builder_get_object(builder, "move_right"));
	g_signal_connect (move_right, "clicked", G_CALLBACK (on_right_button_clicked), NULL);

    /* Rotation */
    rotate_right = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_right"));
    g_signal_connect (rotate_right, "clicked", G_CALLBACK (on_rotate_right_clicked), NULL);

    rotate_left = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_left"));
    g_signal_connect (rotate_left, "clicked", G_CALLBACK (on_rotate_left_clicked), NULL);

    /* Geometric Objects */
	add_geometric_shape = GTK_BUTTON(gtk_builder_get_object(builder, "add_geometric_shape"));
	g_signal_connect (add_geometric_shape, "clicked", G_CALLBACK (on_add_object_button_clicked), NULL);

    /* Window for new Geometric Shape */
    add_geometric_shape_w = gtk_builder_get_object (builder, "add_geometric_shape_w");
    g_signal_connect (add_geometric_shape_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

    /* Point */
	add_point = GTK_BUTTON(gtk_builder_get_object(builder, "add_point"));
	g_signal_connect (add_point, "clicked", G_CALLBACK (on_add_point_clicked), NULL);

    /* Line */
	add_line = GTK_BUTTON(gtk_builder_get_object(builder, "add_line"));
	g_signal_connect (add_line, "clicked", G_CALLBACK (on_add_line_clicked), NULL);

    /* Polygon */
	add_point_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_point_poly"));
	g_signal_connect (add_point_poly, "clicked", G_CALLBACK (on_add_point_poly_clicked), NULL);
	add_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_poly"));
	g_signal_connect (add_poly, "clicked", G_CALLBACK (on_add_poly_clicked), NULL);

/* --------------------------------------------------
    Entries
-------------------------------------------------- */

    /* Movement */
    step_entry = GTK_ENTRY(gtk_builder_get_object(builder, "step_entry"));
    /* Rotation */
    angle_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_entry"));
    /* Point */
    name_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_point_entry"));
    x1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_point_entry"));
    y1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_point_entry"));
    z1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z1_point_entry"));
    /* Line */
    name_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_line_entry"));
    x1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_line_entry"));
    y1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_line_entry"));
    z1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z1_line_entry"));
    x2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x2_line_entry"));
    y2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y2_line_entry"));
    z2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z2_line_entry"));
    /* Polygon */
    name_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_poly_entry"));
    x_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x_poly_entry"));
    y_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y_poly_entry"));
    z_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z_poly_entry"));
    filled = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"fill_poly_checkButton"));

	gtk_widget_show(GTK_WIDGET(main_w));

	gtk_main ();

	return 0;
}

/* CALLBACKS */

/* MAIN_W */
void on_zoom_in_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->zoom(step);
}

void on_zoom_out_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->zoom(-step);
}

void on_up_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveY(step);
}

void on_down_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveY(-step);
}

void on_left_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveX(-step);
}

void on_right_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveX(step);
}

void on_rotate_right_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_entry));
	if (gtk_toggle_button_get_active(x_check)) {
	    viewport->rotate_window_on_x(-angle);
	} else if (gtk_toggle_button_get_active(y_check)) {
	    viewport->rotate_window_on_y(-angle);
	} else if (gtk_toggle_button_get_active(z_check)) {
	    viewport->rotate_window_on_z(-angle);
	}
}

void on_rotate_left_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_entry));
	if (gtk_toggle_button_get_active(x_check)) {
	    viewport->rotate_window_on_x(angle);
	} else if (gtk_toggle_button_get_active(y_check)) {
	    viewport->rotate_window_on_y(angle);
	} else if (gtk_toggle_button_get_active(z_check)){
	    viewport->rotate_window_on_z(angle);
	}
}

void on_add_object_button_clicked (GtkWidget *widget, gpointer data) {
	gtk_widget_show (GTK_WIDGET(add_geometric_shape_w));
}

/* ADD_POINT_W */
void on_add_point_clicked (GtkWidget *widget, gpointer data) {
	const gchar* name = gtk_entry_get_text(name_point_entry);
	double x1 = atof(gtk_entry_get_text(x1_point_entry));
	double y1 = atof(gtk_entry_get_text(y1_point_entry));
	double z1 = atof(gtk_entry_get_text(z1_point_entry));

	fill_treeview(name,"Point");
	Point* point = new Point(name, x1, y1, z1);
	viewport->addObject(point);

    gtk_entry_set_text(name_point_entry,"");
    gtk_entry_set_text(x1_point_entry,"");
    gtk_entry_set_text(y1_point_entry,"");
    gtk_entry_set_text(z1_point_entry,"");
    gtk_widget_hide (GTK_WIDGET(add_point_w));
}

/* ADD_LINE_W */
void on_add_line_clicked (GtkWidget *widget, gpointer data) {
	const gchar* name = gtk_entry_get_text(name_line_entry);
	double x1 = atof(gtk_entry_get_text(x1_line_entry));
	double y1 = atof(gtk_entry_get_text(y1_line_entry));
	double z1 = atof(gtk_entry_get_text(z1_line_entry));
	double x2 = atof(gtk_entry_get_text(x2_line_entry));
	double y2 = atof(gtk_entry_get_text(y2_line_entry));
	double z2 = atof(gtk_entry_get_text(z2_line_entry));

	fill_treeview(name,"Line");
	Line* line = new Line(name, x1, y1, z1, x2, y2, z2);
	viewport->addObject(line);
    gtk_entry_set_text(name_line_entry,"");
    gtk_entry_set_text(x1_line_entry,"");
    gtk_entry_set_text(x2_line_entry,"");
    gtk_entry_set_text(y1_line_entry,"");
    gtk_entry_set_text(y2_line_entry,"");
    gtk_entry_set_text(z1_line_entry,"");
    gtk_entry_set_text(z2_line_entry,"");
    gtk_widget_hide (GTK_WIDGET(add_line_w));
}

/* ADD_POLY */
void on_add_point_poly_clicked (GtkWidget *widget, gpointer data) {
	double x1 = atof(gtk_entry_get_text(x_poly_entry));
	double y1 = atof(gtk_entry_get_text(y_poly_entry));
	double z1 = atof(gtk_entry_get_text(z_poly_entry));
	polygon_coords.push_back(Coordinate(x1,y1,z1));
    gtk_entry_set_text(x_poly_entry,"");
    gtk_entry_set_text(y_poly_entry,"");
    gtk_entry_set_text(z_poly_entry,"");
}

void on_add_poly_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_poly_entry);
	Polygon* polygon = new Polygon(name, polygon_coords, gtk_toggle_button_get_active(filled));
	if (!isObject3D) {
	    fill_treeview(name,"Polygon");
	    viewport->addObject(polygon);
	    gtk_widget_hide (GTK_WIDGET(add_poly_w));
	} else {
	    faces_object3D.push_back(*polygon);
	    gtk_widget_hide (GTK_WIDGET(add_poly_w));
	}
	polygon_coords.clear();
    gtk_entry_set_text(name_poly_entry, "");

}

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data) {
	cairo_set_source_rgb (cr, 1, 0, 0);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

    //int width = gtk_widget_get_allocated_width (widget);
    //int height = gtk_widget_get_allocated_height (widget);
    //530x535
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.0);

    viewport->drawDisplayFile(cr);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, 10, 10);
    cairo_line_to(cr, 520, 10);
    cairo_line_to(cr, 520, 525);
    cairo_line_to(cr, 10, 525);
    cairo_line_to(cr, 10, 10);
    cairo_stroke(cr);


	gtk_widget_queue_draw(draw_viewport);
	return FALSE;
}

int get_index_selected() {
	GtkTreeIter iter;
	GtkTreeModel *model;
	int index;

	if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
		gtk_tree_model_get (model, &iter, COL_ID, &index, -1);
	}
	return index;
}

void fill_treeview (const char* name, const char* type) {
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COL_ID, count_obj, COL_NAME, name, COL_TYPE, type,-1);
	count_obj++;
}

void create_treeview (void) {
	GtkCellRenderer *renderer;
	GtkTreeModel *model;

	/* Column 1 */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "ID", renderer, "text", COL_ID, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Name", renderer, "text", COL_NAME, NULL);
	/* Column 2 */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Type", renderer, "text", COL_TYPE, NULL);


	model = GTK_TREE_MODEL (store = gtk_list_store_new (NUM_COLS, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING));
	gtk_tree_view_set_model (GTK_TREE_VIEW (objects_tree), model);

	objects_select = gtk_tree_view_get_selection(objects_tree);
	gtk_tree_selection_set_mode(objects_select, GTK_SELECTION_SINGLE);

	g_object_unref (model);
}
