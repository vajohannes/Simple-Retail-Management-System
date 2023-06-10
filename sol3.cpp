// === Region: Header files ===
// Necessary header files are included,
// DO NOT include extra header files
// ============================
#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

const int MAX_NUM_SHOPPING_CARTS = 10; // at most 10 shopping carts
const int MAX_ID = 10;                 // at most 10 characters (including the NULL character)
const int MAX_TITLE = 100;             // at most 100 characters (including the NULL character)

// A sorted linked list of StockItem, sorted by its id
struct StockItem
{
    char id[MAX_ID];           // id is a unique identifier of the StockItem (e.g., item001)
    char title[MAX_TITLE];     // title is a description of the StockItem (e.g., Milk)
    unsigned int priceInCents; // Price in cents. double/float is not used to avoid precision problems
    StockItem *next;           // The pointer pointing to the next StockItem
};

// A sorted linked list represents a shopping cart, sorted by item->id
struct ShoppingCartItem
{
    unsigned int quantity;  // A number of items
    const StockItem *item;  // A pointer pointing to the StockItem
    ShoppingCartItem *next; // The pointer pointing to the next ShoppingCartItem
};

StockItem *ll_create_stock_item(const char id[MAX_ID], const char title[MAX_TITLE], const unsigned int priceInCents)
{
    StockItem *newStockItem = new StockItem;
    strcpy(newStockItem->id, id);
    strcpy(newStockItem->title, title);
    newStockItem->priceInCents = priceInCents;
    newStockItem->next = nullptr;
    return newStockItem;
}

ShoppingCartItem *ll_create_shopping_cart_item(const StockItem *stockItem, const unsigned int quantity)
{
    ShoppingCartItem *newShoppingCartItem = new ShoppingCartItem;
    newShoppingCartItem->item = stockItem;
    newShoppingCartItem->quantity = quantity;
    newShoppingCartItem->next = nullptr;
    return newShoppingCartItem;
}

// Helper function: search stock item and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool ll_search_stock_item(StockItem *head, const char id[MAX_ID], StockItem *&prev, StockItem *&current)
{
    prev = nullptr;
    int cmp;
    for (current = head; current != nullptr; current = current->next)
    {
        cmp = strcmp(current->id, id);
        if (cmp == 0)
        {
            // found an existing entry
            return true;
        }
        else if (cmp > 0)
        {
            // cannot find an existing entry
            return false;
        }
        prev = current;
    }
    return false;
}

// Helper function: overloaded, return current only
StockItem *ll_search_stock_item(StockItem *head, const char id[MAX_ID])
{
    StockItem *prev, *current;
    prev = current = nullptr;
    if (ll_search_stock_item(head, id, prev, current))
        return current;
    return nullptr;
}

// Helper function:  search shopping cart and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool ll_search_shopping_cart_item(ShoppingCartItem *head, const char id[MAX_ID], ShoppingCartItem *&prev, ShoppingCartItem *&current)
{
    prev = current = nullptr;
    int cmp;
    for (current = head; current != nullptr; current = current->next)
    {
        if (current->item != nullptr)
        {
            cmp = strcmp(current->item->id, id);
            if (cmp == 0)
            {
                // found an existing entry
                return true;
            }
            else if (cmp > 0)
            {
                return false;
            }
            prev = current;
        }
    }
    return false;
}

// Helper function: overloaded, return current only
ShoppingCartItem *ll_search_shopping_cart_item(ShoppingCartItem *head, const char id[MAX_ID])
{
    ShoppingCartItem *prev, *current;
    prev = current = nullptr;
    if (ll_search_shopping_cart_item(head, id, prev, current))
        return current;
    return nullptr;
}

// Given the number of shopping cart, dynamicially creates and initializes the shopping cart array
ShoppingCartItem **dynamic_init_shopping_cart_array(const unsigned int numOfShoppingCart)
{
    ShoppingCartItem **ret = nullptr;
    ret = new ShoppingCartItem *[numOfShoppingCart];
    for (int i = 0; i < numOfShoppingCart; i++)
        ret[i] = nullptr;
    return ret;
}

