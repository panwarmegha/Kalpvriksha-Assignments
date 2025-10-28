#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

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

    for (int productNumber = 0; productNumber < numberofProducts; productNumber++) 
    {
        printf("\nEnter details for product %d:\n", productNumber + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[productNumber].id);
        getchar();

        printf("Product Name: ");
        fgets(inventory[productNumber].name, NAME_LEN, stdin);
        inventory[productNumber].name[strcspn(inventory[productNumber].name, "\n")] = '\0';

        printf("Product Price: ");
        scanf("%f", &inventory[productNumber].price);

        printf("Product Quantity: ");
        scanf("%d", &inventory[productNumber].quantity);
        getchar();
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
    *products = realloc(*products, (*count + 1) * sizeof(Product));
    if (*products == NULL) 
    {
        printf("Memory reallocation failed!\n");
        return;
    }

    Product *p = &((*products)[*count]);
    printf("\nEnter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &p->id);
    getchar();

    printf("Product Name: ");
    fgets(p->name, NAME_LEN, stdin);
    p->name[strcspn(p->name, "\n")] = '\0';

    printf("Product Price: ");
    scanf("%f", &p->price);

    printf("Product Quantity: ");
    scanf("%d", &p->quantity);
    getchar();

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
    for (int productViewed = 0; productViewed < count; productViewed++) 
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[productViewed].id, products[productViewed].name, products[productViewed].price, 
               products[productViewed].quantity);
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
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
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

    for (int productId = 0; productId < count; productId++) 
    {
        if (products[productId].id == id) 
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[productId].id, products[productId].name, products[productId].price, 
                   products[productId].quantity);
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

    printf("Enter name to search (partial name accepted): ");
    fgets(search, NAME_LEN, stdin);
    search[strcspn(search, "\n")] = '\0';

    printf("Products Found:\n");
    for (int productName = 0; productName < count; productName++) 
    {
        if (strstr(products[productName].name, search)) 
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[productName].id, products[productName].name, products[productName].price, 
                   products[productName].quantity);
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

    printf("Enter minimum price: ");
    scanf("%f", &minimum);
    printf("Enter maximum price: ");
    scanf("%f", &maximum);

    printf("Products in price range:\n");
    for (int productPrice = 0; productPrice < count; productPrice++)
     {
        if (products[productPrice].price >= minimum && products[productPrice].price <= maximum) 
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[productPrice].id, products[productPrice].name, products[productPrice].price, 
                   products[productPrice].quantity);
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

    for (int productid = 0; productid < *count; productid++) 
    {
        if ((*products)[productid].id == id) 
        {
            index = productid;
            break;
        }
    }

    if (index == -1) 
    {
        printf("Product not found!\n");
        return;
    }

    for (int productDelete = index; productDelete < *count - 1; productDelete++)
    {
        (*products)[productDelete] = (*products)[productDelete + 1];
    }

    (*count)--;
    *products = realloc(*products, (*count) * sizeof(Product));
    printf("Product deleted successfully!\n");
}

void freeMemory(Product *products) 
{
    free(products);
}
