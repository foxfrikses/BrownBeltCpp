#include "Common.h"

using namespace std;

Size GetImageSize(const Image& image) {
  if (image.empty()) {
    return {0, 0};
  }
  
  return {static_cast<int>(image.front().size()),
          static_cast<int>(image.size())};
}

namespace Shapes
{
  class IDefault : public IShape {
  public:
    IDefault() = default;
    ~IDefault() override = default;
  
    void SetPosition(Point position) override {
      position_ = position;
    }
    Point GetPosition() const override {
      return position_;
    }
  
    void SetSize(Size size) override {
      size_ = size;
    }
    Size GetSize() const override {
      return size_;
    }
  
    void SetTexture(std::shared_ptr<ITexture> texture) override {
      texture_ = move(texture);
    }
    ITexture* GetTexture() const override {
      return texture_.get();
    }
    
    virtual bool IsInShape(Point) const = 0;
  
    void Draw(Image& image) const override {
      const auto image_size = GetImageSize(image);
      const auto texture_size =
          (texture_ ? GetImageSize(texture_.get()->GetImage()) : Size{0, 0});
      
      const Point begin { position_.x < 0 ? -position_.x : 0,
                          position_.y < 0 ? -position_.y : 0};
      const Point end {min(image_size.width - position_.x, size_.width),
                       min(image_size.height - position_.y, size_.height)};
    
      Point p;
      for (p.y = begin.y; p.y < end.y; ++p.y) {
        for (p.x = begin.x; p.x < end.x; ++p.x) {
          if (!IsInShape(p)) continue;
          
          char c = '.';
          if (p.y < texture_size.height && p.x < texture_size.width) {
            c = texture_.get()->GetImage()[p.y][p.x];
          }
          image[position_.y + p.y][position_.x + p.x] = c;
        }
      }
    }

  protected:
    Size size_ {0, 0};
    Point position_ {0, 0};
    std::shared_ptr<ITexture> texture_;
  };
  
  class Rectangle : public IDefault {
  public:
    Rectangle() = default;
    ~Rectangle() override = default;
  
    std::unique_ptr<IShape> Clone() const override {
      return std::make_unique<Rectangle>(*this);
    }
  
    bool IsInShape(Point p) const override {
      return p.x >= 0 && p.x < size_.width && p.y >= 0 && p.y < size_.height;
    }
  };
  
  class Ellipse : public IDefault {
  public:
    Ellipse() = default;
    ~Ellipse() override = default;
    
    std::unique_ptr<IShape> Clone() const override {
      return std::make_unique<Ellipse>(*this);
    }
  
    bool IsInShape(Point p) const override {
      return IsPointInEllipse(p, size_);
    }
  };
}

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  switch (shape_type) {
    case ShapeType::Rectangle:
      return make_unique<Shapes::Rectangle>();
  
    case ShapeType::Ellipse:
      return make_unique<Shapes::Ellipse>();
  
    default:
      return nullptr;
  }
}