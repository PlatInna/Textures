// Textures.cpp : https://www.coursera.org/ C++ Development Fundamentals: Brown Belt, Week 4.
// Task: Implement shape classes that can be drawn on an image and that jointly own a given texture.
//

#include "Common.h"
#include <stdexcept>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape {
protected:
	Point _shape_start_position_;
	Size _shape_size_;
	shared_ptr<ITexture> _shrd_ptr_texture_;

public:
	// Возвращает точную копию фигуры.
	// Если фигура содержит текстуру, то созданная копия содержит ту же самую
	// текстуру. Фигура и её копия совместно владеют этой текстурой.
	
	//std::unique_ptr<IShape> Clone() const override {}

	void SetPosition(Point point) final {
		_shape_start_position_.x = point.x;
		_shape_start_position_.y = point.y;
	}
	Point GetPosition() const final {
		return _shape_start_position_;
	}

	void SetSize(Size size) final {
		_shape_size_ = size;
	}
	Size GetSize() const final {
		return _shape_size_;
	}

	void SetTexture(std::shared_ptr<ITexture> shrd_ptr) final {
		_shrd_ptr_texture_ = shrd_ptr;
	}
	
	ITexture* GetTexture() const final {
		return _shrd_ptr_texture_.get();
	}

	virtual bool IsPointInShape(Point p, Size size) const = 0;

	bool IsPointInTexture(Point p, Size size) const {
		return  p.x < size.width and p.y < size.height;
	}

	// Рисует фигуру на указанном изображении
	void Draw(Image& image) const override {

		Image texture_image;
		Size texture_size;

		if (_shrd_ptr_texture_) {
			texture_image = _shrd_ptr_texture_->GetImage();
			texture_size = _shrd_ptr_texture_->GetSize();
		}
		else {
			texture_image = {};
			texture_size = { 0,0 };
		}

		for (int y = _shape_start_position_.y; y < _shape_start_position_.y + _shape_size_.height; ++y) {
			for (int x = _shape_start_position_.x; x < _shape_start_position_.x + _shape_size_.width; ++x) {
				if (IsPointInShape(Point{ x - _shape_start_position_.x, y - _shape_start_position_.y }, _shape_size_)) {
					if (IsPointInTexture(Point{ x - _shape_start_position_.x, y - _shape_start_position_.y }, texture_size)) {
						image[y][x] = texture_image[y - _shape_start_position_.y][x - _shape_start_position_.x];
					}
					else {
						image[y][x] = '.';
					}
				}
			}
		}
	}
};

class RectangleShape : public Shape {
	bool IsPointInShape(Point p, Size size) const override {
		return p.x < size.width and p.y < size.height;
	}

	unique_ptr<IShape> Clone() const override {
		auto clone_shape = make_unique<RectangleShape>(*this);
		return clone_shape;
	}
};

class EllipseShape : public Shape {
	bool IsPointInShape(Point p, Size size) const override {
		return IsPointInEllipse(p, size);
	}

	unique_ptr<IShape> Clone() const override {
		auto clone_shape = make_unique<EllipseShape>(*this);
		return clone_shape;
	}
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	if (shape_type == ShapeType::Ellipse) {
		return make_unique<EllipseShape>();
	}
	else if (shape_type == ShapeType::Rectangle) {
		return make_unique<RectangleShape>();
	}
	else {
		throw logic_error("Invalid shape_type");
	}
}
