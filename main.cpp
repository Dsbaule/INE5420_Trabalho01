#include <iostream>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "Viewport.hpp"
#include "Objects.hpp"
#include "Transformation.hpp"
#include "file_handler.hpp"

//Objetos da main window
GtkBuilder *builder;
Viewport* viewport;
Coordinates polygon_coords;
Coordinates face_coords;
Coordinates curve_coords;
int surface_point_number = 0;
Coordinates surface_coords;
gint rows_s = 4;
gint columns_s = 4;
std::vector<Transformation> accumulator;
std::vector<Polygon> faces_object3D;

GObject *main_w;
GtkListStore  *store;
GtkTreeIter iter;
GtkTreeView* objects_tree;
GtkWidget *view;
GtkTreeSelection* objects_select;

/* ---------- Main Window ---------- */
/* Menu Items */
GtkMenuItem* open_file_m;
GtkMenuItem* save_file_m;
GtkMenuItem* reset_rotation_m;
GtkMenuItem* reset_position_m;
/* Widgets */
GtkWidget* draw_viewport;
/* Buttons */
GtkButton* zoom_in_button;
GtkButton* zoom_out_button;
GtkButton* move_down_button;
GtkButton* move_up_button;
GtkButton* move_right_button;
GtkButton* move_left_button;
GtkButton* move_back_button;
GtkButton* move_forward_button;
GtkButton* add_object_button;
GtkButton* change_object_button;
GtkButton* rotate_left_button;
GtkButton* rotate_right_button;
GtkButton* settings_button;
GtkToggleButton *x_check,*y_check,*z_check;
/* Entries */
GtkEntry* step_entry;
GtkEntry* angle_entry;

/* ---------- Settings Window ---------- */
GObject* settings_w;
/* Buttons */
GtkToggleButton *LB_Clipping,*CS_Clipping;
GtkToggleButton *check_parallel,*check_perspective;
/* Entries */
GtkAdjustment* fov_scale;

/* ---------- Add Object Window ---------- */
GObject* add_object_w;

/* Add Point */
GtkEntry* name_point_entry;
GtkEntry* x_point_entry;
GtkEntry* y_point_entry;
GtkEntry* z_point_entry;
GtkButton* add_point_button;
/* Add Line */
GtkEntry* name_line_entry;
GtkEntry* x1_line_entry;
GtkEntry* y1_line_entry;
GtkEntry* z1_line_entry;
GtkEntry* x2_line_entry;
GtkEntry* y2_line_entry;
GtkEntry* z2_line_entry;
GtkButton* add_line_button;
/* Add Poligon */
GtkEntry* name_poly_entry;
GtkEntry* x_poly_entry;
GtkEntry* y_poly_entry;
GtkEntry* z_poly_entry;
GtkButton* add_point_poly_button;
GtkButton* add_poly_button;
GtkToggleButton* filled_poly_checkbox;
/* Add Curve */
GtkEntry* name_curve_entry;
GtkEntry* x_curve_entry;
GtkEntry* y_curve_entry;
GtkEntry* z_curve_entry;
GtkButton* add_point_curve_button;
GtkButton* add_curve_button;
GtkLabel* control_point_number_label;
GtkToggleButton* curve_bs_checkbox;
/* Add 3D Object */
GtkEntry* name_object3D_entry;
GtkEntry* x_face_entry;
GtkEntry* y_face_entry;
GtkEntry* z_face_entry;
GtkButton* add_point_face_button;
GtkButton* add_face_button;
GtkToggleButton* filled_face_checkbox;
GtkButton* add_object3D_button;
GtkToggleButton* filled_object3D_checkbox;
/* Add Surface */
GtkEntry* name_surface_entry;
GtkEntry* x_surface_entry;
GtkEntry* y_surface_entry;
GtkEntry* z_surface_entry;
GtkButton* add_point_surface_button;
GtkButton* add_surface_button;
GtkLabel* label_current_index;
GtkLabel* label_previous_point;
GtkToggleButton* surface_bs_checkbox;
GtkToggleButton* surface_be_checkbox;
GtkSpinButton* surface_columns_spinbutton;
GtkSpinButton* surface_rows_spinbutton;

// Enum para TreeView
enum {
  COL_ID = 0,
  COL_NAME,
  COL_TYPE,
  NUM_COLS
};

