#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>
#include "Coordinate.hpp"
#include "Transformation.hpp"

class Window {
	public:
		Window(double width, double height):
			_center(width/2, height/2),
			_width(width),
			_heigth(height),
			_t({ {1, 0, 0},
				 {0, 1, 0},
				 {0, 0, 1} })
		{}

		virtual ~Window() {}

		double get_width() { return _width; }
		double get_height() { return _heigth; }

		double get_angle() { return _angle; }

		void rotate(double degrees) { _angle += degrees; }

		void zoom(double step);

		void moveX(double value);
		void moveY(double value);

		Coordinate lowmin() const { return Coordinate(-1,-1); }
		Coordinate uppermax() const { return Coordinate(1,1); }
		Coordinate center() const { return _center; }

		Transformation& get_transformation() { return _t; }	 	  	 	     	   	      	     	  	     	  	 	
		void update_transformation();

	protected:
	private:
		void move(double x, double y) {
			Coordinate c(x,y);
			auto t = Transformation::generate_rotation_matrix(_angle);
			c.transform(t.get_transformation_matrix());
			_center += c;
		}

		Coordinate _center;
		double _angle = 0; // radians
		double _width, _heigth;
		double _d = 1000;
		Transformation _t;
};

/* step > 0  -  Zoom in */
void Window::zoom(double step) {
	_width -= step;
	_heigth -= step;
}

/* Move Window Horizontally */
void Window::moveX(double value) {
	move(value, 0);
}

/* Move Window Vertically */
void Window::moveY(double value) {
	move(0, value);
}

void Window::update_transformation() {
	_t = Transformation({ {1, 0, 0},
						  {0, 1, 0},
						  {0, 0, 1} });

	_t *= Transformation::generate_translation_matrix(-_center[0], -_center[1]);
	_t *= Transformation::generate_rotation_matrix(-_angle);
	_t *= Transformation::generate_scaling_matrix(1/(_width/2), 1/(_heigth/2));
}	 	  	 	     	   	      	     	  	     	  	 	

#endif
