#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here
void reverse_string(char *buff, int str_len);
void word_print(char *buff, int str_len);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
        int i = 0;
        int consecutive_whitespace = 0;

    //removes leading white spaces
    while (*user_str == ' ' || *user_str == '\t') {
        user_str++;
    }

    //processes the rest of the string 
    while (*user_str != '\0' && i < len) { 
        if (*user_str == ' ' || *user_str == '\t') {
            if (!consecutive_whitespace) {
                buff[i++] = ' '; 
                consecutive_whitespace = 1;
            }
        } else {
            buff[i++] = *user_str; 
            consecutive_whitespace = 0;
        }
        user_str++;
    }

    //checks for max length if the string isn't at null yet
    if (*user_str != '\0') {
        //return -1 because string is too large
        return -1; 
    }

    //removes trailing space
    if (i > 0 && buff[i - 1] == ' ') {
        i--;
    }

    //populates rest of string with '.'
    while (i < len) {
        buff[i++] = '.';
    }

    //return the length of string or error
    return i;
}

void print_buff(char *buff, int len){
    //changed print buff to match tests
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len){
    //YOU MUST IMPLEMENT
    int word_count = 0;
    int is_word = 0;

    //loops through buffer
    for (int i = 0; i < len; i++) {
        //checks for non space / .
        if (buff[i] != ' ' && buff[i] != '.') {
            //checks if in a word or going to a new word
            if (!is_word) {
                word_count++;
                is_word = 1;
            }
        } else {
            //reset otherwise
            is_word = 0;
        }
    }
    //return count
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int str_len) {
    int start = 0;    
    //get end of length of string              
    int end = str_len - 1;          
    char temp;

    //iterate until you get to the end of the string
    while (start < end) {
        //skips the leading '.'
        while (start < end && buff[start] == '.') {
            start++;
        }

        //skips the ending '.'
        while (start < end && buff[end] == '.') {
            end--;
        }

        //swaps characters from end and beginning until reversed
        if (start < end) {
            temp = buff[start];
            buff[start] = buff[end];
            buff[end] = temp;
            start++;
            end--;
        }
    }
}

void word_print(char *buff, int str_len) {
    int start = 0;
    int count = 0;

    printf("Word Print\n----------\n");

    //iterates through buffer
    for (int i = 0; i <= str_len; i++) {
        //checks if there is a space, ., or null terminator
        if (buff[i] == ' ' || buff[i] == '.' || buff[i] == '\0') {
            //checks if the word is valid or not
            if (i > start) {
                //if so, increment and print the current number of the word
                count++;
                printf("%d. ", count);
                //then print the entire word by iterating through it
                for (int j = start; j < i; j++) {
                    putchar(buff[j]);
                }
                //print the lenght of the word
                printf("(%d)\n", i - start);
            }
            //iterate to go to next word
            start = i + 1;
        }
    }
    printf("\n");
    printf("Number of words returned: %d\n", count);
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //       safe because it checks if argc is less than two (meaning there are at less than two arguments) 
    //       and if it is, then argv[1] will not exist and it will call usage to exit before accessing argv[1]
    //       so the second part of the condition is safe because we know argv[1] exists because of the first part
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //       the if statement below checks if there is less than 3 arguments and if there is, exit 
    //       because we're expecting three arguments including the program name (./string.fun), 
    //       the operation (-c or -r), and the string to process
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (!buff) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);  // Reverse only the string content
            break;

        case 'w':
            word_print(buff, user_str_len);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          It is good practice because we can make sure that the buffer's size
//          is accurate instead of relying on or assuming that it is 50 bytes
//          because of what we initilized in main. Also, passing the buffer
//          and lenght lets us change the size of the buffer in case we want
//          to in the future. Finally, it prevents the threat of buffer overflows