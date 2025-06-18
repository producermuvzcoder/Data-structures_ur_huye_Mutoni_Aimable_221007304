#include <iostream>
#include <cstring>
#include <iomanip>

// MenuItem struct definition
struct MenuItem {
    char name[30];
    float price;
};

// Static menu array - represents the restaurant's fixed menu
static MenuItem menu[20] = {
    {"Burger", 12.99f},
    {"Pizza", 15.50f},
    {"Pasta", 11.25f},
    {"Salad", 8.75f},
    {"Steak", 24.99f},
    {"Chicken Wings", 9.99f},
    {"Fish & Chips", 13.50f},
    {"Tacos", 7.99f},
    {"Soup", 6.50f},
    {"Sandwich", 8.99f},
    {"", 0.0f} // Empty slots for expansion
};

// Abstract base class for orders
class OrderBase {
protected:
    static int nextOrderId;
    int orderId;
    MenuItem** items;  // Dynamic array of pointers to menu items
    int nItems;
    int capacity;

public:
    OrderBase() : orderId(++nextOrderId), items(nullptr), nItems(0), capacity(0) {}

    virtual ~OrderBase() {
        delete[] items;
    }

    // Pure virtual function - must be implemented by derived classes
    virtual float getTotal() = 0;

    // Add item to order using pointer to static menu
    void addItem(MenuItem* menuItem) {
        if (nItems >= capacity) {
            // Resize dynamic array
            capacity = (capacity == 0) ? 2 : capacity * 2;
            MenuItem** newItems = new MenuItem*[capacity];

            // Copy existing pointers using pointer arithmetic
            for (int i = 0; i < nItems; i++) {
                *(newItems + i) = *(items + i);
            }

            delete[] items;
            items = newItems;
        }

        // Add new item pointer using pointer arithmetic
        *(items + nItems) = menuItem;
        nItems++;
    }

    // Calculate base total using pointer arithmetic
    float calculateBaseTotal() const {
        float total = 0.0f;
        for (int i = 0; i < nItems; i++) {
            total += (*(items + i))->price;  // Pointer arithmetic to access price
        }
        return total;
    }

    int getOrderId() const { return orderId; }
    int getItemCount() const { return nItems; }

    void displayItems() const {
        std::cout << "Order #" << orderId << " items:\n";
        for (int i = 0; i < nItems; i++) {
            MenuItem* item = *(items + i);  // Using pointer arithmetic
            std::cout << "  - " << item->name << " ($" << std::fixed << std::setprecision(2)
                      << item->price << ")\n";
        }
    }
};

// Initialize static order ID
int OrderBase::nextOrderId = 0;

// Dine-in order class
class DineInOrder : public OrderBase {
private:
    float serviceCharge;  // Percentage (e.g., 0.18 for 18%)

public:
    DineInOrder(float serviceRate = 0.15f) : OrderBase(), serviceCharge(serviceRate) {}

    // Override getTotal to add service charge
    float getTotal() override {
        float baseTotal = calculateBaseTotal();
        return baseTotal + (baseTotal * serviceCharge);
    }

    void setServiceCharge(float rate) { serviceCharge = rate; }
    float getServiceCharge() const { return serviceCharge; }
};

// Pickup order class
class PickUpOrder : public OrderBase {
private:
    float packagingFee;

public:
    PickUpOrder(float fee = 1.50f) : OrderBase(), packagingFee(fee) {}

    // Override getTotal to add packaging fee
    float getTotal() override {
        return calculateBaseTotal() + packagingFee;
    }

    void setPackagingFee(float fee) { packagingFee = fee; }
    float getPackagingFee() const { return packagingFee; }
};

// Order Management System
class OrderManager {
private:
    OrderBase** orders;  // Dynamic array of order pointers
    int nOrders;
    int capacity;

public:
    OrderManager() : orders(nullptr), nOrders(0), capacity(0) {}

    ~OrderManager() {
        // Clean up all orders
        for (int i = 0; i < nOrders; i++) {
            delete *(orders + i);
        }
        delete[] orders;
    }

    // Add order with dynamic resizing using pointer arithmetic
    void addOrder(OrderBase* order) {
        if (nOrders >= capacity) {
            capacity = (capacity == 0) ? 2 : capacity * 2;
            OrderBase** newOrders = new OrderBase*[capacity];

            // Copy existing pointers using pointer arithmetic
            for (int i = 0; i < nOrders; i++) {
                *(newOrders + i) = *(orders + i);
            }

            delete[] orders;
            orders = newOrders;
        }

        *(orders + nOrders) = order;
        nOrders++;
        std::cout << "Order added successfully. Order ID: " << order->getOrderId() << "\n";
    }

