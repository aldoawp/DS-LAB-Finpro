#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold user information
typedef struct {
    char username[20];
    char password[20];
    char securityAnswer1[50];
    char securityAnswer2[50];
} User;

// Structure to hold product information
typedef struct {
    int id;
    char productName[50];
    char brandName[20];
    char productCategory[20];
    float buyPrice;
    float sellPrice;
    int stock;
} Product;

typedef struct BSTNode {
    Product product;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

void displayInOrder(BSTNode* root) {
    if (root != NULL) {
        displayInOrder(root->left);
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               root->product.id, root->product.productName, root->product.brandName,
               root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
               root->product.stock);
        displayInOrder(root->right);
    }
}


void insertProduct(BSTNode** root, Product product) {
    if (*root == NULL) {
        *root = (BSTNode*)malloc(sizeof(BSTNode));
        (*root)->product = product;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        if (product.id < (*root)->product.id) {
            insertProduct(&(*root)->left, product);
        } else {
            insertProduct(&(*root)->right, product);
        }
    }
}

void loadProductsFromFile(BSTNode** root) {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    Product product;
    int lineCount = 0;
    while (1) {
        int result = fscanf(file, "%d | %[^\n]s | %[^\n]s | %[^\n]s | %f | %f | %d\n", &product.id, product.productName, product.brandName, product.productCategory, &product.buyPrice, &product.sellPrice, &product.stock);
        if (result == EOF) {
            break;  // Reached end of file
        } else if (result < 7) {
            insertProduct(root, product);
        } else {
            // Invalid line format or error occurred
            printf("Error reading line %d\n", lineCount + 1);
            break;
        }
    }


//    while (fscanf(file, "%d | %[^\n]s | %[^\n]s | %[^\n]s | %f | %f | %d\n", &product.id, product.productName, product.brandName,
//                  product.productCategory, &product.buyPrice, &product.sellPrice, &product.stock) < 7) {
//        insertProduct(root, product);
//    }


    fclose(file);
}

void displayAllProducts(BSTNode* root) {
    if (root == NULL) {
        printf("Warehouse is empty!\n");
    } else {
        printf("All Products:\n");
        displayInOrder(root);
    }
}

void deleteBST(BSTNode* root) {
    if (root == NULL) {
        return;
    }

    // Delete left and right subtrees
    deleteBST(root->left);
    deleteBST(root->right);

    // Delete the current node
    free(root);
}

int isIdExists(BSTNode* root, int id) {
    if (root == NULL) {
        return 0; // ID not found
    }

    if (id == root->product.id) {
        return 1; // ID found
    } else if (id < root->product.id) {
        return isIdExists(root->left, id);
    } else {
        return isIdExists(root->right, id);
    }
}


// Function to add a new product
void addNewProduct(Product* product) {
    printf("--- Add New Product ---\n");

    printf("Product Name: ");
    scanf(" %[^\n]s", product->productName);

    printf("Brand Name:\n");
    printf("1. Polytron\n");
    printf("2. LG\n");
    printf("3. Sanken\n");
    printf("4. Miyako\n");
    printf("5. Samsung\n");
    printf("6. Sony\n");
    int brandChoice;
    printf("Enter your choice: ");
    scanf("%d", &brandChoice);

    switch (brandChoice) {
        case 1:
            strcpy(product->brandName, "Polytron");
            break;
        case 2:
            strcpy(product->brandName, "LG");
            break;
        case 3:
            strcpy(product->brandName, "Sanken");
            break;
        case 4:
            strcpy(product->brandName, "Miyako");
            break;
        case 5:
            strcpy(product->brandName, "Samsung");
            break;
        case 6:
            strcpy(product->brandName, "Sony");
            break;
        default:
            printf("Invalid choice. Brand Name set to Unknown.\n");
            strcpy(product->brandName, "Unknown");
            break;
    }

    printf("Product Category:\n");
    printf("1. Kulkas\n");
    printf("2. Mesin Cuci\n");
    printf("3. Kompor Listrik\n");
    printf("4. Magic Com\n");
    printf("5. LED TV\n");
    int categoryChoice;
    printf("Enter your choice: ");
    scanf("%d", &categoryChoice);

    switch (categoryChoice) {
        case 1:
            strcpy(product->productCategory, "Kulkas");
            break;
        case 2:
            strcpy(product->productCategory, "Mesin Cuci");
            break;
        case 3:
            strcpy(product->productCategory, "Kompor Listrik");
            break;
        case 4:
            strcpy(product->productCategory, "Magic Com");
            break;
        case 5:
            strcpy(product->productCategory, "LED TV");
            break;
        default:
            printf("Invalid choice. Product Category set to Unknown.\n");
            strcpy(product->productCategory, "Unknown");
            break;
    }

    printf("Buy Price: ");
    scanf("%f", &product->buyPrice);

    printf("Sell Price: ");
    scanf("%f", &product->sellPrice);

    printf("Stock: ");
    scanf("%d", &product->stock);

    printf("New product added successfully!\n");
}

