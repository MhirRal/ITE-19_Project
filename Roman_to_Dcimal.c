#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100    // Maximum length for each line in the input file
#define MAX_WORD_LENGTH 50     // Maximum length for words (used in decimalToWords)

// Function prototypes
int romanToDecimal(const char *roman);     // Converts Roman numeral to decimal
void decimalToWords(int num, char *words); // Converts decimal number to words
int performOperation(int num1, int num2, char operator); // Performs arithmetic operation
void processLine(const char *line, FILE *outputFile);  // Processes a single line of input
void parseRomanNumerals(const char *line, char *roman1, char *roman2, char *operator); // Parses a line into Roman numerals and operator

// Function to convert Roman numerals to decimal
int romanToDecimal(const char *roman) {
    int values[256] = {0}; // Array to hold Roman numeral character values
    values['I'] = 1;       // Value for Roman numeral 'I'
    values['V'] = 5;       // Value for Roman numeral 'V'
    values['X'] = 10;      // Value for Roman numeral 'X'
    values['L'] = 50;      // Value for Roman numeral 'L'
    values['C'] = 100;     // Value for Roman numeral 'C'
    values['D'] = 500;     // Value for Roman numeral 'D'
    values['M'] = 1000;    // Value for Roman numeral 'M'

    int total = 0;         // Variable to hold the total value
    for (int i = 0; i < strlen(roman); i++) {
        // If the current character is smaller than the next, subtract its value (Roman numeral rule)
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else {
            total += values[roman[i]];  // Otherwise, add its value
        }
    }
    return total; // Return the calculated total value in decimal
}

// Function to convert a decimal number to its word representation
void decimalToWords(int num, char *words) {
    if (num < 0) {  // If the number is negative
        strcat(words, "Negative "); // Add "Negative" to the beginning of the words string
        num = -num; // Convert the number to positive for easier processing
    }

    // Arrays to represent units, teens, tens, and thousands in words
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};

    int parts[4] = {0};  // Array to store chunks of the number (e.g., thousands, hundreds, etc.)
    int partIndex = 0;   // Index for parts array
    while (num > 0) {
        parts[partIndex++] = num % 1000; // Store the last 3 digits in the current chunk
        num /= 1000; // Remove the last 3 digits for the next chunk
    }

    words[0] = '\0'; // Initialize the words string as empty

    for (int i = partIndex - 1; i >= 0; i--) {  // Loop through each part in reverse order (thousands, hundreds, etc.)
        int n = parts[i];  // Get the current chunk of the number
        if (n == 0) continue; // Skip if the chunk is zero

        char part[MAX_WORD_LENGTH] = "";  // Temporary string to store the word representation of the current chunk

        if (n / 100 > 0) {  // If the chunk has hundreds
            sprintf(part + strlen(part), "%s Hundred ", units[n / 100]);  // Add "X Hundred" (e.g., "One Hundred")
            n %= 100;  // Remove the hundreds part from the chunk
        }
        if (n >= 10 && n < 20) {  // Handle numbers between 10 and 19 (teens)
            sprintf(part + strlen(part), "%s ", teens[n - 10]);  // Add "Ten" to "Nineteen"
        } else {  
            if (n / 10 > 0) {  // Handle tens (20, 30, etc.)
                sprintf(part + strlen(part), "%s ", tens[n / 10]);  // Add "Twenty", "Thirty", etc.
            }
            if (n % 10 > 0) {  // Handle units (1, 2, etc.)
                sprintf(part + strlen(part), "%s ", units[n % 10]);  // Add "One", "Two", etc.
            }
        }
        if (strlen(thousands[i]) > 0) {  // If there is a thousand or million suffix
            sprintf(part + strlen(part), "%s ", thousands[i]);  // Add "Thousand", "Million", etc.
        }
        strcat(words, part);  // Append the current chunk's word representation to the full words string
    }
}

// Function to perform arithmetic operations
int performOperation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;  // Addition
        case '-': return num1 - num2;  // Subtraction
        case '*': return num1 * num2;  // Multiplication
        default: return 0;  // Return 0 for invalid or unsupported operators
    }
}

// Function to parse a line and extract Roman numerals and the operator
void parseRomanNumerals(const char *line, char *roman1, char *roman2, char *operator) {
    sscanf(line, "%s %c %s", roman1, operator, roman2);  // Extract two Roman numerals and the operator from the line
}

// Function to process a single line of input
void processLine(const char *line, FILE *outputFile) {
    char roman1[MAX_LINE_LENGTH], roman2[MAX_LINE_LENGTH], operator;
    parseRomanNumerals(line, roman1, roman2, &operator);  // Parse the line into two Roman numerals and the operator

    int num1 = romanToDecimal(roman1);  // Convert the first Roman numeral to decimal
    int num2 = romanToDecimal(roman2);  // Convert the second Roman numeral to decimal
    int result = performOperation(num1, num2, operator);  // Perform the operation

    if (result == 0 && operator != '+') {  // Handle invalid or unsupported operations (only addition is supported)
        fprintf(outputFile, "Invalid Operation\n");  // Write "Invalid Operation" to the output file
        return;
    }

    char resultWords[MAX_LINE_LENGTH] = "";  // Initialize an empty string to store the result in words
    decimalToWords(result, resultWords);  // Convert the result to words

    fprintf(outputFile, "%s\n", resultWords);  // Write the result in words to the output file
}

int main() {
    FILE *inputFile = fopen("Input.txt", "r");  // Open the input file for reading
    FILE *outputFile = fopen("Output.txt", "w");  // Open the output file for writing

    if (!inputFile || !outputFile) {  // Check if either file could not be opened
        printf("Error: Unable to open input or output file.\n");  // Print error message
        return 1;  // Return with error code
    }

    char line[MAX_LINE_LENGTH];  // Variable to store each line from the input file
    while (fgets(line, sizeof(line), inputFile)) {  // Read each line from the input file
        if (strlen(line) > 1) {  // Skip empty lines (lines with only a newline character)
            processLine(line, outputFile);  // Process the current line and write the result to the output file
        }
    }

    fclose(inputFile);  // Close the input file
    fclose(outputFile);  // Close the output file

    printf("Processing complete. Results saved to Output.txt.\n");  // Print completion message
    return 0;  // Return successfully
}