    // Remove order by ID with array compaction using pointer arithmetic
    bool removeOrder(int orderId) {
        int index = -1;

        // Find order by ID using pointer arithmetic
        for (int i = 0; i < nOrders; i++) {
            if ((*(orders + i))->getOrderId() == orderId) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            std::cout << "Order ID " << orderId << " not found.\n";
            return false;
        }

        // Delete the order object
        delete *(orders + index);

        // Shift remaining orders left using pointer arithmetic
        for (int i = index; i < nOrders - 1; i++) {
            *(orders + i) = *(orders + i + 1);
        }

        nOrders--;
        std::cout << "Order ID " << orderId << " removed successfully.\n";
        return true;
    }

    // Display all orders using polymorphic dispatch
    void displayAllOrders() const {
        if (nOrders == 0) {
            std::cout << "No orders in system.\n";
            return;
        }

        std::cout << "\n=== ALL ORDERS ===\n";
        for (int i = 0; i < nOrders; i++) {
            OrderBase* order = *(orders + i);  // Using pointer arithmetic
            order->displayItems();

            // Polymorphic call - dispatches to correct getTotal() implementation
            std::cout << "Total: $" << std::fixed << std::setprecision(2) << order->getTotal() << "\n";

            // Display order type using dynamic_cast
            if (DineInOrder* dine = dynamic_cast<DineInOrder*>(order)) {
                std::cout << "Type: Dine-In (Service: " << (dine->getServiceCharge() * 100) << "%)\n";
            } else if (PickUpOrder* pickup = dynamic_cast<PickUpOrder*>(order)) {
                std::cout << "Type: Pickup (Packaging: $" << pickup->getPackagingFee() << ")\n";
            }
            std::cout << "------------------------\n";
        }
    }

    // Find menu item by name (returns pointer to static menu)
    MenuItem* findMenuItem(const char* name) {
        for (int i = 0; i < 20; i++) {
            if (strlen((menu + i)->name) > 0 && strcmp((menu + i)->name, name) == 0) {
                return menu + i;  // Return pointer to static menu item using pointer arithmetic
            }
        }
        return nullptr;
    }

    void displayMenu() const {
        std::cout << "\n=== MENU ===\n";
        for (int i = 0; i < 20; i++) {
            if (strlen((menu + i)->name) > 0) {
                std::cout << i + 1 << ". " << (menu + i)->name
                         << " - $" << std::fixed << std::setprecision(2) << (menu + i)->price << "\n";
            }
        }
        std::cout << "============\n";
    }

    int getOrderCount() const { return nOrders; }

    // Get order by index using pointer arithmetic
    OrderBase* getOrder(int index) const {
        if (index >= 0 && index < nOrders) {
            return *(orders + index);
        }
        return nullptr;
    }
};

// Demo function to showcase the system
void demonstrateSystem() {
    OrderManager manager;

    std::cout << "=== RESTAURANT ORDER MANAGEMENT SYSTEM DEMO ===\n";

    // Display menu
    manager.displayMenu();

    // Create dine-in order
    std::cout << "\nCreating Dine-In Order:\n";
    DineInOrder* dineIn = new DineInOrder(0.18f); // 18% service charge
    dineIn->addItem(manager.findMenuItem("Burger"));
    dineIn->addItem(manager.findMenuItem("Pizza"));
    dineIn->addItem(manager.findMenuItem("Steak"));
    manager.addOrder(dineIn);

    // Create pickup order
    std::cout << "\nCreating Pickup Order:\n";
    PickUpOrder* pickup = new PickUpOrder(2.00f); // $2.00 packaging fee
    pickup->addItem(manager.findMenuItem("Pasta"));
    pickup->addItem(manager.findMenuItem("Salad"));
    manager.addOrder(pickup);

    // Create another dine-in order
    std::cout << "\nCreating Another Dine-In Order:\n";
    DineInOrder* dineIn2 = new DineInOrder(0.20f); // 20% service charge
    dineIn2->addItem(manager.findMenuItem("Chicken Wings"));
    dineIn2->addItem(manager.findMenuItem("Tacos"));
    dineIn2->addItem(manager.findMenuItem("Soup"));
    manager.addOrder(dineIn2);

    // Display all orders (demonstrates polymorphism)
    manager.displayAllOrders();

    // Demonstrate polymorphic dispatch
    std::cout << "\n=== POLYMORPHIC DISPATCH DEMO ===\n";
    std::cout << "Order totals calculated polymorphically:\n";
    for (int i = 0; i < manager.getOrderCount(); i++) {
        OrderBase* order = manager.getOrder(i);
        if (order) {
            std::cout << "Order " << order->getOrderId() << " total: $"
                      << std::fixed << std::setprecision(2) << order->getTotal() << "\n";
        }
    }

    // Remove an order
    std::cout << "\nRemoving Order ID 2:\n";
    manager.removeOrder(2);

    // Display remaining orders
    std::cout << "\nRemaining orders after removal:\n";
    manager.displayAllOrders();
}

int main() {
    demonstrateSystem();
    return 0;
}
