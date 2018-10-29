#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "coordinate.hpp"

typedef std::vector<Coordinate> Coordinates;

enum obj_type { OBJECT,
				POINT,
				LINE,
				POLYGON};

class Object {
	public:
		Object(const std::string name) :
			_name(name)
		{}

		virtual ~Object() {};

		std::string get_name() {
			return _name;
		}

		const std::string get_name() const {
			return _name;
		}

		virtual obj_type get_type() const {
			return obj_type::OBJECT;
		}

		virtual std::string get_type_name() const {
			return "Object";
		}

		Coordinates& get_coords() {
			return _coords;
		}

		const Coordinates& get_coords() const {
			return _coords;
		}

		virtual Coordinate get_center_coord() {
			Coordinate sum(3);
			for (int i = 0; i < _coords.size(); i++)
				sum += _coords[i];
			for (int i = 0; i < sum.size()-1; i++)
				sum[i] /= _coords.size();
			return sum;
		}

		bool operator==(const Object& other) {
			return this->_name == other.get_name();
		}

		Object& operator*() {
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const Object& obj) {
			os << obj.get_name() << ": [";
			for (auto i = obj._coords.begin(); i != obj._coords.end(); ++i)
				os << *i << ',';
			os << ']';
			return os;
		}

		virtual bool isFilled() { return false; }

		void add_coordinate(const Coordinate& coord) {
			_coords.push_back(coord);
		}

	protected:
		void add_coordinate(double x , double y, double z) {
			_coords.emplace_back(x,y,z);
		}

		void add_coordinate(const Coordinates& coords) {
			_coords.insert(_coords.end(), coords.begin(), coords.end());
		}
	private:
		const std::string _name;
		Coordinates _coords;
};

class Point : public Object {

	public:
		Point(std::string name, double x, double y, double z) :
			Object(name)
		{
			this->add_coordinate(x,y,z);
		}

		Point(std::string name, Coordinate coord) :
			Object(name)
		{
			this->add_coordinate(coord);
		}

		virtual ~Point() {}

		virtual obj_type get_type() const {
			return obj_type::POINT;
		}

		virtual std::string get_type_name() const {
			return "Point";
		}
	protected:
	private:
};

class Line : public Object {
	public:
		Line(std::string name,
			 double xi, double yi, double zi,
			 double xf, double yf, double zf) :
			Object(name)
		{
			this->add_coordinate(xi, yi, zi);
			this->add_coordinate(xf, yf, zf);
		}

		Line(std::string name, Coordinate initial_coord, Coordinate final_coord) :
			Object(name)
		{
			this->add_coordinate(initial_coord);
			this->add_coordinate(final_coord);
		}

		Line(std::string name, Coordinates coords) :
			Object(name)
		{
			if (coords.size() != 2) {
				throw "vector size must be 2 to construct a line";
			}
			this->add_coordinate(coords);
		}

		~Line() {}

		virtual obj_type get_type() const {
			return obj_type::LINE;
		}

		virtual std::string get_type_name() const {
			return "Line";
		}
	protected:
	private:
};

class Polygon : public Object {
	public:
		Polygon(std::string name, Coordinates coords, bool fill) :
			Object(name),
			_filled(fill)
		{
			if (coords.size() < 3) {
				throw "Polygon must have at least 3 coordinates";
			}
			this->add_coordinate(coords);
		}

		virtual ~Polygon() {}

		virtual obj_type get_type() const {
			return obj_type::POLYGON;
		}

		virtual std::string getTypeName() const {
			return "Polygon";
		}

		virtual bool isFilled() {return _filled;}
	protected:
	private:
		bool _filled;
};

#endif
