#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

// Structure to represent a box header
typedef struct {
    uint32_t size;
    char type[5];
} BoxHeader;

// Function to print indentation based on the depth of the box
void print_indent(int depth) {
    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }
}

// Function to process sub-boxes recursively
void process_sub_boxes(FILE *file, int depth);

// Function to process a box header
void process_box(FILE *file, int depth) {
    BoxHeader box_header;

    // Read the box header
    if (fread(&box_header, sizeof(BoxHeader), 1, file) != 1) {
        return;
    }

    // Swap endianness if necessary
    box_header.size = ntohl(box_header.size);

    // Print box information
    print_indent(depth);
    printf("Box ID: %s of size %u\n", box_header.type, box_header.size);

    // Check box type and process accordingly
    if (strcmp(box_header.type, "moof") == 0 || strcmp(box_header.type, "traf") == 0) {
        process_sub_boxes(file, depth + 1);
    } else if (strcmp(box_header.type, "mdat") == 0) {
        // Extract and print MDAT content
        char *mdat_content = (char *)malloc(box_header.size - sizeof(BoxHeader) + 1);
        if (fread(mdat_content, box_header.size - sizeof(BoxHeader), 1, file) != 1) {
            free(mdat_content);
            return;
        }
        mdat_content[box_header.size - sizeof(BoxHeader)] = '\0'; // Null-terminate the string
        printf("Mdat content:\n%s\n", mdat_content);
        free(mdat_content);
    } else {
        // Skip the box contents for other box types
        fseek(file, box_header.size - sizeof(BoxHeader), SEEK_CUR);
    }
}

// Function to process sub-boxes recursively
void process_sub_boxes(FILE *file, int depth) {
    while (!feof(file)) {
        process_box(file, depth);
    }
}

int main() {
    // Open the file
    FILE *file = fopen("text0.mp4", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Process the top-level boxes
    process_sub_boxes(file, 0);

    // Close the file
    fclose(file);

    return 0;
}
