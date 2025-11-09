#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 50
#define MIN_PRICE 0.0f
#define MIN_QUANTITY 0

typedef struct 
{
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Product;

void addProduct(Product **products, int *count);
void viewProducts(Product *products, int count);
void updateQuantity(Product *products, int count);
void searchByID(Product *products, int count);
void searchByName(Product *products, int count);
void searchByPriceRange(Product *products, int count);
void deleteProduct(Product **products, int *count);
void freeMemory(Product *products);

int main() {
    int numberofProducts, choice;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    scanf("%d", &numberofProducts);
    getchar(); 

    inventory = (Product *)calloc(numberofProducts, sizeof(Product));
    if (inventory == NULL) 
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int product = 0; product < numberofProducts; product++) 
{
    int unique = 0;

    while (!unique)
    {
        printf("\nEnter details for product %d:\n", product + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[product].id);
        getchar();

        unique = 1;  

        
        for (int previousProduct = 0; previousProduct < product; previousProduct++) 
        {
            if (inventory[previousProduct].id == inventory[product].id) 
            {
                printf("Error: Product ID %d already exists! Please enter a unique ID.\n", inventory[product].id);
                unique = 0; 
                break;
            }
        }
    }

    printf("Product Name: ");
    fgets(inventory[product].name, NAME_LEN, stdin);
    inventory[product].name[strcspn(inventory[product].name, "\n")] = '\0';

    do {
        printf("Product Price: ");
        scanf("%f", &inventory[product].price);
        if (inventory[product].price <= MIN_PRICE)
            printf("Error: Price must be greater than 0. Please re-enter.\n");
    } while (inventory[product].price <= MIN_PRICE);

    do {
        printf("Product Quantity: ");
        scanf("%d", &inventory[product].quantity);
        if (inventory[product].quantity <= MIN_QUANTITY)
            printf("Error: Quantity must be greater than 0. Please re-enter.\n");
    } while (inventory[product].quantity <= MIN_QUANTITY);

}

    do {
        printf("\n========= INVENTORY MENU ========= \n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) 
        {
            case 1:
                addProduct(&inventory, &numberofProducts);
                break;
            case 2:
                viewProducts(inventory, numberofProducts);
                break;
            case 3:
                updateQuantity(inventory, numberofProducts);
                break;
            case 4:
                searchByID(inventory, numberofProducts);
                break;
            case 5:
                searchByName(inventory, numberofProducts);
                break;
            case 6:
                searchByPriceRange(inventory, numberofProducts);
                break;
            case 7:
                deleteProduct(&inventory, &numberofProducts);
                break;
            case 8:
                freeMemory(inventory);
                printf("\nMemory released successfully. Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 8);

    return 0;
}

void addProduct(Product **products, int *count) 
{
    int newID;
    char tempName[NAME_LEN];
    float newPrice;
    int newQuantity;

    printf("\nEnter new product ID: ");
    scanf("%d", &newID);
    getchar();

    for (int product = 0; product < *count; product++) 
    {
        if ((*products)[product].id == newID) 
        {
            printf("Error: Product ID %d already exists! Please use a unique ID.\n", newID);
            return; 
        }
    }

    printf("Enter Product Name: ");
    fgets(tempName, NAME_LEN, stdin);
    tempName[strcspn(tempName, "\n")] = '\0';

     do {
        printf("Enter Product Price: ");
        scanf("%f", &newPrice);
        if (newPrice <= MIN_PRICE)
            printf("Error: Price must be greater than 0. Please re-enter.\n");
    } while (newPrice <= MIN_PRICE);

    do {
        printf("Enter Product Quantity: ");
        scanf("%d", &newQuantity);
        if (newQuantity <= MIN_QUANTITY)
            printf("Error: Quantity must be greater than 0. Please re-enter.\n");
    } while (newQuantity <= MIN_QUANTITY);

    getchar();

    *products = realloc(*products, (*count + 1) * sizeof(Product));
    if (*products == NULL) 
    {
        printf("Memory reallocation failed!\n");
        return;
    }

    Product *newProduct = &((*products)[*count]);
    newProduct->id = newID;
    strcpy(newProduct->name, tempName);
    newProduct->price = newPrice;
    newProduct->quantity = newQuantity;

    (*count)++;

    printf("Product added successfully!\n");
}

void viewProducts(Product *products, int count) 
{
    if (count == 0) 
    {
        printf("No products in inventory.\n");
        return;
    }

    printf("\n========= PRODUCT LIST ========= \n");
    for (int product = 0; product < count; product++) 
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[product].id, products[product].name, products[product].price, 
               products[product].quantity);
    }
}

void updateQuantity(Product *products, int count) 
{
    int id, newQuantity, found = 0;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);

    for (int productId = 0; productId < count; productId++) 
    {
        if (products[productId].id == id) 
        {
            do {
                printf("Enter new Quantity: ");
                scanf("%d", &newQuantity);
                if (newQuantity <= MIN_QUANTITY)
                    printf("Error: Quantity must be greater than 0. Please re-enter.\n");
            } while (newQuantity <= MIN_QUANTITY);

            products[productId].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product ID not found!\n");
}

void searchByID(Product *products, int count) 
{
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int product = 0; product < count; product++) 
    {
        if (products[product].id == id) 
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[product].id, products[product].name, products[product].price, 
                   products[product].quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product not found!\n");
}

void searchByName(Product *products, int count) 
{
    char search[NAME_LEN];
    int found = 0;

    printf("Enter name to search (case-insensitive, partial name accepted): ");
    fgets(search, NAME_LEN, stdin);
    search[strcspn(search, "\n")] = '\0';

    for (int string = 0; search[string]; string++)
        search[string] = tolower(search[string]);

    printf("Products Found:\n");

    for (int product = 0; product < count; product++) 
    {
        char lowerName[NAME_LEN];
        strcpy(lowerName, products[product].name);

        for (int string = 0; lowerName[string]; string++)
            lowerName[string] = tolower(lowerName[string]);

        if (strstr(lowerName, search)) 
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[product].id, products[product].name, products[product].price, 
                   products[product].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No matching products found!\n");
}

void searchByPriceRange(Product *products, int count) 
{
    float minimum, maximum;
    int found = 0;

    do {
        printf("Enter minimum price (cannot be negative): ");
        scanf("%f", &minimum);
        if (minimum < MIN_PRICE)
            printf("Error: Minimum price cannot be negative. Please re-enter.\n");
    } while (minimum < MIN_PRICE);

    do {
        printf("Enter maximum price (must be greater than or equal to minimum): ");
        scanf("%f", &maximum);
        if (maximum < minimum)
            printf("Error: Maximum price must be greater than or equal to minimum price. Please re-enter.\n");
    } while (maximum < minimum);

    printf("\nProducts in price range %.2f to %.2f:\n", minimum, maximum);
    for (int product = 0; product < count; product++)
    {
        if (products[product].price >= minimum && products[product].price <= maximum) 
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[product].id, products[product].name, products[product].price, 
                   products[product].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products found in this price range!\n");
}

void deleteProduct(Product **products, int *count) 
{
    int id, index = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int product = 0; product < *count; product++) 
    {
        if ((*products)[product].id == id) 
        {
            index = product;
            break;
        }
    }

    if (index == -1) 
    {
        printf("Product not found!\n");
        return;
    }

    for (int product = index; product < *count - 1; product++)
    {
        (*products)[product] = (*products)[product + 1];
    }

    (*count)--;
    *products = realloc(*products, (*count) * sizeof(Product));
    printf("Product deleted successfully!\n");
}

void freeMemory(Product *products) 
{
    free(products);
}