// Function to generate a unique ID for each product
int generateProductId() {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return -1;
    }

    int maxId = 0;
    int currentId;
    while (fscanf(file, "%d", &currentId) == 1) {
        if (currentId > maxId) {
            maxId = currentId;
        }
        fscanf(file, "%*[^\n]"); // Skip to the end of the line
    }
    fclose(file);

    return maxId + 1;
}

// Function to write product data to a text file
void writeProductDataToFile(Product* product) {
    FILE* file = fopen("products.txt", "a");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    fprintf(file, "%d | %s | %s | %s | %.2f | %.2f | %d\n", product->id, product->productName, product->brandName,
            product->productCategory, product->buyPrice, product->sellPrice, product->stock);
    fclose(file);
}

// Function to reset password and update the text file
void resetPassword(User* user) {
    char newPassword[20];
    char confirmPassword[20];

    do {
        printf("Enter new password: ");
        scanf("%s", newPassword);

        printf("Confirm new password: ");
        scanf("%s", confirmPassword);

        if (strcmp(newPassword, confirmPassword) != 0) {
            printf("Passwords do not match. Please try again.\n");
        }
    } while (strcmp(newPassword, confirmPassword) != 0);

    strcpy(user->password, newPassword);
    printf("Password reset successfully!\n");
    getch();

    // Update the text file with the new password
    FILE* file = fopen("users.txt", "w");
    fprintf(file, "%s %s %s %s", user->username, user->password, user->securityAnswer1, user->securityAnswer2);
    fclose(file);
}

// Function to display the main menu
void displayMainMenu() {
    printf("\n--- Main Menu ---\n");
    printf("1. New Product\n");
    printf("2. Show All Product\n");
    printf("3. Generate Reports\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    User user;
    Product product;
    BSTNode* root = NULL;

    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    fscanf(file, "%s %s %s %s", user.username, user.password, user.securityAnswer1, user.securityAnswer2);
    fclose(file);

    char enteredUsername[20];
    char enteredPassword[20];
    char choice = 'N';

    while (1) {
        printf("Username: ");
        scanf("%s", enteredUsername);
        printf("Password: ");
        scanf("%s", enteredPassword);

        if (strcmp(enteredUsername, user.username) == 0 && strcmp(enteredPassword, user.password) == 0) {
            printf("Login successful!\n");
            getch();
            system("cls");
            break;
        } else {
            printf("Invalid username or password.\n");

            printf("Forgot password? (Y/N): ");
            scanf(" %c", &choice);

            if (choice == 'Y' || choice == 'y') {
                char securityAnswer1[50];
                char securityAnswer2[50];

                system("cls");
                printf("Please enter additional security below to continue\n");
                printf("When is your birthday? (dd/mm/yyyy): ");
                scanf(" %[^\n]s", securityAnswer1);
                printf("What is your last name?: ");
                scanf(" %[^\n]s", securityAnswer2);

                if (strcmp(securityAnswer1, user.securityAnswer1) == 0 && strcmp(securityAnswer2, user.securityAnswer2) == 0) {
                    resetPassword(&user);
                    system("cls");
                } else {
                    printf("Incorrect security answer. Password reset failed.\n");
                    getch();
                    system("cls");
                }
            }
            system("cls");
        }
    }

    // To load the text file content into BST
    loadProductsFromFile(&root);

    int menuChoice;
    do {
        displayMainMenu();
        scanf("%d", &menuChoice);

        switch (menuChoice) {
            case 1:
                system("cls");
                addNewProduct(&product);
                product.id = generateProductId();
                writeProductDataToFile(&product);
                break;
            case 2:
                system("cls");
                displayAllProducts(root);
                break;
            case 3:
                printf("Generate Reports\n");
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        getch();
        system("cls");

    } while (menuChoice != 4);

    return 0;
}
