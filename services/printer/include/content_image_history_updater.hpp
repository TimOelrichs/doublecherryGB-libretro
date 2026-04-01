//WIP ty to update content_image_history.lpl, so that print is show in images history


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// Path to the .lpl playlist file
const char* filename = "content_image_history.lpl";

// Example path for the new item to add
const char* new_path = "C:\\\\RetroArch-Win64-1.19.1\\\\screenshots\\\\new_image.png";

void add_item_to_lpl(const char* filepath) {
    // Open the file for reading
    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    // Read the entire file into memory
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);
    char* data = (char*)malloc(len + 1);
    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    // Parse the JSON content
    cJSON* root = cJSON_Parse(data);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON\n");
        free(data);
        return;
    }

    // Get the "items" array from the JSON object
    cJSON* items = cJSON_GetObjectItemCaseSensitive(root, "items");
    if (!cJSON_IsArray(items)) {
        fprintf(stderr, "\"items\" is not an array\n");
        cJSON_Delete(root);
        free(data);
        return;
    }
}

 
