/*##########################################################################
System Requirements
--------------------
1. Restaurant Management : Handle restaurants and their menu
2. Order Processing: Enable customers to place orders and track their status.
3. Payment Processing: Handle various strategy for delivery charge calculation.
4. Delivery Management: Assign orders to delivery agents and manage the delivery process.
5. User Account Management: Handle customer and delivery agent profiles.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Adding and Managing Restaurants and their dishes
2. Registering and Managing User and Delivery Agent Accounts
3. Placing and Tracking Orders
4. Assigning and Managing Deliveries
5. Delivery Payment Calculation

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

enum class CUISINE
{
    NORTH_INDIAN,
    SOUTH_INDIAN,
    CHINESE,
    STREET_FOOD,
    SWEETS,
    ITALIAN,
};

enum class ORDER_STATUS
{
    PLACED,
    ORDERED,
    ACCEPTED,
    ON_THE_WAY,
    REACHED,
    DELIVERED,
    CANCELLED
};
enum class RATING
{
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    UNASSIGNED
};
class Location
{
    int longitude, latitude;

public:
    Location(int pLongitude, int pLatitude) : longitude(pLongitude), latitude(pLatitude)
    {
    }
    int getLongitude()
    {
        return longitude;
    }
    int getLatitude()
    {
        return Latitude;
    }
};
// Observer Pattern - NotificationMgr = subject
class INotificationSender
{
public:
    virtual void sendNotification(string userId, string msg) = 0;
};

class SMSNotificationSender : public INotificationSender
{
public:
    void sendNotification(string userId, string msg)
    {
        cout << " SMS sent to " << userId << " Msg = " << msg << endl;
    }
};

class PushNotificationSender : public INotificationSender
{
public:
    void sendNotification(string userId, string msg)
    {
        cout << "Push Notification for " << userId << " Msg = " << msg << endl;
    }
};

class NotificationMgr
{
    NotificationMgr()
    {
    }
    static NotificationMgr *notificationMgrInstance;
    static mutex mtx;
    unordered_map<string, vector<pair<string, INotificationSender *>>> notificationSendersMap;

public:
    static NotificationMgr *getNotificationMgr()
    {
        if (notificationMgrInstance == nullptr)
        {
            mtx.lock();
            if (notificationMgrInstance == nullptr)
                notificationMgrInstance = new NotificationMgr();
            mtx.unlock();
        }
        return notificationMgrInstance;
    }
    // Subscribe observer
    void addNotificationSender(string pOrderId, string pUserId, INotificationSender *pNotificationSender)
    {
        if (find(notificationSendersMap[pOrderId].begin(), notificationSendersMap[pOrderId].end(), {pUserId, pNotificationSender}) == notificationSendersMap[pOrderId].end())
        {
            // making sure the sender is already not there in the vector  to avoid sending multiple notifications
            notificationSendersMap[pOrderId].push_back({pUserId, pNotificationSender});
        }
    }
    // Unsubscribe observer
    void removeNotificationSender(string pOrderId, string pUserId, INotificationSender *pNotificationSender)
    {
        auto senderPos = find(notificationSendersMap[pOrderId].begin(), notificationSendersMap[pOrderId].end(), {pUserId, pNotificationSender});
        if (senderPos != notificationSendersMap[pOrderId].end())
            notificationSendersMap[pOrderId].erase(senderPos);
    }

    // notify subscribers
    void notify(string orderId, string pMsg)
    {
        for (auto it : notificationSendersMap[orderId])
            it.second->sendNotification(it.first, pMsg);
    }

    // notify one user
    void notifyOneUser(string userId, string pMsg, INotificationSender *sender)
    {
        sender->sendNotification(userId, pMsg);
    }
};
// Interface
class IPartner
{
    Rating rating;
    string name;

public:
    IPartner(string pName) : name(pName)
    {
        rating = RATING::UNASSIGNED;
    }
    string getName()
    {
        return name;
    }
    // void performKYC() = 0;
};

class DeliveryPartner : public IPartner
{
public:
    DeliveryPartner(string pName) : IPartner(pName) {}
    // Order Status also needs to be updated while these steps are happening
    // We have black-boxed that
    void performDelivery(string pOrderId, DeliveryMetaData *pDeliveryMetaData)
    {
        NotificationMgr *notificationMgr = NotificationMgr::getNotificationMgr();

        double restaurantLocLatitude = pDeliveryMetaData->getRestaurantLoc()->getLatitude();
        double restaurantLocLongitude = pDeliveryMetaData->getRestaurantLoc()->getLongitude();
        notificationMgr->notify(pOrderId, name + " going to pick up delivery from location " + to_string(restaurantLocLatitude) + "," + to_string(restaurantLocLongitude));

        this_thread::sleep_for(chrono::seconds(5));

        notificationMgr->notify(pOrderId, name + " picked up delivery!");

        this_thread::sleep_for(chrono::seconds(5));

        notificationMgr->notify(pOrderId, name + " on the way to deliver!");

        this_thread::sleep_for(chrono::seconds(5));

        double userLocLatitude = pDeliveryMetaData->getUserLoc()->getLatitude();
        double userLocLongitude = pDeliveryMetaData->getUserLoc()->getLongitude();
        notificationMgr->notify(pOrderId, name + " reached the location " + to_string(userLocLatitude) + "," + to_string(userLocLongitude));

        this_thread::sleep_for(chrono::seconds(5));

        notificationMgr->notify(pOrderId, name + " delivered the order. CONGRATULATIONS!!");
    }
};
class DeliveryPartnerMgr
{
    unordered_map<string, DeliveryPartner *> deliveryPartnerMap;
    static DeliveryPartnerMgr *deliveryPartnerMgrInstance = nullptr;
    static mutex mtx;
    DeliveryPartnerMgr() {}

public:
    static DeliveryPartnerMgr *getDeliveryPartnerMgr()
    {
        if (deliveryPartnerMgrInstance == nullptr)
        {
            mtx.lock();
            if (deliveryPartnerMgrInstance == nullptr)
                deliveryPartnerMgrInstance = new DeliveryPartnerMgr();
            mtx.unlock();
        }
        return deliveryPartnerMgrInstance;
    }
    void addDeliveryPartner(string partnerName, DeliveryPartner *partner)
    {
        deliveryPartnerMap[partnerName] = partner;
    }
    DeliveryPartner *getDeliveryPartner(string partnerName)
    {
        return deliveryPartnerMap[partnerName];
    }
    unordered_map<string, DeliveryPartner *> getDeliveryPartnerMap()
    {
        return deliveryPartnerMap;
    }
};
class RestaurantPartner : public IPartner
{
public:
    RestaurantPartner(string name) : IPartner(name) {}
};
class DishAddOn
{
    string addOnName;
    string description;
    vector<string> images;
    double price;
    bool isAvailable;

public:
    DishAddOn(string name, double pPrice) : addOnName(name), price(pPrice) {}
    double getPrice()
    {
        return price;
    }
};
class Dish
{
    string dishName;
    CUISINE cuisine;
    string description;
    vector<string> dishImages;
    vector<DishAddOn *> addOns;
    double price;

public:
    Dish(string pName, CUISINE pCuisine, double pPrice) : dishName(pName), cuisine(pCuisine), price(pPrice)
    {
    }
    void addAddOn(DishAddOn *pAddOn)
    {
        addOns.push_back(pAddOn);
    }
    vector<DishAddOn *> getAddOns()
    {
        return addOns;
    }
    string getDescription() { return description; }
    string getDishName() { return dishName; }
    CUISINE getCuisine() { return cuisine; }
    double getPrice()
    {
        double total = price;
        for (auto addOn : addOns)
            total += addOn->getPrice();
        return total;
    }
};

class Menu
{
    vector<Dish *> dishes;

public:
    Menu(vector<Dish *> pDishes) : dishes(pDishes) {}
};

class FoodMgr
{
    static FoodMgr *foodMgrInstance = nullptr;
    static mutex mtx;
    FoodMgr() {}

public:
    static FoodMgr *getFoodMgr()
    {
        if (foodMgrInstance == nullptr)
        {
            mtx.lock();
            if (foodMgrInstance == nullptr)
                foodMgrInstance = new FoodMgr();
        }
        return foodMgrInstance;
    }
    void prepareFood(string orderId, string restaurantId, unordered_map<Dish *, int> pDishes)
    {
        RestaurantMgr *restaurantMgr = RestaurantMgr::getRestaurantMgr();
        Restaurant *restaurant = restaurantMgr->getRestaurant(RestaurantId);
        restaurant->prepareFood(orderId, pDishes);

        addRestaurantForNotificationUpdates(orderId, restaurantId);
    }
    void addRestaurantForNotificationUpdates(string orderId, string restaurantId)
    {
        NotificationMgr *notificationMgr = NotificationMgr::getNotificationMgr();
        notificationMgr->addNotificationSender(orderId, restaurantId, new PushNotificationSender());
    }
};
class Restaurant
{
    string name;
    bool isAvailable;
    Location *loc;
    Menu *menul RestaurantPartner *owner;

public:
    Restaurant(string pName, RestaurantPartner *pOwner, Location *pLoc) : name(pName), owner(pOwner), loc(pLoc)
    {
        isAvailable = false;
        menu = nullptr;
    }
    void addMenu(Menu *pMenu)
    {
        menu = pMenu;
    }
    string getName()
    {
        return name;
    }
    Location *getLocation()
    {
        return loc;
    }
    bool prepareFood(string orderId, unordered_map < Dish *, int quantity)
    {
        cout << " Restaurant acdepted the order. Your food is being prepared." << endl;
        NotificationMgr *notificationMggr = NotificationMgr::getNotificationMgr();
        notificationMgr->notify(orderId, "Food id being prepared.");
        notificationMgr->notify(orderId, "Food id ready and ready for pickup.");
        return true;
    }
};

class RestaurantMgr
{
    unordered_map<string, Restaurant *> restaurantMap;
    static RestaurantMgr *restaurantMgrInstance = nullptr;
    static mutex mtx;
    RestaurantMgr() {}

public:
    static RestaurantMgr *getRestaurantMgr()
    {
        if (restaurantMgrInstance == nullptr)
        {
            mtx.lock();
            if (restaurantMgrInstance == nullptr)
                restaurantMgrInstance = new RestaurantMgr();
            mtx.unlock();
        }
        return restaurantMgrInstance;
    }
    void addRestaurant(string restaurantName, Restaurant *restaurant)
    {
        restaurantMap[restaurantName] = restaurant;
    }
    Restaurant *getRestaurant(string restaurantName)
    {
        return restaurantMap[restaurantName];
    }
};

class User
{
    string name;
    Location *loc;

public:
    User(string pName, Location *pLoc) : name(pName), loc(pLoc) {}
    Location *getLocation()
    {
        return loc;
    }
    string getName()
    {
        return name;
    }
};

class UserMgr
{
    static UserMgr *userMgrInstance = nullptr;
    static mutex mtx;
    unordered_map<string, User *> userMap;
    UserMgr() {}

public:
    UserMgr *getUserMgr()
    {
        if (userMgrInstance == nullptr)
        {
            mtx.lock();
            if (userMgrInstance == nullptr)
                userMgrInstance = new UserMgr();
        }
        return userMgrInstance;
    }
    User *getUser(string userName)
    {
        return userMap[userName];
    }

    void addUser(string name, User *user)
    {
        userMap[name] = user;
    }
};

class DeliveryMetaData
{
    string orderId;
    Location *userLocation, *restaurantLocation;

public:
    DeliveryMetaData(string pOrderId, Location *pUserLoc, Location *pRestaurantLoc) : orderId(pOrderId), userLocation(pUserLoc), restaurantLocation(pRestaurantLoc)
    {
    }
    Location *getUserLocation()
    {
        return userLocation;
    }

    Location *getRestaurantLocation()
    {
        return restaurantLocation;
    }
};

class DeliveryChargeCalculationStrategy
{
public:
    virtual double calculateDeliveryCharge(DeliveryMetaData *data) = 0;
};

class LocationBasedDeliveryChargeCalculationStrategy : public DeliveryChargeCalculationStrategy
{
public:
    double calculateDeliveryCharge(DeliveryMetaData *data)
    {
        cout << " Delivery charge based on location." << endl;
        return 20.0;
    }
};

class IDeliveryPartnerMatchingStrategy
{
public:
    virtual vector<DeliveryPartner *> matchDeliveryPartners(DeliveryMetaData *pDeliveryMetaData) = 0;
};

class LocationBasedDeliveryPartnerMatchingStrategy : public IDeliveryPartnerMatchingStrategy
{
public:
    DeliveryPartnerMgr *deliveryPartnerMgr = DeliveryPartnerMgr::getDeliveryPartnerMgr();
    vector<DeliveryPartner *> matchDeliveryPartners(DeliveryMetaData *pDeliveryMetaData)
    {
        // Find nearestdriver using QuadTrees
        vector<DeliveryPartner *> nerestPartners;
        for (auto deliveryPartner : deliveryPartnerMgr->getDeliveryPartnerMap())
        {
            nerestPartners.push_back(deliveryPartner.second);
        }
        return nerestPartners;
    }
};
class StrategyMgr
{
    static StrategyMgr *strategyMgrInstance = nullptr;
    static mutex mtx;
    StrategyMgr() {}

public:
    StrategyMgr *getStrategyMgrInstance()
    {
        if (strategyMgrInstance == nullptr)
        {
            mtx.lock();
            if (strategyMgrInstance == nullptr)
                strategyMgrInstance = new StrategyMgr();
        }
        return strategyMgrInstance;
    }
    IDeliveryPartnerMatchingStrategy *determineDeliveryPartnerMatchingStrategy(DeliveryMetaData *metaData)
    {
        cout < " Based on Location, setting partner strategy " << endl;
        return new LocationBasedDeliveryPartnerMatchingStrategy();
    }
};
class DeliveryMgr
{
    static DeliveryMgr *deliveryMgrInstance = nullptr;
    static mutex mtx;
    unordered_map<string, Restaurant *> restaurantMap;
    DeliveryMgr() {}

public:
    DeliveryMgr *getDeliveryMgrInstance()
    {
        if (deliveryMgrInstance == nullptr)
        {
            mtx.lock();
            if (deliveryMgrInstance == nullptr)
                deliveryMgrInstance = new DeliveryMgr();
        }
        return deliveryMgrInstance;
    }
    void manageDelivery(string pOrderId, DeliveryMetaData *data)
    {
        StrategyMgr *strategyMgr = StrategyMgr::getStrategyMgrInstance();

        IDeliveryPartnerMatchingStrategy *partnerMatchingStrategy = strategyMgr->determineDeliveryPartnerMatchingStrategy(data);

        vector<DeliveryPartner *> deliveryPartners = partnerMatchingStrategy->matchDeliveryPartner(data);
        NotificationMgr *notificationMgr = NotificationMgr::getNotificationMgr();

        for (auto partner : deliveryPartners)
        {
            notificationMgr->notifyOneUser(partner->getName(), "Delivery Requested", new PushNotificationSender());
        }

        DeliveryPartner *assignedDeliveryPartner = deliverypartners[0];
        notificationMgr->notify(pOrderId, "Delivery Partner " + assignedDeliveryPartner->getName() + " assigned  for Order " + pOrderId);

        assignedDeliveryPartner->performDelivery(pOrderId, data);
    }
};

class Order
{
    User *user;
    Restaurant *restaurant;
    unordered_map<Dish *, int> dishes;
    string discountCode;
    string paymentId;
    ORDER_STATUS status;
    // generate orderId
    string orderId;

public:
    Order(User *pUser, Restaurant *pRestaurant, unordered_map<Dish *, int> pDishes) : user(pUser), restaurant(pRestaurant), dishes(pDishes)()
    {
        status = ORDER_STATUS ::PLACED;
    }

    string getUserName()
    {
        return user->getName();
    }
    string getRestaurantName()
    {
        return restaurant->getName();
    }
    string getOrderId()
    {
        return orderId;
    }
    unordered_map<Dish *, int> getDishes() { return dishes; }
    Location *getUserLocation()
    {
        return user->getLocation();
    }

    Location *getRestaurantLocation()
    {
        return restaurant->getLocation();
    }
};

class OrderMgr
{
    static OrderMgr *orderMgrInstance = nullptr;
    static mutex mtx;
    unordered_map<string, Order *> orderMap;
    DeliveryMgr *deliveryMgr;
    FoodMgr *foodMgr;
    OrderMgr()
    {
        deliveryMgr = DeliveryMgr::getDeliveryMgr();
        foodMgr = FoodMgr::getFoodMgr();
    }

    void addUserForNotificationUpdates(string orderId, Order *pOrder)
    {
        NotificationMgr *notificationMgr = NotificationMgr::getNotificationMgr();

        notificationMgr->addNotificationSender(orderId, pOrder->getUserName(), new SMSNotificationSender());
    }

    void manageDelivery(string orderId, Order *pOrder)
    {
        DeliveryMetaData *data = new DeliveryMetaData(orderId, pOrder->getUserLocation(), pOrder->getRestaurantLocation());
        deliveryMgr->manageDelivery(orderId, data);
    }

    void manageFood(string orderId, Order *pOrder)
    {
        foodMgr->prepareFood(orderId, pOrder->getRestaurantName(), pOrder->getDishes);
    }

public:
    OrderMgr *getOrderMgr()
    {
        if (orderMgrInstance == nullptr)
        {
            mtx.lock();
            if (orderMgrInstance == nullptr)
                orderMgrInstance = new OrderMgr();
            mtx.unlock();
        }
        return orderMgrInstance;
    }
    void createOrder(string orderId, Order *pOrder)
    {
        addUserForNotificationUpdates(pOrderId, pOrder);

        manageFood(pOrderId, pOrder);

        manageDelivery(pOrderId, pOrder);
    }
    Order *getOrder(string orderId)
    {
        return orderMap[orderId];
    }
};

int main()
{
    // Chinese Restaurant
    RestaurantPartner *owner1 = new RestaurantOwner("owner1");
    Restaurant *chineseRest = new Restaurant("Hadako", owner1, new Location(1, 2));
    Dish *noodles = new Dish("noodles", CUISINE::CHINESE, 200);
    noodles->addAddOn({new DishAddOn("premium sauce", 20)});
    Dish *fried_rice = new Dish("fried rice", CUISINE::CHINESE, 180);
    Dish *spring_rolls = new Dish("spring rolls", CUISINE::CHINESE, 120);
    Menu *chinese_menu = new Menu({noodles, fried_rice, spring_rolls});
    chineseRest->addMenu(chinese_menu);

    RestaurantMgr *restaurantMgr = RestaurantMgr::getRestaurantMgr();
    restaurantMgr->addRestaurant("Hadako", chineseRest);

    DeliveryPartner *partner1 = new DeliveryPartner("Rakesh");
    DeliveryPartner *partner2 = new DeliveryPartner("Suresh");
    DeliveryPartner *partner3 = new DeliveryPartner("Bidesh");

    DeliveryPartnerMgr *deliveryPartnerMgr = DeliveryPartnerMgr::getDeliveryPartnerMgr();
    deliveryPartnerMgr->addDeliveryPartner("Rakesh", partner1);
    deliveryPartnerMgr->addDeliveryPartner("Suresh", partner2);
    deliveryPartnerMgr->addDeliveryPartner("Bidesh", partner3);

    User *user1 = new User("Titas", new Location(10, 11));
    User *user2 = new User("Suku", new Location(13, 14));
    User *user3 = new User("Purnima", new Location(15, 16));

    UserMgr *userMgr = UserMgr::getUserMgr();
    userMgr->addUser("keerti", user1);
    userMgr->addUser("Suku", user2);
    userMgr->addUser("Purnima", user3);

    unordered_map<Dish *, int> cart;
    cart.insert({noodles, 2});
    cart.insert({fried_rice, 1});
    Order *order1 = new Order(user1, chineseRest, cart);

    OrderMgr *orderMgr = OrderMgr::getOrderMgr();
    orderMgr->createOrder("order1", order1);

    return 0;
}