bool ll_insert_stock_item(StockItem *&stockItemHead, const char id[MAX_ID], const char title[MAX_TITLE], const unsigned int priceInCents)
{

    // empty list handling
    if (stockItemHead == nullptr)
    {
        stockItemHead = ll_create_stock_item(id, title, priceInCents);
        return true;
    }

    StockItem *prev, *current;
    prev = current = nullptr;
    bool foundGoods = ll_search_stock_item(stockItemHead, id, prev, current);

    if (foundGoods)
    {
        // found an existing entry
        return false; // cannot insert
    }

    // insert - normal case handling
    StockItem *newStockItem = ll_create_stock_item(id, title, priceInCents);
    if (prev == nullptr)
    {
        // insert to the front
        newStockItem->next = stockItemHead;
        stockItemHead = newStockItem;
    }
    else
    {
        prev->next = newStockItem;
        newStockItem->next = current;
    }
    return true;
}

bool ll_update_stock_item_price(StockItem *stockItemHead, const char id[MAX_ID], const unsigned int newPriceInCents)
{
    StockItem *prev, *current;
    prev = current = nullptr;
    bool foundGoods = ll_search_stock_item(stockItemHead, id, prev, current);
    if (foundGoods)
    {
        current->priceInCents = newPriceInCents; // updated
        return true;
    }
    return false;
}

bool ll_insert_or_add_stock_item_quantity(ShoppingCartItem *&shoppingCartHead, StockItem *stockItemHead, const char id[MAX_ID], const unsigned int quantity)
{

    StockItem *currentGoods = ll_search_stock_item(stockItemHead, id);
    if (currentGoods == nullptr)
    {
        // cannot find the goods based on the id
        return false; // cannot insert
    }

    // currentGoods is not nullptr

    // empty list handling
    if (shoppingCartHead == nullptr)
    {
        shoppingCartHead = ll_create_shopping_cart_item(currentGoods, quantity);
        return true;
    }

    ShoppingCartItem *prev, *current;
    prev = current = nullptr;
    bool foundShoppingCartItem = ll_search_shopping_cart_item(shoppingCartHead, id, prev, current);

    if (foundShoppingCartItem)
    {
        // found an existing entry
        // Action: update the quantity
        current->quantity += quantity;
        return true;
    }

    // insert - normal case handling
    ShoppingCartItem *newItem = ll_create_shopping_cart_item(currentGoods, quantity);
    if (prev == nullptr)
    {
        // insert to the front
        newItem->next = shoppingCartHead;
        shoppingCartHead = newItem;
    }
    else
    {
        prev->next = newItem;
        newItem->next = current;
    }
    return true;
}

bool ll_deduct_stock_item_quantity_from_shopping_cart(ShoppingCartItem *&shoppingCartHead, const char id[MAX_ID], const unsigned int deductQuantity)
{

    ShoppingCartItem *prev, *current;
    prev = current = nullptr;
    bool foundShoppingCartItem = ll_search_shopping_cart_item(shoppingCartHead, id, prev, current);

    if (foundShoppingCartItem)
    {
        // found an existing entry
        // Action: update the quantity
        int newQuatity = current->quantity - deductQuantity;
        if (newQuatity < 0)
        {
            return false; // quantity cannot be negative in the shopping cart
        }
        else if (newQuatity == 0)
        {
            // need to delete the shopping cart item

            if (prev == nullptr)
            {

                // delete and update the head
                shoppingCartHead = current->next;
                delete current;
            }
            else
            {
                prev->next = current->next;
                delete current;
            }
            return true;
        }

        // quantity is positive
        current->quantity = newQuatity;
        return true;
    }

    return false;
}

bool ll_remove_stock_item_from_shopping_cart(ShoppingCartItem *&shoppingCartHead, const char id[MAX_ID])
{

    ShoppingCartItem *prev, *current;
    prev = current = nullptr;
    bool foundShoppingCartItem = ll_search_shopping_cart_item(shoppingCartHead, id, prev, current);

    if (foundShoppingCartItem)
    {
        // found an existing entry
        if (prev == nullptr)
        {

            // delete and update the head
            shoppingCartHead = current->next;
            delete current;
        }
        else
        {
            prev->next = current->next;
            delete current;
        }
        return true;
    }

    return false;
}

