#include <iostream>
#include <vector>
#include <algorithm>

// The component interface declares common operations for both
// simple and complex objects of a composition.
class Graphic
{
public:
    virtual ~Graphic() = default;
    virtual void move(int x, int y) = 0;
    virtual void draw() const = 0;
};

// The leaf class represents end objects of a composition. A
// leaf object can't have any sub-objects. Usually, it's leaf
// objects that do the actual work, while composite objects only
// delegate to their sub-components.
class Dot : public Graphic
{
protected:
    int x, y;

public:
    Dot(int x, int y) : x(x), y(y) {}

    void move(int dx, int dy) override
    {
        x += dx;
        y += dy;
    }

    void draw() const override
    {
        std::cout << "Dot at (" << x << ", " << y << ")\n";
    }
};

// All component classes can extend other components.
class Circle : public Dot
{
    int radius;

public:
    Circle(int x, int y, int radius) : Dot(x, y), radius(radius) {}

    void draw() const override
    {
        std::cout << "Circle at (" << x << ", " << y << ") with radius " << radius << "\n";
    }
};

// The composite class represents complex components that may
// have children. Composite objects usually delegate the actual
// work to their children and then "sum up" the result.
class CompoundGraphic : public Graphic
{
    std::vector<Graphic *> children;

public:
    ~CompoundGraphic()
    {
        for (Graphic *child : children)
        {
            delete child;
        }
    }

    void add(Graphic *child)
    {
        children.push_back(child);
    }

    void remove(Graphic *child)
    {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void move(int x, int y) override
    {
        for (Graphic *child : children)
        {
            child->move(x, y);
        }
    }

    void draw() const override
    {
        for (const Graphic *child : children)
        {
            child->draw();
        }
        // Draw bounding rectangle (example implementation)
        std::cout << "Draw bounding rectangle\n";
    }
};

// The client code works with all the components via their base
// interface. This way the client code can support simple leaf
// components as well as complex composites.
class ImageEditor
{
    CompoundGraphic *all;

public:
    ~ImageEditor()
    {
        delete all;
    }

    void load()
    {
        all = new CompoundGraphic();
        all->add(new Dot(1, 2));
        all->add(new Circle(5, 3, 10));
        // ...
    }

    // Combine selected components into one complex composite
    // component.
    void groupSelected(const std::vector<Graphic *> &components)
    {
        CompoundGraphic *group = new CompoundGraphic();
        for (Graphic *component : components)
        {
            group->add(component);
            all->remove(component);
        }
        all->add(group);
        // All components will be drawn.
        all->draw();
    }
};

// Example usage
int main()
{
    ImageEditor editor;
    editor.load();
    std::vector<Graphic *> components = {new Dot(1, 2), new Circle(5, 3, 10)};
    editor.groupSelected(components);
    return 0;
}
