#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ID 10000

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
        if (product.id == (*root)->product.id) {
            return; // Product with the same ID already exists, no need to insert again
        } else if (product.id < (*root)->product.id) {
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

    int maxLines = 100;  // Maximum number of lines to read
    int lineCount = 0;
    Product* products = (Product*)malloc(maxLines * sizeof(Product));  // Dynamic allocation

    while (lineCount < maxLines && !feof(file)) {
        int result = fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %f | %f | %d\n",
                            &products[lineCount].id, products[lineCount].productName,
                            products[lineCount].brandName, products[lineCount].productCategory,
                            &products[lineCount].buyPrice, &products[lineCount].sellPrice,
                            &products[lineCount].stock);

        if (result == 7) {
            lineCount++;
        } else if (result == EOF) {
            break;  // Reached end of file
        } else {
            printf("Error reading line %d\n", lineCount + 1);
            break;
        }
    }

    fclose(file);

    // Insert the products into the BST
    for (int i = 0; i < lineCount; i++) {
        insertProduct(root, products[i]);
    }

    free(products);  // Free the dynamically allocated memory
}


void displayAllProducts(BSTNode* root) {
    if (root == NULL) {
        printf("Warehouse is empty!\n");
    } else {
        printf("\n");
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
    printf("-----------------------\n");
    printf("    Add New Product\n");
    printf("-----------------------\n\n");

    printf("Product Name: ");
    scanf(" %[^\n]s", product->productName);

    printf("\nBrand Name:\n");
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

    printf("\nProduct Category:\n");
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

    printf("\nBuy Price: ");
    scanf("%f", &product->buyPrice);

    printf("Sell Price: ");
    scanf("%f", &product->sellPrice);

    printf("Stock: ");
    scanf("%d", &product->stock);

    printf("\nNew product added successfully!\n");
}


bool isIdExist(int id, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int existingId;
    while (fscanf(file, "%d", &existingId) == 1) {
        if (existingId == id) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}


// Function to generate a unique ID for each product
int generateProductId(const char* filename) {
    srand(time(NULL));
    int id;
    bool isUnique;

    do {
        id = rand() % 9000 + 1000;  // Generate a random number between 1000 and 9999
        isUnique = !isIdExist(id, filename);
    } while (!isUnique);

    return id;
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


void searchProduct(BSTNode* root, int id) {
    if (root == NULL) {
        printf("Product not found.\n");
    } else if (id == root->product.id) {
        printf("\n------------------------------------------------------------------------------------------------------------\n");
        printf("|  ID  | Nama Barang          | Nama Brand      | Kategori             | Harga Beli  | Harga Jual  | Stock |\n");
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               root->product.id, root->product.productName, root->product.brandName,
               root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
               root->product.stock);
               getchar();
        printf("------------------------------------------------------------------------------------------------------------\n");
    } else if (id < root->product.id) {
        searchProduct(root->left, id);
    } else {
        searchProduct(root->right, id);
    }
}


void writeInOrder(BSTNode* root, FILE* file) {
    if (root != NULL) {
        writeInOrder(root->left, file);
        fprintf(file, "%d | %s | %s | %s | %.2f | %.2f | %d\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        writeInOrder(root->right, file);
    }
}

void updateProductStock(BSTNode* root, int id, int newStock) {
    if (root == NULL) {
        printf("Product not found.\n");
        return;
    }

    if (id == root->product.id) {
        root->product.stock = newStock;
        printf("\nProduct stock updated successfully!\n");

        // Update the products.txt file
        FILE* file = fopen("products.txt", "r");
        FILE* tempFile = fopen("temp.txt", "w");
        if (file == NULL || tempFile == NULL) {
            printf("Failed to open the file.\n");
            return;
        }

        // Copy the data to a temporary file, updating the stock for the matching product ID
        char line[200];
        while (fgets(line, sizeof(line), file)) {
            int currentId;
            sscanf(line, "%d", &currentId);

            if (currentId == id) {
                fprintf(tempFile, "%d | %s | %s | %s | %.2f | %.2f | %d\n",
                        root->product.id, root->product.productName, root->product.brandName,
                        root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                        root->product.stock);
            } else {
                fputs(line, tempFile);
            }
        }

        fclose(file);
        fclose(tempFile);

        // Rename the temporary file to overwrite the original file
        remove("products.txt");
        rename("temp.txt", "products.txt");
    } else if (id < root->product.id) {
        updateProductStock(root->left, id, newStock);
    } else {
        updateProductStock(root->right, id, newStock);
    }
}


void deleteProductConfirmation(BSTNode** rootPtr, int id) {
    if (*rootPtr == NULL) {
        printf("Product not found.\n");
    } else if (id == (*rootPtr)->product.id) {
        printf("\nAre you sure you want to delete this product?\n");
        printf("\n------------------------------------------------------------------------------------------------------------\n");
        printf("|  ID  | Nama Barang          | Nama Brand      | Kategori             | Harga Beli  | Harga Jual  | Stock |\n");
        // -------
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               (*rootPtr)->product.id, (*rootPtr)->product.productName, (*rootPtr)->product.brandName,
               (*rootPtr)->product.productCategory, (*rootPtr)->product.buyPrice, (*rootPtr)->product.sellPrice,
               (*rootPtr)->product.stock);
        // -------
        printf("------------------------------------------------------------------------------------------------------------\n");
        printf("\nEnter 'Y' to confirm deletion, or any other key to cancel: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            // Delete the product
            BSTNode* current = *rootPtr;
            BSTNode* parent = NULL;
            BSTNode* successor;

            // Case 1: No child or one child
            if (current->left == NULL) {
                *rootPtr = current->right;
                free(current);
            } else if (current->right == NULL) {
                *rootPtr = current->left;
                free(current);
            }
            // Case 2: Two children
            else {
                successor = current->right;
                while (successor->left != NULL) {
                    parent = successor;
                    successor = successor->left;
                }
                current->product = successor->product;

                if (parent != NULL)
                    parent->left = successor->right;
                else
                    current->right = successor->right;

                free(successor);
            }

            printf("Product deleted successfully!\n");

            // Update the products.txt file
            FILE* file = fopen("products.txt", "r");
            FILE* tempFile = fopen("temp.txt", "w");
            if (file == NULL || tempFile == NULL) {
                printf("Failed to open the file.\n");
                return;
            }

            // Copy the data to a temporary file, excluding the deleted product
            char line[200];
            while (fgets(line, sizeof(line), file)) {
                int currentId;
                sscanf(line, "%d", &currentId);

                if (currentId != id) {
                    fputs(line, tempFile);
                }
            }

            fclose(file);
            fclose(tempFile);

            // Rename the temporary file to overwrite the original file
            remove("products.txt");
            rename("temp.txt", "products.txt");
        } else {
            printf("Deletion canceled.\n");
        }
    } else if (id < (*rootPtr)->product.id) {
        deleteProductConfirmation(&((*rootPtr)->left), id);
    } else {
        deleteProductConfirmation(&((*rootPtr)->right), id);
    }
}


// Function to reset password and update the text file
void resetPassword(User* user) {
    char newPassword[20];
    char confirmPassword[20];

    do {
        printf("\nEnter new password: ");
        scanf("%s", newPassword);

        printf("Confirm new password: ");
        scanf("%s", confirmPassword);

        if (strcmp(newPassword, confirmPassword) != 0) {
            printf("\nPasswords do not match. Please try again.\n");
        }
    } while (strcmp(newPassword, confirmPassword) != 0);

    strcpy(user->password, newPassword);
    printf("\nPassword reset successfully!\n");
    getch();

    // Update the text file with the new password
    FILE* file = fopen("users.txt", "w");
    fprintf(file, "%s %s %s %s", user->username, user->password, user->securityAnswer1, user->securityAnswer2);
    fclose(file);
}


void generateReportByStockDescending(BSTNode* root, FILE* file) {
    if (root != NULL) {
        generateReportByStockDescending(root->right, file);
        fprintf(file, "| %-3d | %-20s | %-15s | %-20s | $%-9.2f | $%-9.2f | %-5d |\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        generateReportByStockDescending(root->left, file);
    }
}


void generateReportByStockAscending(BSTNode* root, FILE* file) {
    if (root != NULL) {
        generateReportByStockAscending(root->left, file);
        fprintf(file, "| %-3d | %-20s | %-15s | %-20s | $%-9.2f | $%-9.2f | %-5d |\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        generateReportByStockAscending(root->right, file);
    }
}


void generateReportByStock(BSTNode* root, int order) {
    FILE* file;
    if (order == 1)
        file = fopen("report_stock_most_to_least.txt", "w");
    else
        file = fopen("report_stock_least_to_most.txt", "w");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    fprintf(file, "| %-3s  | %-20s | %-15s | %-20s | %-10s | %-10s | %-5s |\n",
            "ID", "Product Name", "Brand Name", "Product Category", "Buy Price", "Sell Price", "Stock");

    if (order == 1)
        generateReportByStockDescending(root, file);
    else
        generateReportByStockAscending(root, file);

    fclose(file);
}


// Function to display the main menu
void displayMainMenu(BSTNode* root) {
    printf("========================================================\n");
    printf("                   XYZ STORE WAREHOUSE\n");
    printf("========================================================\n\n");
//    printf(" ------------------------------------------------------------ \n");
//    printf("| Low on stock:                                              |\n");
    printf("Low on stock:\n");


    bool hasLowStock = false;
    checkLowStock(root, &hasLowStock); // Check low stock products
//    printf(" ------------------------------------------------------------ \n");

    if (!hasLowStock) {
        printf("No product is low on stock.\n");
    }

    printf("\n1. New Product\n");
    printf("2. Show All Product\n");
    printf("3. Generate Reports\n");
    printf("4. Exit\n");
    printf("\nEnter your choice: ");
}


void checkLowStock(BSTNode* root, bool* hasLowStock) {
    if (root != NULL) {
        checkLowStock(root->left, hasLowStock);
        if (root->product.stock < 3) {
            printf("ID= %-3d | Product Name= %-20s | Stock= %-3d\n",
                   root->product.id, root->product.productName, root->product.stock);
            *hasLowStock = true;
        }
        checkLowStock(root->right, hasLowStock);
    }
}


int main() {
    User user;
    Product product;
    BSTNode* root = NULL;
    loadProductsFromFile(&root);

    // File to load the pre determined administrator account data
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

    // If the user entered wrong password 3 times in a row, then forgot password feature will pop up.
    int counter = 1;

    // Administrator Login Panel
    while (1) {
        printf("================================\n");
        printf("      Administrator Login\n");
        printf("================================\n");
        printf("\nUsername: ");
        scanf("%s", enteredUsername);
        printf("Password: ");
        scanf("%s", enteredPassword);

        if (strcmp(enteredUsername, user.username) == 0 && strcmp(enteredPassword, user.password) == 0) {
            printf("\nLogin successful! Press enter to continue...");
            getch();
            system("cls");
            break;
        } else if (counter == 3){
            printf("\nInvalid username or password.\n");
            printf("\nForgot password? (Y/N): ");
            scanf(" %c", &choice);

            if (choice == 'Y' || choice == 'y') {
                char securityAnswer1[50];
                char securityAnswer2[50];

                system("cls");
                printf("===========================================\n");
                printf("         Enter Additional Security\n");
                printf("===========================================\n");
                printf("\n1. When is your birthday? (dd/mm/yyyy): ");
                scanf(" %[^\n]s", securityAnswer1);
                printf("2. What is your last name?: ");
                scanf(" %[^\n]s", securityAnswer2);

                if (strcmp(securityAnswer1, user.securityAnswer1) == 0 && strcmp(securityAnswer2, user.securityAnswer2) == 0) {
                    resetPassword(&user);
                    system("cls");
                } else {
                    printf("\nIncorrect security answer. Password reset failed.");
                    getch();
                    system("cls");
                }
            }
            system("cls");
        } else {
            counter++;
            printf("\nInvalid username or password.");
            getch();
            system("cls");
        }
    }

    int input;
    int menuChoice;
    int id;
    int productId, newStock;
    char opt;

    do {
        int flagMenu2 = 1, flagMenu3 = 1;

        // To load the text file content into BST
        loadProductsFromFile(&root);
        displayMainMenu(root);
        scanf("%d", &menuChoice);

        switch (menuChoice) {
            case 1:
                while (1){
                    system("cls");
                    addNewProduct(&product);
                    product.id = generateProductId("products.txt");
                    writeProductDataToFile(&product);

                    printf("\nEnter another product? [Y/N]: ");
                    scanf(" %c", &opt);
                    if (opt == 'Y' || opt == 'y')
                    {
                        continue;
                    }
                    else
                        break;
                }
                system("cls");
                break;
            case 2:
                while (flagMenu2) {
                    system("cls");
                    printf("|  ID  | Nama Barang          | Nama Brand      | Kategori             | Harga Beli  | Harga Jual  | Stock |\n");
                    printf("==============================================================================================================");
                    displayAllProducts(root);

                    // RUD functionality
                    printf("--------------------------------------------------------------------------------------------------------------\n");
                    printf("\n0. Back To Menu\t\n");
                    printf("1. Search\t\n");
                    printf("2. Update\t\n");
                    printf("3. Delete\t\n");
                    printf("\nChoice: ");
                    scanf("%d", &input);

                    switch (input) {
                        case 1:
                            while(1) {
                                system("cls");
                                printf("Enter product ID to search: ");
                                scanf("%d", &id);
                                searchProduct(root, id);

                                char opt;
                                printf("\nSearch another product? [Y/N]: ");
                                scanf(" %c", &opt);
                                getchar();
                                if (opt == 'Y' || opt =='y')
                                    continue;
                                else {
                                    BSTNode* root = NULL;
                                    loadProductsFromFile(&root);
                                    break;
                                }
                            }
                            system("cls");
                            break;
                        case 2:
                            while(1) {
                                system("cls");
                                printf("--- Update Product Stock ---\n");
                                printf("Enter the product ID: ");
                                scanf("%d", &productId);
                                printf("Enter the new stock: ");
                                scanf("%d", &newStock);
                                updateProductStock(root, productId, newStock);

                                char opt;
                                    printf("\nUpdate another product? [Y/N]: ");
                                    scanf(" %c", &opt);
                                    getchar();
                                    if (opt == 'Y' || opt =='y')
                                        continue;
                                    else {
                                        BSTNode* root = NULL;
                                        loadProductsFromFile(&root);
                                        break;
                                    }
                            }
                            system("cls");
                            break;
                        case 3:
                            while(1){
                            system("cls");
                            printf("Enter product ID to delete: ");
                            scanf("%d", &id);
                            deleteProductConfirmation(&root, id);

                            char opt;
                                    printf("\nDelete another product? [Y/N]: ");
                                    scanf(" %c", &opt);
                                    getchar();
                                    if (opt == 'Y' || opt =='y')
                                        continue;
                                    else {
                                        BSTNode* root = NULL;
                                        loadProductsFromFile(&root);
                                        break;
                                    }
                            }
                            system("cls");
                            break;
                        case 0:
                            flagMenu2 = 0;
                            system("cls");
                            break;
                        }
                        if (flagMenu2 == 0)
                            break;
                    }
                    system("cls");
                    break;
            case 3:
                while(flagMenu3) {
                    system("cls");
                    printf("-------------------------\n");
                    printf("    Generate Reports\n");
                    printf("-------------------------\n\n");
                    printf("0. Back to menu\n");
                    printf("1. Generate report from high to lower ID\n");
                    printf("2. generate report from lower to high ID\n\n");
                    printf("Choice: ");
                    scanf("%d", &input);
                    getchar();
                    switch (input) {
                        case 1:
                            generateReportByStock(root, 1);
                            printf("\nReport printed successfuly.\n");
                            break;
                        case 2:
                            generateReportByStock(root, 0);
                            printf("\nReport printed successfuly.\n");
                            break;
                        case 0:
                            flagMenu3 = 0;
                            system("cls");
                            break;
                    }

                    if (flagMenu3 == 0)
                        break;

                    char opt;
                        printf("\nGenerate another reports? [Y/N]: ");
                        scanf(" %c", &opt);
                        getchar();
                        if (opt == 'Y' || opt =='y')
                            continue;
                        else
                            break;
                }
                system("cls");
                break;
            case 4:
                printf("\nExiting...\n");
                exit(1);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (menuChoice != 5);

    deleteBST(root);

    return 0;
}