bool ll_remove_stock_item(StockItem *&stockItemHead, ShoppingCartItem **shoppingCartItemArray, const unsigned int numOfShoppingCart, const char id[MAX_ID])
{

    // empty list handling
    if (stockItemHead == nullptr)
    {
        return false; // no need to remove anything from an empty list
    }

    StockItem *prev, *current;
    prev = current = nullptr;
    bool foundGoods = ll_search_stock_item(stockItemHead, id, prev, current);

    if (foundGoods == false)
    {
        return false; // cannot remove a goods if it is not found
    }

    // We need to loop through each shopping cart, and
    // remove the corresponding shopping cart item on each shopping cart
    // Each cart should have at most one item
    for (int i = 0; i < numOfShoppingCart; i++)
        ll_remove_stock_item_from_shopping_cart(shoppingCartItemArray[i], id);

    // Now, it is safe to remove the goods

    if (prev == nullptr)
    {
        // delete and update the head
        stockItemHead = current->next;
        delete current;
    }
    else
    {
        prev->next = current->next;
        delete current;
    }
    return true;
}

unsigned int calculate_total_amount_in_shopping_cart(const ShoppingCartItem *shoppingCartHead)
{
    const ShoppingCartItem *c;
    unsigned int totalAmount = 0;
    for (c = shoppingCartHead; c != nullptr; c = c->next)
    {
        if (c->item != nullptr)
        {
            totalAmount = totalAmount + c->quantity * c->item->priceInCents;
        }
    }
    return totalAmount;
}

void ll_clear_shopping_cart(ShoppingCartItem *&shoppingCartHead)
{
    while (shoppingCartHead != nullptr)
    {
        if (shoppingCartHead->item != nullptr)
        {
            ll_remove_stock_item_from_shopping_cart(shoppingCartHead, shoppingCartHead->item->id);
        }
    }
}

void ll_cleanup(StockItem *&stockItemHead, ShoppingCartItem **&shoppingCartItemArray, const unsigned int numOfShoppingCart)
{
    while (stockItemHead != nullptr)
    {
        ll_remove_stock_item(stockItemHead, shoppingCartItemArray, numOfShoppingCart, stockItemHead->id);
    }

    delete[] shoppingCartItemArray; // delete the dynamically allocated 2D array
    shoppingCartItemArray = nullptr;
}

void ll_print_all(const StockItem *stockItemHead, ShoppingCartItem **shoppingCartItemArray, const unsigned int numOfShoppingCart)
{
    const StockItem *p;
    const ShoppingCartItem *c;
    int count, i;
    cout << "=== StockItem List (id[price]) ===" << endl;
    count = 0;
    for (p = stockItemHead; p != nullptr; p = p->next)
    {
        cout << p->id << "[$" << p->priceInCents / 100;
        cout << "." << setfill('0') << setw(2) << p->priceInCents % 100 << "]";
        if (p->next != nullptr)
            cout << " -> ";
        count++;
    }
    if (count == 0)
    {
        cout << "No items in the StockItem list";
    }
    cout << endl;

    cout << "=== StockItem titles ===" << endl;
    count = 0;
    for (p = stockItemHead; p != nullptr; p = p->next)
    {
        cout << p->id << ": " << p->title << endl;
        count++;
    }
    if (count == 0)
    {
        cout << "No StockItem titles" << endl;
    }

    cout << "=== Shopping carts ===" << endl;
    if (shoppingCartItemArray != nullptr)
    {
        for (i = 0; i < numOfShoppingCart; i++)
        {
            count = 0;
            cout << "Cart " << i << ": ";
            for (c = shoppingCartItemArray[i]; c != nullptr; c = c->next)
            {
                if (c->item != nullptr)
                {
                    cout << c->item->id << ": " << c->quantity;
                    if (c->next != nullptr)
                        cout << ", "; // except the last item, print a comma
                    count++;
                }
            }
            if (count == 0)
            {
                cout << "No items in the shopping cart" << endl;
            }
            else
            {
                cout << endl;
            }
        }
    }
}