int count_obj = 0;

//Objects from change_obj_w
GObject* change_obj_w;
GtkEntry* angle_world_entry;
GtkEntry* angle_obj_entry;
GtkEntry* trans_y_entry;
GtkEntry* trans_x_entry;
GtkEntry* trans_z_entry;
GtkEntry* angle_point_entry;
GtkEntry* angle_pointx_entry;
GtkEntry* angle_pointy_entry;
GtkEntry* angle_pointz_entry;
GtkEntry* angle_vectorx_entry;
GtkEntry* angle_vectory_entry;
GtkEntry* angle_vectorz_entry;
GtkEntry* sx_entry;
GtkEntry* sy_entry;
GtkEntry* sz_entry;
GtkButton* angle_world_button;
GtkButton* angle_obj_button;
GtkButton* translate_button;
GtkButton* rotate_point_button;
GtkButton* schedule_button;
GtkButton* change_obj_button;
GtkToggleButton *x_checkr,*y_checkr,*z_checkr;

void fill_treeview(const char* name,const char* type);
int get_index_selected();

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

void on_back_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveZ(-step);
}

void on_forward_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveZ(step);
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
	gtk_widget_show (GTK_WIDGET(add_object_w));
}

void on_change_object_clicked (GtkWidget *widget, gpointer data) {
	//esse metodo deve instanciar uma matriz em que será acumulada as transformações
	//cada botão adicionará a sua matriz na matriz atual acumulando as transformações
	GtkTreeIter iter;
	GtkTreeModel *model;
	if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
		gtk_widget_show (GTK_WIDGET(change_obj_w));
	}
}

void open_file (GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          (GtkWindow*)main_w,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == GTK_RESPONSE_ACCEPT){

        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        const gchar* filename = gtk_file_chooser_get_filename (chooser);

        if(filename == nullptr)
            return;

        std::string file(filename);

        delete filename;

        try{
            ObjReader r(file);

            for(auto obj : r.getObjs()){
                try{
                    viewport->addObject(obj);
                    fill_treeview(obj->get_name().c_str(),obj->get_type_name().c_str());

                    // gtk_widget_queue_draw(m_mainWindow);
                }catch(char* e){
                    std::cout<<e<<std::endl;
                    delete obj;
                }
            }

            std::cout<<"Arquivo carregado.\n";
        }catch(char* e){
            std::cout<<e<<std::endl;
        }
    }

    gtk_widget_destroy (dialog);
}

void save_file (GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          (GtkWindow*)main_w,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        const gchar* filename = gtk_file_chooser_get_filename (chooser);

        if(filename == nullptr)
            return;

        std::string file(filename);
        delete filename;

        try{
            ObjWriter w(file);
            w.writeObjs(viewport);

            std::cout<<"Arquivo salvo.\n";
        }catch(char * e){
            std::cout<< e<< std::endl;
        }
    }

    gtk_widget_destroy (dialog);
}

void reset_rotation (GtkWidget *widget, gpointer data) {
	viewport->reset_rotation();
}

void reset_position (GtkWidget *widget, gpointer data) {
	viewport->reset_position();
}

