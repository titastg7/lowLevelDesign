/*##########################################################################
System Requirements
--------------------
1. Table Reservation Management: Handle booking and management of tables.
2. Order Management: Process food orders from customers.
3. Inventory Management: Keep track of kitchen inventory and supplies.
4. Billing System: Generate and manage customer bills.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Reserving Tables
2. Placing and Processing Food Orders
3. Managing Inventory
4. Generating and Processing Bills
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class Table
{
private:
    int tableId;
    int seatingCapacity;
    bool isReserved;

public:
    Table(int tableId, int seatingCapacity)
        : tableId(tableId), seatingCapacity(seatingCapacity), isReserved(false) {}

    void reserveTable()
    {
        isReserved = true;
    }

    void releaseTable()
    {
        isReserved = false;
    }

    int getTableId() const
    {
        return tableId;
    }

    bool isReserved() const
    {
        return isReserved;
    }
};

enum class Ingredient
{
    BREAD,
    CHICKEN,
    PANEER,
    GINGER_GARLIC_PASTE,
    PENNE,
    TOMATTO_SAUCE
    // Add more ingredients as needed
};

enum class Dish
{
    PENNE_PASTA_IN_RED_SAUCE,
    CHICKEN_BURGER,
    PANNER_KADHAI
};

class DishRecipe
{
protected:
    std::unordered_map<Ingredient, int> ingredients;
    int price;

public:
    virtual bool checkIngredients(const Inventory &inventory) const = 0;
    virtual void makeDish(Inventory &inventory) const = 0;
    virtual int getPrice() const = 0;
    virtual ~DishRecipe() = default;
};

class PENNE_PASTA_IN_RED_SAUCE : public DishRecipe
{
public:
    PENNE_PASTA_IN_RED_SAUCE()
    {
        ingredients[Ingredient::PENNE] = 5;
        ingredients[Ingredient::TOMATO_SAUCE] = 2;
        price = 10;
    }

    bool checkIngredients(const Inventory &inventory) const override
    {
        for (const auto &ingredient : ingredients)
        {
            if (!inventory.checkIngredientAvailability(ingredient.first, ingredient.second))
            {
                return false;
            }
        }
        return true;
    }

    int getPrice() const override
    {
        return price;
    }

    void makeDish(Inventory &inventory) const override
    {
        for (const auto &ingredient : ingredients)
        {
            inventory.useIngredient(ingredient.first, ingredient.second);
        }
        std::cout << "Preparing PENNE_PASTA_IN_RED_SAUCE..." << std::endl;
    }
};

class Order
{
private:
    int orderId;
    std::unordered_map<DishRecipe *, int> dishes; // Item name and quantity

public:
    Order(int orderId) : orderId(orderId) {}

    void addDish(DishRecipe *dish, int quantity)
    {
        dishes[dish] += quantity;
    }
};

class Bill
{
private:
    int billId;
    Order order;
    double totalAmount;

public:
    Bill(int billId, const Order &order) : billId(billId), order(order)
    {
        totalAmount = calculateTotal(order);
    }

private:
    double calculateTotal(const Order &order) const
    {
        return 0.0; // Placeholder for total calculation
    }
};

class RestaurantManagementSystem
{
private:
    std::vector<Table> tables;
    Inventory inventory;

public:
    RestaurantManagementSystem()
    {
        // Initialize tables and inventory
        initializeTables();
    }

    Table *reserveTable(int tableId)
    {
        // Find and reserve a table based on tableId
        for (auto &table : tables)
        {
            if (table.getTableId() == tableId && !table.isReserved())
            {
                table.reserveTable();
                return &table;
            }
        }
        return nullptr; // No table available or invalid tableId
    }

    Order placeOrder(int orderId, const std::unordered_map<DishRecipe *, int> &orderedDishes)
    {
        // Place a new food order
        Order newOrder(orderId);
        for (const auto &dish : orderedDishes)
        {
            if (dish.first->checkIngredients(inventory))
            {
                newOrder.addDish(dish.first, dish.second);
            }
            else
            {
                // Handle case where dish cannot be prepared due to insufficient ingredients
                std::cout << "Dish not available: " << dish.first->getName() << std::endl;
            }
        }
        return newOrder;
    }

private:
    void initializeTables()
    {
        // Assuming the restaurant has 10 tables with varying seating capacities
        for (int i = 1; i <= 10; i++)
        {
            tables.emplace_back(i, i + 2); // Example: Table ID 1 with 3 seats, etc.
        }
    }
};

int main()
{
    // Example usage of RestaurantManagementSystem
    RestaurantManagementSystem restaurant;

    // Reserve a table
    Table *reservedTable = restaurant.reserveTable(1);
    if (reservedTable)
    {
        std::cout << "Table " << reservedTable->getTableId() << " reserved." << std::endl;
    }
    else
    {
        std::cout << "Failed to reserve table." << std::endl;
    }

    // Place an order
    PENNE_PASTA_IN_RED_SAUCE pennePasta;
    std::unordered_map<DishRecipe *, int> orderedDishes = {{&pennePasta, 1}};
    Order newOrder = restaurant.placeOrder(123, orderedDishes);

    // Generate a bill for the order
    Bill bill(456, newOrder);
    std::cout << "Total amount for order " << newOrder.getOrderId() << " is $" << bill.getTotalAmount() << std::endl;

    return 0;
}