// === Region: The main function ===
// The main function implementation is given
// DO NOT make any changes to the main function
// ============================
int main()
{
    enum MeunOption
    {
        OPTION_DISPLAY_CURRENT_LIST = 0,
        OPTION_INSERT_STOCK_ITEM,
        OPTION_UPDATE_STOCK_ITEM_PRICE,
        OPTION_INSERT_OR_ADD_STOCK_ITEM_TO_SHOPPING_CART,
        OPTION_REMOVE_STOCK_ITEM_FROM_SHOPPING_CART,
        OPTION_DEDUCT_STOCK_ITEM_FROM_SHOPPING_CART,
        OPTION_REMOVE_STOCK_ITEM_FROM_STOCK_ITEM_LIST,
        OPTION_CHECKOUT_AND_CLEAR_SHOPPING_CART,
        OPTION_EXIT_SYSTEM,
        MAX_MENU_OPTIONS
    };
    const int MAX_MENU_OPTIONS_LENGTH = 80;
    char menuOptions[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
        "Display the current lists",
        "Insert a new stock item to the stock item list",
        "Update the price of the stock item",
        "Insert/Add a number of stock items to a shopping cart",
        "Remove an item from the shopping cart",
        "Deduct a number of stock items from a shopping cart",
        "Remove an item from the stock item list",
        "Checkout and clear a shopping cart",
        "Exit the system"};

    StockItem *stockItemHead = nullptr;
    ShoppingCartItem **shoppingCartItemArray = nullptr;
    unsigned int numOfShoppingCart = 0;
    int i, option;
    unsigned int priceInCents = 0;
    unsigned int quantity = 0;
    unsigned int deductQuantity = 0;
    unsigned int whichCart = 0;
    unsigned int totalAmount = 0;
    char id[MAX_ID] = "";
    char title[MAX_TITLE] = "";
    bool ret = false;

    cout << "=== Initialize the shopping carts ===" << endl;
    while (true)
    {
        cout << "Enter the number of shopping cart (1.." << MAX_NUM_SHOPPING_CARTS << "): ";
        cin >> numOfShoppingCart;
        if (numOfShoppingCart == 0 || numOfShoppingCart > MAX_NUM_SHOPPING_CARTS)
        {
            cout << "Invalid number of shopping cart" << endl;
            continue;
        }
        shoppingCartItemArray = dynamic_init_shopping_cart_array(numOfShoppingCart);
        break;
    }

    cout << "=== Simplified Retail System ===" << endl;
    while (true)
    {
        cout << "=== Menu ===" << endl;
        for (i = 0; i < MAX_MENU_OPTIONS; i++)
            cout << i + 1 << ": " << menuOptions[i] << endl; // shift by +1 when display

        cout << "Enter your option: ";
        cin >> option;
        option = option - 1; // shift by -1 after entering the option

        // The invalid menu option handling
        if (option < 0 || option >= MAX_MENU_OPTIONS)
        {
            cout << "Invalid option" << endl;
            continue;
        }

        // Exit operations handling
        if (option == OPTION_EXIT_SYSTEM)
        {
            ll_cleanup(stockItemHead, shoppingCartItemArray, numOfShoppingCart);
            break; // break the while loop
        }

        switch (option)
        {
        case OPTION_DISPLAY_CURRENT_LIST:
            ll_print_all(stockItemHead, shoppingCartItemArray, numOfShoppingCart);
            break;
        case OPTION_INSERT_STOCK_ITEM:
            cout << "Enter a ID: ";
            cin >> id;
            cout << "Enter a title: ";
            cin >> title;

            priceInCents = 0;
            while (priceInCents == 0)
            {
                cout << "Enter a price in cents: ";
                cin >> priceInCents;
                if (priceInCents == 0)
                {
                    cout << "Enter a positive price in cents" << endl;
                }
            }

            ret = ll_insert_stock_item(stockItemHead, id, title, priceInCents);
            if (ret == false)
            {
                cout << "Failed to insert " << id << endl;
            }
            else
            {
                cout << id << " is successfully inserted" << endl;
            }
            break;
        case OPTION_UPDATE_STOCK_ITEM_PRICE:

            cout << "Enter a ID: ";
            cin >> id;

            priceInCents = 0;
            while (priceInCents == 0)
            {
                cout << "Enter a price in cents: ";
                cin >> priceInCents;
                if (priceInCents == 0)
                {
                    cout << "Enter a positive price in cents" << endl;
                }
            }

            ret = ll_update_stock_item_price(stockItemHead, id, priceInCents);
            if (ret == false)
            {
                cout << "Failed to update the price of " << id << endl;
            }
            else
            {
                cout << id << " price is updated" << endl;
            }
            break;
        case OPTION_INSERT_OR_ADD_STOCK_ITEM_TO_SHOPPING_CART:

            while (true)
            {
                cout << "Enter a shopping cart ID (valid: 0 to " << numOfShoppingCart - 1 << "): ";
                cin >> whichCart;
                if (whichCart >= 0 && whichCart < numOfShoppingCart)
                    break;
                else
                    cout << "Please enter a valid shopping cart ID" << endl;
            }
            cout << "Enter a ID: ";
            cin >> id;
            quantity = 0;
            while (quantity == 0)
            {
                cout << "Add a quantity: ";
                cin >> quantity;
                if (quantity == 0)
                {
                    cout << "Please enter a positive quantity" << endl;
                }
            }
            ret = ll_insert_or_add_stock_item_quantity(shoppingCartItemArray[whichCart], stockItemHead, id, quantity);
            if (ret == false)
            {
                cout << "Failed to insert/update " << id << endl;
            }
            else
            {
                cout << id << " is successfully inserted/updated" << endl;
            }
            break;
        case OPTION_DEDUCT_STOCK_ITEM_FROM_SHOPPING_CART:

            while (true)
            {
                cout << "Enter a shopping cart ID (valid: 0 to " << numOfShoppingCart - 1 << "): ";
                cin >> whichCart;
                if (whichCart >= 0 && whichCart < numOfShoppingCart)
                    break;
                else
                    cout << "Please enter a valid shopping cart ID" << endl;
            }
            cout << "Enter a ID: ";
            cin >> id;
            deductQuantity = 0;
            while (deductQuantity == 0)
            {
                cout << "Deduct a quantity: ";
                cin >> deductQuantity;
                if (deductQuantity == 0)
                {
                    cout << "Please enter a positive quantity" << endl;
                }
            }
            ret = ll_deduct_stock_item_quantity_from_shopping_cart(shoppingCartItemArray[whichCart], id, deductQuantity);
            if (ret == false)
            {
                cout << "Failed to deduct quantity " << id << endl;
            }
            else
            {
                cout << "Quantity of " << id << " is successfully deducted" << endl;
            }
            break;
        case OPTION_REMOVE_STOCK_ITEM_FROM_SHOPPING_CART:
            while (true)
            {
                cout << "Enter a shopping cart ID (valid: 0 to " << numOfShoppingCart - 1 << "): ";
                cin >> whichCart;
                if (whichCart >= 0 && whichCart < numOfShoppingCart)
                    break;
                else
                    cout << "Please enter a valid shopping cart ID" << endl;
            }
            cout << "Enter a ID: ";
            cin >> id;
            ret = ll_remove_stock_item_from_shopping_cart(shoppingCartItemArray[whichCart], id);
            if (ret == false)
            {
                cout << "Failed to remove goods " << id << endl;
            }
            else
            {
                cout << id << " is successfully removed" << endl;
            }
            break;
        case OPTION_REMOVE_STOCK_ITEM_FROM_STOCK_ITEM_LIST:
            cout << "Enter a ID: ";
            cin >> id;
            ret = ll_remove_stock_item(stockItemHead, shoppingCartItemArray, numOfShoppingCart, id);
            if (ret == false)
            {
                cout << "Failed to remove " << id << " from the goods list" << endl;
            }
            else
            {
                cout << id << " is removed from the goods list" << endl;
            }
            break;
        case OPTION_CHECKOUT_AND_CLEAR_SHOPPING_CART:
            while (true)
            {
                cout << "Enter a shopping cart ID (valid: 0 to " << numOfShoppingCart - 1 << "): ";
                cin >> whichCart;
                if (whichCart >= 0 && whichCart < numOfShoppingCart)
                    break;
                else
                    cout << "Please enter a valid shopping cart ID" << endl;
            }
            totalAmount = calculate_total_amount_in_shopping_cart(shoppingCartItemArray[whichCart]);
            if (totalAmount > 0)
            {

                cout << "Please pay for $" << totalAmount / 100;
                cout << "." << setfill('0') << setw(2) << totalAmount % 100 << endl;
            }
            else
            {
                cout << "You don't need to pay!" << endl;
            }
            ll_clear_shopping_cart(shoppingCartItemArray[whichCart]);
            cout << "The shopping cart " << whichCart << " is cleared" << endl;
            break;
        default:
            break;

        } // end of switch (option)
    }

    return 0;
}