/* ADD_POINT_W */
void on_add_point_clicked (GtkWidget *widget, gpointer data) {
	const gchar* name = gtk_entry_get_text(name_point_entry);
	double x1 = atof(gtk_entry_get_text(x_point_entry));
	double y1 = atof(gtk_entry_get_text(y_point_entry));
	double z1 = atof(gtk_entry_get_text(z_point_entry));

	fill_treeview(name,"Point");
	Point* point = new Point(name, x1, y1, z1);
	viewport->addObject(point);

    gtk_entry_set_text(name_point_entry,"");
    gtk_entry_set_text(x_point_entry,"");
    gtk_entry_set_text(y_point_entry,"");
    gtk_entry_set_text(z_point_entry,"");
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

void on_add_point_face_clicked (GtkWidget *widget, gpointer data) {
	double x1 = atof(gtk_entry_get_text(x_face_entry));
	double y1 = atof(gtk_entry_get_text(y_face_entry));
	double z1 = atof(gtk_entry_get_text(z_face_entry));
	face_coords.push_back(Coordinate(x1,y1,z1));
    gtk_entry_set_text(x_face_entry,"");
    gtk_entry_set_text(y_face_entry,"");
    gtk_entry_set_text(z_face_entry,"");
}

void on_add_poly_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_poly_entry);
	Polygon* polygon = new Polygon(name, polygon_coords, gtk_toggle_button_get_active(filled_poly_checkbox));
    fill_treeview(name,"Polygon");
    viewport->addObject(polygon);
	polygon_coords.clear();
    gtk_entry_set_text(name_poly_entry, "");

}
/* ADD CURVE */
void on_add_point_curve_clicked (GtkWidget *widget, gpointer data) {
    double x1 = atof(gtk_entry_get_text(x_curve_entry));
    double y1 = atof(gtk_entry_get_text(y_curve_entry));
    double z1 = atof(gtk_entry_get_text(z_curve_entry));
    curve_coords.push_back(Coordinate(x1,y1,z1));
    int n = atoi(gtk_label_get_text(control_point_number_label));
    //char* s;
    //itoa(++n, s, 10);
    char buf[10];
    sprintf(buf, "  %d  ", ++n);

    gtk_label_set_text(control_point_number_label, buf);
    gtk_entry_set_text(x_curve_entry, "");
    gtk_entry_set_text(y_curve_entry, "");
    gtk_entry_set_text(z_curve_entry, "");
}

void on_add_curve_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_curve_entry);
  if (gtk_toggle_button_get_active(curve_bs_checkbox)){
        fill_treeview(name,"B-Spline Curve");
        BsplineCurve* curve = new BsplineCurve(name, curve_coords);
        viewport->addObject(curve);
        curve_coords.clear();
  } else {
        fill_treeview(name,"Bezier Curve");
        BezierCurve* curve = new BezierCurve(name, curve_coords);
        viewport->addObject(curve);
        curve_coords.clear();
  }


    gtk_label_set_text(control_point_number_label, "0");
    gtk_entry_set_text(x_curve_entry, "");
    gtk_entry_set_text(y_curve_entry, "");
    gtk_entry_set_text(z_curve_entry, "");
    gtk_entry_set_text(name_curve_entry, "");
}

void on_add_face_clicked (GtkWidget *widget, gpointer data) {
    const gchar* name = "Face";
	Polygon* polygon = new Polygon(name, face_coords, gtk_toggle_button_get_active(filled_face_checkbox));
    faces_object3D.push_back(*polygon);
	face_coords.clear();
}

void on_add_object3D_clicked (GtkWidget *widget, gpointer data) {
    const gchar* name = gtk_entry_get_text(name_object3D_entry);
    Object3D* object = new Object3D(name, faces_object3D);
    fill_treeview(name,"Object3D");
    faces_object3D.clear();
    viewport->addObject(object);
}

void on_add_point_surface_clicked (GtkWidget *widget, gpointer data) {
    double x1 = atof(gtk_entry_get_text(x_surface_entry));
    double y1 = atof(gtk_entry_get_text(y_surface_entry));
    double z1 = atof(gtk_entry_get_text(z_surface_entry));

    if (surface_point_number < columns_s*rows_s) {
        surface_point_number++;

        Coordinate coord = Coordinate(x1,y1,z1);
        surface_coords.push_back(coord);

        char point[64];
        sprintf(point, "(%.1f,%.1f,%.1f)",x1,y1,z1);
        char next[64];
        if(surface_point_number == columns_s*rows_s) {
            sprintf(next, "Completed");
        } else {
            sprintf(next, "(%d,%d)",surface_point_number/columns_s+1,surface_point_number%columns_s+1);
        }
        gtk_label_set_text(label_current_index, next);
        gtk_label_set_text(label_previous_point, point);
    }

    gtk_entry_set_text(x_surface_entry, "");
    gtk_entry_set_text(y_surface_entry, "");
    gtk_entry_set_text(z_surface_entry, "");
}

