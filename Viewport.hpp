#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "Window.hpp"
#include "objects.hpp"
#include "ListaEnc.hpp"
#include "elemento.hpp"
#include "clipping.hpp"

class Viewport {
	public:
		Viewport(double width, double height):
			_width(width),
			_height(height),
			_window(new Window(width,height)),
			_clipper(-1,1,-1,1)
		{
			normalize_and_clip_all_objs();
		}
		virtual ~Viewport() {}

		void zoom(double step);
		void moveX(double value);
		void moveY(double value);
		void moveZ(double value);
		void change_view(const window_view view);
		void rotate_window(double degrees);

		void drawDisplayFile(cairo_t* cr);
		void addObject(Object* obj) { _objetos.adiciona(obj); normalize_and_clip_obj(obj); };
		Object* getObject(int index) { _objetos.retornaDaPosicao(index);};
		int get_display_file_size() { return _objetos.tamanho(); };

	protected:
	private:
		Window* _window;
		double _width, _height;
		ListaEnc<Object*> _objetos;

		void drawPoint(Object* objeto, cairo_t* cr);
		void drawLine(Object* objeto, cairo_t* cr);
		void drawPolygon(Object* objeto, cairo_t* cr);
};

void Viewport::zoom(double step) {
	_window->zoom(step);
}

void Viewport::moveX(double step) {
	_window->moveX(step);
}

void Viewport::moveY(double step){
	_window->moveY(step);
}

void Viewport::moveZ(double step){
	_window->moveZ(step);
}

void Viewport::rotate_window(double degrees) {
	_window->rotate(degrees);
}

void Viewport::drawPoint(Object* objeto, cairo_t* cr) {
	if (objeto->get_coords().size() == 0)
		return;
	Coordinate coord = transformOneCoordinate(objeto->get_normalized_coord_at_index(0));
	//prepareContext();
	cairo_move_to(cr, coord[0]+10, coord[1]+10);
	cairo_arc(cr, coord[0]+10, coord[1]+10, 1.0, 0.0, (2*G_PI) );
	cairo_fill(cr);
}

void Viewport::drawLine(Object* objeto, cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(objeto->get_normalized_coords());
	if (transformed_vector.size() == 0)
		return;
	cairo_move_to(cr, transformed_vector[0][0]+10, transformed_vector[0][1]+10);
	cairo_line_to(cr, transformed_vector[1][0]+10, transformed_vector[1][1]+10);
	cairo_stroke(cr);
}

void Viewport::drawPolygon(Object* obj,cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(obj->get_normalized_coords());
	if (transformed_vector.size() == 0)
		return;
	cairo_move_to(cr, transformed_vector[0][0]+10, transformed_vector[0][1]+10);
	for (int i = 1; i < transformed_vector.size(); ++i)
		cairo_line_to(cr, transformed_vector[i][0]+10, transformed_vector[i][1]+10);
	cairo_line_to(cr, transformed_vector[0][0]+10, transformed_vector[0][1]+10);
	if(obj->isFilled()) {
		cairo_fill(cr);
	} else {
		cairo_stroke(cr);
	}
}

void Viewport::drawCurve(Object* obj, cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(obj->get_normalized_coords());
	if (transformed_vector.size() == 0)
		return;
	cairo_move_to(cr, transformed_vector[0][0]+10, transformed_vector[0][1]+10);
	for (int i = 1; i < transformed_vector.size(); ++i)
		cairo_line_to(cr, transformed_vector[i][0]+10, transformed_vector[i][1]+10);
	cairo_stroke(cr);

}

void Viewport::drawObj3D(Object3D* obj, cairo_t* cr) {
	for (auto &face : obj->get_face_list()) {
		if (face.get_normalized_coords().size() > 0)
			drawPolygon(&face, cr);
	}
}

void Viewport::drawSurface(Surface* obj, cairo_t* cr) {
	for (auto &curve : obj->get_curve_list()) {
		if (curve.get_normalized_coords().size() > 0) {
			drawCurve(&curve, cr);
		}
	}
}

void Viewport::drawDisplayFile(cairo_t* cr) {
	//percorrer o displayfile enviando os objetos para o respectivo draw
	for (int i = 0; i < _objetos.tamanho(); ++i) {
		auto obj = _objetos.retornaDaPosicao(i);
		switch(obj->get_type()) {
			case obj_type::OBJECT:
				break;
			case obj_type::POINT:
				drawPoint(obj, cr);
				break;
			case obj_type::LINE:
				drawLine(obj, cr);
				break;
			case obj_type::POLYGON:
				drawPolygon(obj, cr);
				break;
			case obj_type::BSPLINE_CURVE:
			case obj_type::BEZIER_CURVE:
				drawCurve(obj, cr);
				break;
			case obj_type::OBJECT_3D:
				drawObj3D((Object3D*) obj, cr);
				break;
			case obj_type::BEZIER_SURFACE:
			case obj_type::BSPLINE_SURFACE:
				drawSurface((Surface*) obj, cr);
				break;
			default:
				break;
		}
	}
}

#endif
