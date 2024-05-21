/*##########################################################################
System Requirements
--------------------
1. Product Management: Manage a catalog of products.
2. User Account Management: Handle user registrations and logins.
3. Shopping Cart Management: Allow users to add and remove products from their shopping cart.
4. Order Processing: Process user orders and maintain order history.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Browsing Products
2. Managing User Accounts
3. Handling Shopping Carts
4. Processing Orders
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Interface for different payment methods
class IPaymentMethod
{
public:
    virtual void processPayment(double amount) = 0;
    virtual ~IPaymentMethod() = default;
};

// Concrete implementation of Credit Card payment method
class CreditCard : public IPaymentMethod
{
public:
    void processPayment(double amount) override
    {
        std::cout << "Credit Card Payment: $" << amount << "\n";
        // Implement Credit Card payment processing logic here
    }
};

// Concrete implementation of Internet Banking payment method
class InternetBanking : public IPaymentMethod
{
public:
    void processPayment(double amount) override
    {
        std::cout << "Internet Banking Payment: $" << amount << "\n";
        // Implement Internet Banking payment processing logic here
    }
};

// Concrete implementation of PayPal payment method
class PayPal : public IPaymentMethod
{
public:
    void processPayment(double amount) override
    {
        std::cout << "PayPal Payment: $" << amount << "\n";
        // Implement PayPal payment processing logic here
    }
};

class Product
{
private:
    std::string productId;
    std::string name;
    double price;
    std::string description;

public:
    Product(const std::string &productId, const std::string &name, double price, const std::string &description)
        : productId(productId), name(name), price(price), description(description) {}

    // Getters and setters...
    const std::string &getProductId() const { return productId; }
    const std::string &getName() const { return name; }
    double getPrice() const { return price; }
    const std::string &getDescription() const { return description; }
};

class ShoppingCart;

class User
{
private:
    std::string userId;
    std::string name;
    std::string email;
    ShoppingCart *cart;
    std::vector<Order> orderHistory;

public:
    User(const std::string &userId, const std::string &name, const std::string &email)
        : userId(userId), name(name), email(email), cart(new ShoppingCart(this)) {}

    // Implementation of User methods
    void addProductToCart(Product *product, int quantity)
    {
        cart->addProduct(product, quantity);
    }

    Order checkout(IPaymentMethod *paymentMethod)
    {
        Order newOrder = cart->checkout(paymentMethod);
        orderHistory.push_back(newOrder);
        return newOrder;
    }

    // Getters and setters...
    const std::string &getUserId() const { return userId; }
    const std::string &getName() const { return name; }
    const std::string &getEmail() const { return email; }
    const ShoppingCart *getCart() const { return cart; }
    const std::vector<Order> &getOrderHistory() const { return orderHistory; }
};

class ShoppingCart
{
private:
    User *owner;
    std::unordered_map<Product *, int> items;

public:
    ShoppingCart(User *owner) : owner(owner) {}
    const User *getOwner() const { return owner; }
    const std::unordered_map<Product *, int> &getItems() const { return items; }

    // Implementation of ShoppingCart methods
    void addProduct(Product *product, int quantity)
    {
        items[product] += quantity;
    }

    Order checkout(IPaymentMethod *paymentMethod)
    {
        Order newOrder(owner, items);
        items.clear();

        // Process payment
        double totalAmount = 0.0;
        for (const auto &entry : newOrder.getOrderedItems())
        {
            totalAmount += entry.first->getPrice() * entry.second;
        }

        paymentMethod->processPayment(totalAmount);

        return newOrder;
    }
};

class Order
{
private:
    User *user;
    std::unordered_map<Product *, int> orderedItems;
    OrderStatus status;

public:
    Order(User *user, const std::unordered_map<Product *, int> &orderedItems)
        : user(user), orderedItems(orderedItems), status(OrderStatus::PROCESSING) {}

    void updateStatus(OrderStatus newStatus)
    {
        status = newStatus;
    }

    // Getters and setters...
    const User *getUser() const { return user; }
    const std::unordered_map<Product *, int> &getOrderedItems() const { return orderedItems; }
    OrderStatus getStatus() const { return status; }
};

enum class OrderStatus
{
    PROCESSING,
    SHIPPED,
    DELIVERED
};

// OnlineShoppingSystem class
class OnlineShoppingSystem
{
private:
    std::vector<Product *> catalog;
    std::vector<User *> users;

public:
    void addProductToCatalog(Product *product)
    {
        catalog.push_back(product);
    }

    void addUser(User *user)
    {
        users.push_back(user);
    }

    // Add product to a user's cart
    void addProductToUserCart(User *user, Product *product, int quantity)
    {
        auto it = std::find_if(users.begin(), users.end(), [user](const User *u)
                               { return u->getUserId() == user->getUserId(); });
        if (it != users.end())
        {
            (*it)->cart->addProduct(product, quantity);
        }
        else
        {
            throw std::invalid_argument("User not found in the system.");
        }
    }

    // Checkout user's shopping cart
    Order cartCheckout(User *user, IPaymentMethod *paymentMethod)
    {
        auto it = std::find_if(users.begin(), users.end(), [user](const User *u)
                               { return u->getUserId() == user->getUserId(); });
        if (it != users.end())
        {
            return (*it)->checkout(paymentMethod);
        }
        else
        {
            throw std::invalid_argument("User not found in the system.");
        }
    }

    // Getters...
    const std::vector<Product *> &getCatalog() const { return catalog; }
    const std::vector<User *> &getUsers() const { return users; }
};

int main()
{
    // Usage example...

    OnlineShoppingSystem shoppingSystem;

    // Adding products to the catalog
    Product *product1 = new Product("P001", "Product 1", 29.99, "Description 1");
    Product *product2 = new Product("P002", "Product 2", 39.99, "Description 2");
    shoppingSystem.addProductToCatalog(product1);
    shoppingSystem.addProductToCatalog(product2);

    // Adding users
    User *user1 = new User("U001", "John Doe", "john@example.com");
    shoppingSystem.addUser(user1);

    // Adding products to user's cart
    shoppingSystem.addProductToUserCart(user1, product1, 2);
    shoppingSystem.addProductToUserCart(user1, product2, 1);

    // Checkout with Credit Card
    CreditCard creditCard;
    Order newOrder = shoppingSystem.cartCheckout(user1, &creditCard);

    return 0;
}