void on_add_surface_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_surface_entry);
  if(surface_coords.size() == rows_s*columns_s) {
	  if (gtk_toggle_button_get_active(surface_bs_checkbox)){
	        fill_treeview(name,"B-Spline Surface");
	        BSplineSurface* surface = new BSplineSurface(name, surface_coords);
	        viewport->addObject(surface);
	        surface_coords.clear();
            surface_point_number = 0;
	  } else if (gtk_toggle_button_get_active(surface_be_checkbox)) {
	        fill_treeview(name,"Bezier Surface");
	        BezierSurface* surface = new BezierSurface(name, surface_coords);
	        viewport->addObject(surface);
	        surface_coords.clear();
            surface_point_number = 0;
	  }

	    gtk_entry_set_text(x_surface_entry, "");
	    gtk_entry_set_text(y_surface_entry, "");
	    gtk_entry_set_text(z_surface_entry, "");
	    gtk_entry_set_text(name_surface_entry, "");
	    gtk_label_set_text(label_current_index, "(1,1)");
	    gtk_label_set_text(label_previous_point, "");
  }
}

/* Buttons Change object */
void on_angle_world_button_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_world_entry));
	if (gtk_toggle_button_get_active(x_checkr)) {
	    accumulator.push_back(Transformation::generate_rx(angle,false));
	} else if (gtk_toggle_button_get_active(y_checkr)) {
	    accumulator.push_back(Transformation::generate_ry(angle,false));
	} else if (gtk_toggle_button_get_active(z_checkr)) {
	    accumulator.push_back(Transformation::generate_rz(angle,false));
	}
}

void on_angle_obj_button_clicked(GtkWidget *widget, gpointer data) {
	int index = get_index_selected();
	Coordinate center = (viewport->getObject(index))->get_center_coord();
	double angle = atof(gtk_entry_get_text(angle_obj_entry));

	accumulator.push_back(Transformation::generate_translation_matrix(-center[0], -center[1], -center[2]));
	if (gtk_toggle_button_get_active(x_checkr)) {
	    accumulator.push_back(Transformation::generate_rx(angle,false));
	} else if (gtk_toggle_button_get_active(y_checkr)) {
	    accumulator.push_back(Transformation::generate_ry(angle,false));
	} else if (gtk_toggle_button_get_active(z_checkr)) {
	    accumulator.push_back(Transformation::generate_rz(angle,false));
	}
	accumulator.push_back(Transformation::generate_translation_matrix(center[0], center[1], center[2]));

}

void on_translate_button_clicked (GtkWidget *widget, gpointer data) {
	double dx = atof(gtk_entry_get_text(trans_x_entry));
	double dy = atof(gtk_entry_get_text(trans_y_entry));
	double dz = atof(gtk_entry_get_text(trans_z_entry));
	accumulator.push_back(Transformation::generate_translation_matrix(dx, dy, dz));
}

void on_rotate_point_button_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_point_entry));
	double x = atof(gtk_entry_get_text(angle_pointx_entry));
	double y = atof(gtk_entry_get_text(angle_pointy_entry));
	double z = atof(gtk_entry_get_text(angle_pointz_entry));
	double xv = atof(gtk_entry_get_text(angle_vectorx_entry));
	double yv = atof(gtk_entry_get_text(angle_vectory_entry));
	double zv = atof(gtk_entry_get_text(angle_vectorz_entry));

	Coordinate p(x, y, z);
	Coordinate v(xv, yv, zv);
	accumulator.push_back(Transformation::generate_ra(angle, p, v));

}
//ok
void on_schedule_button_clicked (GtkWidget *widget, gpointer data) {
	double sx = atof(gtk_entry_get_text(sx_entry));
	double sy = atof(gtk_entry_get_text(sy_entry));
	double sz = atof(gtk_entry_get_text(sz_entry));

	int index = get_index_selected();
	Coordinate center = (viewport->getObject(index))->get_center_coord();
	accumulator.push_back(Transformation::generate_scaling_around_obj_center_matrix(sx, sy, sz, center));
}

void on_change_obj_button_clicked (GtkWidget *widget, gpointer data) {
	//std::string name = get_name_selected();
	//aqui será onde será feita a multiplicação da matriz final no objeto com nome name
	Matrix identity = { {1, 0, 0, 0},
	         			{0, 1, 0, 0},
	         			{0, 0, 1, 0},
	         			{0, 0, 0, 1} };
	Transformation id = Transformation(identity);
	for(int i=0; i < accumulator.size(); i++){
		id *= accumulator.at(i);
	}
	Object* obj = viewport->getObject(get_index_selected());
	obj->transform_coords(id);
	viewport->normalize_and_clip_obj(obj);
	accumulator.clear();
}

void on_settings_button_clicked (GtkWidget *widget, gpointer data) {
    gtk_widget_show (GTK_WIDGET(settings_w));
}


void on_surface_spinbutton_clicked (GtkWidget *widget, gpointer data) {
        surface_coords.clear();
        surface_point_number = 0;
        rows_s = gtk_spin_button_get_value_as_int(surface_rows_spinbutton);
        columns_s = gtk_spin_button_get_value_as_int(surface_columns_spinbutton);
	    gtk_entry_set_text(name_surface_entry, "");
	    gtk_entry_set_text(x_surface_entry, "");
	    gtk_entry_set_text(y_surface_entry, "");
	    gtk_entry_set_text(z_surface_entry, "");
	    gtk_label_set_text(label_current_index, "(1,1)");
	    gtk_label_set_text(label_previous_point, "");
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

void fov_scale_event(){
    //std::cout << gtk_adjustment_get_value (fov_scale)<< std::endl;
    viewport->set_focal_distance(gtk_adjustment_get_value (fov_scale)*PI/180);
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

void check() {
    if (gtk_toggle_button_get_active(CS_Clipping)){
        gtk_toggle_button_set_active(LB_Clipping, false);
        viewport->changeLineClipAlg(Line_clip_algs::CS);
    } else if(gtk_toggle_button_get_active(LB_Clipping)) {
        gtk_toggle_button_set_active(CS_Clipping, false);
        viewport->changeLineClipAlg(Line_clip_algs::LB);
    } else
        viewport->changeLineClipAlg(Line_clip_algs::NC);
}

void check_parallel_event() {
	if (gtk_toggle_button_get_active(check_parallel)) {
        gtk_toggle_button_set_active(check_perspective, false);
        viewport->change_view(window_view::PARALLEL);
    }
}

void check_perspective_event() {
	if (gtk_toggle_button_get_active(check_perspective)) {
        gtk_toggle_button_set_active(check_parallel, false);
        viewport->change_view(window_view::PERSPECTIVE);
    }
}
void check_x() {
    if (gtk_toggle_button_get_active(x_check)) {
        gtk_toggle_button_set_active(y_check, false);
        gtk_toggle_button_set_active(z_check, false);
    }
}

void check_y() {
    if (gtk_toggle_button_get_active(y_check)) {
        gtk_toggle_button_set_active(x_check, false);
        gtk_toggle_button_set_active(z_check, false);
    }
}

void check_z() {
    if (gtk_toggle_button_get_active(z_check)) {
        gtk_toggle_button_set_active(x_check, false);
        gtk_toggle_button_set_active(y_check, false);
    }
}

void check_xr() {
    if (gtk_toggle_button_get_active(x_checkr)) {
        gtk_toggle_button_set_active(y_checkr, false);
        gtk_toggle_button_set_active(z_checkr, false);
    }
}

void check_yr() {
    if (gtk_toggle_button_get_active(y_checkr)) {
        gtk_toggle_button_set_active(x_checkr, false);
        gtk_toggle_button_set_active(z_checkr, false);
    }
}

void check_zr() {
    if (gtk_toggle_button_get_active(z_checkr)) {
        gtk_toggle_button_set_active(x_checkr, false);
        gtk_toggle_button_set_active(y_checkr, false);
    }
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

int main (int argc, char *argv[]) {
    /* ---------- Init GTK ---------- */
	gtk_init (&argc, &argv);

    /* ---------- Load Glade UI ---------- */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "Interface.glade", NULL);

    /* ---------- Main Window ---------- */

	/* Init Window */
	main_w = gtk_builder_get_object (builder, "main_w");
	g_signal_connect (main_w, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_resizable (GTK_WINDOW(main_w),  false);

    /* Init Viewport */
	viewport = new Viewport(510,515);
	draw_viewport = GTK_WIDGET(gtk_builder_get_object(builder, "draw_viewport"));
	g_signal_connect(draw_viewport, "draw", G_CALLBACK(draw_objects), NULL);

	/* Init Treeview*/
	objects_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "object_tree"));
	create_treeview();

    /* Menu Items */
   /* File */
	open_file_m = GTK_MENU_ITEM(gtk_builder_get_object(builder, "file_open_menu_item"));
	g_signal_connect (open_file_m, "activate", G_CALLBACK (open_file), NULL);
	save_file_m = GTK_MENU_ITEM(gtk_builder_get_object(builder, "file_save_menu_item"));
	g_signal_connect (save_file_m, "activate", G_CALLBACK (save_file), NULL);
   /* View */
	reset_rotation_m = GTK_MENU_ITEM(gtk_builder_get_object(builder, "reset_rotation_menu_item"));
	g_signal_connect (reset_rotation_m, "activate", G_CALLBACK (reset_rotation), NULL);
	reset_position_m = GTK_MENU_ITEM(gtk_builder_get_object(builder, "reset_position_menu_item"));
	g_signal_connect (reset_position_m, "activate", G_CALLBACK (reset_position), NULL);

    /* Controls */
    zoom_in_button = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_in"));
    g_signal_connect (zoom_in_button, "clicked", G_CALLBACK (on_zoom_in_button_clicked), NULL);

    zoom_out_button = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_out"));
    g_signal_connect (zoom_out_button, "clicked", G_CALLBACK (on_zoom_out_button_clicked), NULL);

    move_down_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_down"));
    g_signal_connect (move_down_button, "clicked", G_CALLBACK (on_down_button_clicked), NULL);

    move_up_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_up"));
    g_signal_connect (move_up_button, "clicked", G_CALLBACK (on_up_button_clicked), NULL);

    move_right_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_right"));
    g_signal_connect (move_right_button, "clicked", G_CALLBACK (on_right_button_clicked), NULL);

    move_left_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_left"));
    g_signal_connect (move_left_button, "clicked", G_CALLBACK (on_left_button_clicked), NULL);

    move_back_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_back"));
    g_signal_connect (move_back_button, "clicked", G_CALLBACK (on_back_button_clicked), NULL);

    move_forward_button = GTK_BUTTON(gtk_builder_get_object(builder, "move_forward"));
    g_signal_connect (move_forward_button, "clicked", G_CALLBACK (on_forward_button_clicked), NULL);

    add_object_button = GTK_BUTTON(gtk_builder_get_object(builder, "add_object_button"));
    g_signal_connect (add_object_button, "clicked", G_CALLBACK (on_add_object_button_clicked), NULL);

    change_object_button = GTK_BUTTON(gtk_builder_get_object(builder, "change_object"));
    g_signal_connect (change_object_button, "clicked", G_CALLBACK (on_change_object_clicked), NULL);

    rotate_left_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_left"));
    g_signal_connect (rotate_left_button, "clicked", G_CALLBACK (on_rotate_left_clicked), NULL);

    rotate_right_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_right"));
    g_signal_connect (rotate_right_button, "clicked", G_CALLBACK (on_rotate_right_clicked), NULL);

    settings_button = GTK_BUTTON(gtk_builder_get_object(builder, "settings_button"));
    g_signal_connect (settings_button, "clicked", G_CALLBACK (on_settings_button_clicked), NULL);

    step_entry = GTK_ENTRY(gtk_builder_get_object(builder, "step_entry"));
    angle_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_entry"));

    /* ---------- Settings Window ---------- */
	settings_w = gtk_builder_get_object (builder, "settings_w");
	g_signal_connect (settings_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

    /* Projection Settings */
	check_parallel = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"projection_parallel_checkbox"));
    g_signal_connect(check_parallel, "toggled", G_CALLBACK(check_parallel_event), NULL);
    check_perspective = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"projection_perspective_checkbox"));
    g_signal_connect(check_perspective, "toggled", G_CALLBACK(check_perspective_event), NULL);
    /* FOV */
	fov_scale = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "projection_fov_scale"));
    g_signal_connect(fov_scale, "value-changed", G_CALLBACK(fov_scale_event), NULL);
    /* Clipping */
    CS_Clipping = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"line_clipping_cs_checkbox"));
    g_signal_connect(CS_Clipping, "toggled", G_CALLBACK(check), NULL);

    LB_Clipping = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"line_clipping_lb_checkbox"));
    g_signal_connect(LB_Clipping, "toggled", G_CALLBACK(check), NULL);

    /* ---------- Add Object Window ---------- */
    /* Init Add Object Window */
	add_object_w = gtk_builder_get_object (builder, "add_object_w");
	g_signal_connect (add_object_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

    /* Point */
	add_point_button = GTK_BUTTON(gtk_builder_get_object(builder, "point_add_button"));
	g_signal_connect (add_point_button, "clicked", G_CALLBACK (on_add_point_clicked), NULL);

    name_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_name_text_entry"));
	x_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_x_coord_text_entry"));
	y_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_y_coord_text_entry"));
	z_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_z_coord_text_entry"));

    /* Line */
	add_line_button = GTK_BUTTON(gtk_builder_get_object(builder, "line_add_button"));
	g_signal_connect (add_line_button, "clicked", G_CALLBACK (on_add_line_clicked), NULL);

    name_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "line_name_text_entry"));
	x1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_x1_coord_text_entry"));
	y1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_y1_coord_text_entry"));
	z1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_z1_coord_text_entry"));
	x2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_x2_coord_text_entry"));
	y2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_y2_coord_text_entry"));
	z2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "point_z2_coord_text_entry"));

    /* Polygon */
	add_point_poly_button = GTK_BUTTON(gtk_builder_get_object(builder, "polygon_add_point_button"));
	g_signal_connect (add_point_poly_button, "clicked", G_CALLBACK (on_add_point_poly_clicked), NULL);

	add_poly_button = GTK_BUTTON(gtk_builder_get_object(builder, "polygon_add_button"));
	g_signal_connect (add_poly_button, "clicked", G_CALLBACK (on_add_poly_clicked), NULL);

    name_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "polygon_name_text_entry"));
	x_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "polygon_x_coord_text_entry"));
	y_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "polygon_y_coord_text_entry"));
	z_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "polygon_z_coord_text_entry"));

    filled_poly_checkbox = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"polygon_fill_checkbutton"));

    /* Curve */
    add_point_curve_button = GTK_BUTTON(gtk_builder_get_object(builder, "curve_add_point_button"));
    g_signal_connect (add_point_curve_button, "clicked", G_CALLBACK (on_add_point_curve_clicked), NULL);

    add_curve_button = GTK_BUTTON(gtk_builder_get_object(builder, "curve_add_button"));
    g_signal_connect (add_curve_button, "clicked", G_CALLBACK (on_add_curve_clicked), NULL);

    name_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "curve_name_text_entry"));
    x_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "curve_x_coord_text_entry"));
    y_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "curve_y_coord_text_entry"));
    z_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "curve_z_coord_text_entry"));

    curve_bs_checkbox = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"curve_bspline_checkbutton"));
    control_point_number_label = GTK_LABEL(gtk_builder_get_object(builder, "curve_controlpoints_label"));

    /* 3D Object */
    add_point_face_button = GTK_BUTTON(gtk_builder_get_object(builder, "object3d_add_point_button"));
	g_signal_connect (add_point_face_button, "clicked", G_CALLBACK (on_add_point_face_clicked), NULL);

    add_face_button = GTK_BUTTON(gtk_builder_get_object(builder, "object3d_add_face_button"));
    g_signal_connect (add_face_button, "clicked", G_CALLBACK (on_add_face_clicked), NULL);

    add_object3D_button = GTK_BUTTON(gtk_builder_get_object(builder, "object3d_add_button"));
    g_signal_connect (add_object3D_button, "clicked", G_CALLBACK (on_add_object3D_clicked), NULL);

    x_face_entry = GTK_ENTRY(gtk_builder_get_object(builder, "object3d_x_coord_text_entry"));
    y_face_entry = GTK_ENTRY(gtk_builder_get_object(builder, "object3d_y_coord_text_entry"));
    z_face_entry = GTK_ENTRY(gtk_builder_get_object(builder, "object3d_z_coord_text_entry"));
    name_object3D_entry = GTK_ENTRY(gtk_builder_get_object(builder, "object3d_name_text_entry"));

    filled_object3D_checkbox = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"object3d_fill_checkbutton"));

    /* Surface */
    add_point_surface_button = GTK_BUTTON(gtk_builder_get_object(builder, "surface_addpoint_button"));
    g_signal_connect (add_point_surface_button, "clicked", G_CALLBACK (on_add_point_surface_clicked), NULL);

    add_surface_button = GTK_BUTTON(gtk_builder_get_object(builder, "surface_add_button"));
    g_signal_connect (add_surface_button, "clicked", G_CALLBACK (on_add_surface_clicked), NULL);

    name_surface_entry = GTK_ENTRY(gtk_builder_get_object(builder, "surface_name_text_entry"));
    x_surface_entry = GTK_ENTRY(gtk_builder_get_object(builder, "surface_x_coord_text_entry"));
    y_surface_entry = GTK_ENTRY(gtk_builder_get_object(builder, "surface_y_coord_text_entry"));
    z_surface_entry = GTK_ENTRY(gtk_builder_get_object(builder, "surface_z_coord_text_entry"));
    surface_bs_checkbox = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"surface_BS_checkbutton"));
    surface_be_checkbox = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"surface_BE_checkbutton"));

    surface_rows_spinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"surface_rows_spinbutton"));
    g_signal_connect (surface_rows_spinbutton, "value-changed", G_CALLBACK (on_surface_spinbutton_clicked), NULL);
    surface_columns_spinbutton = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"surface_columns_spinbutton"));
    g_signal_connect (surface_columns_spinbutton, "value-changed", G_CALLBACK (on_surface_spinbutton_clicked), NULL);

    label_current_index = GTK_LABEL(gtk_builder_get_object(builder, "surface_currindex_number_label"));
    label_previous_point = GTK_LABEL(gtk_builder_get_object(builder, "surface_previndex_number_label"));

	/* Buttons Change object */
	change_obj_w = gtk_builder_get_object (builder, "change_obj_w");
	g_signal_connect (change_obj_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

	angle_world_button = GTK_BUTTON(gtk_builder_get_object(builder, "angle_world_button"));
	g_signal_connect (angle_world_button  , "clicked", G_CALLBACK (on_angle_world_button_clicked), NULL);

	angle_obj_button = GTK_BUTTON(gtk_builder_get_object(builder, "angle_obj_button"));
	g_signal_connect (angle_obj_button, "clicked", G_CALLBACK (on_angle_obj_button_clicked), NULL);

	translate_button = GTK_BUTTON(gtk_builder_get_object(builder, "translate_button"));
	g_signal_connect (translate_button, "clicked", G_CALLBACK (on_translate_button_clicked), NULL);

	rotate_point_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_point_button"));
	g_signal_connect (rotate_point_button, "clicked", G_CALLBACK (on_rotate_point_button_clicked), NULL);

	schedule_button = GTK_BUTTON(gtk_builder_get_object(builder, "schedule_button"));
	g_signal_connect (schedule_button, "clicked", G_CALLBACK (on_schedule_button_clicked), NULL);

	change_obj_button = GTK_BUTTON(gtk_builder_get_object(builder, "change_obj_button"));
	g_signal_connect (change_obj_button, "clicked", G_CALLBACK (on_change_obj_button_clicked), NULL);

	/* Connecting Entry*/
	angle_world_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_world_entry"));
	angle_obj_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_obj_entry"));
	trans_x_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_x_entry"));
	trans_y_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_y_entry"));
	trans_z_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_z_entry"));
	angle_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_point_entry"));
	angle_pointx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointx_entry"));
	angle_pointy_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointy_entry"));
	angle_pointz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointz_entry"));
	angle_vectorx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectorx_entry"));
	angle_vectory_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectory_entry"));
	angle_vectorz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectorz_entry"));
	sx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sx_entry"));
	sy_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sy_entry"));
	sz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sz_entry"));
    x_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"x_check"));
    y_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"y_check"));
    z_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"z_check"));
    x_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"x_checkr"));
    y_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"y_checkr"));
    z_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"z_checkr"));
    g_signal_connect(x_check, "toggled", G_CALLBACK(check_x), NULL);
    g_signal_connect(y_check, "toggled", G_CALLBACK(check_y), NULL);
    g_signal_connect(z_check, "toggled", G_CALLBACK(check_z), NULL);
    g_signal_connect(x_checkr, "toggled", G_CALLBACK(check_xr), NULL);
    g_signal_connect(y_checkr, "toggled", G_CALLBACK(check_yr), NULL);
    g_signal_connect(z_checkr, "toggled", G_CALLBACK(check_zr), NULL);

	gtk_widget_show(GTK_WIDGET(main_w));

	gtk_main ();

	return 0;
}
