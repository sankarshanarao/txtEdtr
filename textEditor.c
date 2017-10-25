
#include <stdio.h>
#include <stdlib.h>

#define OP_INSERT_CHAR 		'I' //Insert
#define OP_PRESSED_ENTER 	'N' //Newline
#define OP_PRESSED_DELETE 	'D' //Delete
#define OP_PRESSED_LEFT 	'h' //Left arrow equivalent in vi editor
#define OP_PRESSED_DOWN 	'j' //Down arrow equivalent in vi editor
#define OP_PRESSED_UP 		'k' //Up arrow equivalent in vi editor
#define OP_PRESSED_RIGHT 	'l' //Right arrow equivalent in vi editor
#define OP_PRESSED_HOME 	'H' //Home
#define OP_PRESSED_END 		'E' //End
#define OP_PRINT_LINE 		'P' //Print the current line
#define OP_PRINT_TEXT 		'T' //print the whole Text
#define OP_QUIT 			'Q' //Quit the editor

//Forward declaration; see the definition of struct for more details.
struct text_node;

//A node in
struct line_node {
	char ch;
	struct line_node *next_char;
	struct line_node *prev_char;
	struct text_node *line_header;
};

struct text_node {
	struct line_node *line_begin;
	struct text_node *next_line;
	struct text_node *prev_line;
};

typedef struct line_node LINE;
typedef struct text_node TEXT;


//Creates the empty text.
//Empty text has one empty line, which has a newline '\n' as end of line.
struct text_node *create_text(struct line_node **pcursor);

//Free the whole text
void free_text(struct text_node *text);

//Insert a char just before the cursor.
//Cursor always points to a line_node.
//A line has a newline char as the end of line.
struct line_node *op_insert_char(struct line_node *cursor, char ch);

//“Enter” (i.e. create) a new line and the cursor moves to the new line.
//If the cursor was at the end of line, a new empty line is created, otherwise
//the current line is split into a new line starting from the character at the cursor.
struct line_node *op_pressed_enter(struct line_node *cursor);

//“Delete” a character at the cursor, or the end of line if the cursor is
//at the end of a line. Deleting an end of line is essentially concatenating
//the next line with the current line.
//No operation if the cursor is at the end of line of the last line.
struct line_node *op_pressed_delete(struct line_node *cursor);

//Move the cursor “Left” by a character (or to the end of line of
//above line if the cursor is at the beginning of a line).
//No operation if the cursor is the beginning of the first line.
struct line_node *op_pressed_left(struct line_node *cursor);

//Move the cursor “Down” to the below line (beginning of the below line).
//No operation if the cursor is already at the last line.
struct line_node *op_pressed_down(struct line_node *cursor);

//Move the cursor “Up” to the above line (beginning of the above line).
//No operation if the cursor is already at the first line.
struct line_node *op_pressed_up(struct line_node *cursor);

//Move the cursor “Right” by a character (or to the beginning of below line
//if the cursor is at the end of a line).
//No operation if the cursor is the end of line of the last line.
struct line_node *op_pressed_right(struct line_node *cursor);

//Move the cursor to “Home”, that is, beginning of the current line.
//No operation if the cursor is at the beginning of a line.
struct line_node *op_pressed_home(struct line_node *cursor);

//Move the cursor to “End”, that is, end of line the current line.
//No operation if the cursor is at the end of a line.
struct line_node *op_pressed_end(struct line_node *cursor);

//“Print Line” prints the current line.
void op_print_line(struct line_node *cursor);

//“Print Text” prints the whole text.
void op_print_text(struct text_node *text);

int main() {
	struct line_node *cursor;
	struct text_node *text = create_text(&cursor);


//    op_insert_char(cursor,'a');
//    op_insert_char(cursor,'b');
//	printf("%d,%d,%d",text,text,text->line_begin->line_header);
//	op_print_line(text->line_begin);






	char op[4];

	do {
		scanf(" %[^\n]", op);
		switch(op[0]) {
	        case OP_INSERT_CHAR:
	            cursor = op_insert_char(cursor, op[2]);
	            break;
	        case OP_PRESSED_ENTER:
	            cursor = op_pressed_enter(cursor);
	            break;
	        case OP_PRESSED_DELETE:
	            cursor = op_pressed_delete(cursor);
	            break;
	        case OP_PRESSED_LEFT:
	            cursor = op_pressed_left(cursor);
	            break;
	        case OP_PRESSED_DOWN:
	            cursor = op_pressed_down(cursor);
	            break;
	        case OP_PRESSED_UP:
	            cursor = op_pressed_up(cursor);
	            break;
	        case OP_PRESSED_RIGHT:
	            cursor = op_pressed_right(cursor);
	            break;
	        case OP_PRESSED_HOME:
	            cursor = op_pressed_home(cursor);
	            break;
	        case OP_PRESSED_END:
	            cursor = op_pressed_end(cursor);
	            break;
	        case OP_PRINT_LINE:
	            op_print_line(cursor);
	            break;
	        case OP_PRINT_TEXT:
	            op_print_text(text);
	            break;
	    }
	} while(op[0] != OP_QUIT);

	free_text(text);
	return 0;
}

//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################
//##################################################################################################################################################################


struct text_node *create_text(struct line_node **pcursor)
{
    TEXT * newText;
    newText = (TEXT*)malloc(sizeof(TEXT));
    newText->line_begin = (LINE*)malloc(sizeof(LINE));
    newText->line_begin->ch='\n';
    newText->line_begin->next_char = NULL;
    newText->line_begin->prev_char = NULL;
    newText->line_begin->line_header = newText;
    newText->next_line = NULL;
    newText->prev_line = NULL;
    *pcursor = newText->line_begin;
    return newText;
}

void free_text(struct text_node *text)
{
    if(text!=NULL)
    {
        TEXT * tp = text;
        LINE * lp = NULL;

        while(tp->next_line!=NULL)
        {
            lp = tp->line_begin;
            while(lp->next_char!=NULL)
            {
                lp = lp->next_char;
                free(lp->prev_char);
            }
            tp = tp->next_line;
            free(tp->prev_line);
        }
    }
}

struct line_node *op_insert_char(struct line_node *cursor, char ch)
{
    if(cursor!=NULL)
    {
        LINE * newC = (LINE*)malloc(sizeof(LINE));
        newC->ch = ch;
        newC->next_char = cursor;
        newC->prev_char = cursor->prev_char;
        newC->line_header = cursor->line_header;
        if(cursor->prev_char==NULL)                           //When cursor is on empty line
            cursor->line_header->line_begin = newC;
        else
            cursor->prev_char->next_char = newC;
        cursor->prev_char = newC;
        return cursor;
    }
}

struct line_node *op_pressed_enter(struct line_node *cursor)
{
    if(cursor!=NULL)
    {
        TEXT * newT = (TEXT*)malloc(sizeof(TEXT));               //Create new Line and inserted after cursors line
        newT->next_line = cursor->line_header->next_line;
        newT->prev_line = cursor->line_header;
        newT->line_begin = cursor;

        cursor->line_header->next_line = newT;

        LINE * newL = (LINE*)malloc(sizeof(LINE));              //Created new line node and stored \n in it
        newL->ch = '\n';                                        //Appended it instead of cursor
        newL->line_header=cursor->line_header;
        newL->next_char=NULL;
        newL->prev_char=cursor->prev_char;

        if(cursor->prev_char==NULL)  //Enter was hit in the beginning of the line
            cursor->line_header->line_begin = newL;
        else
            cursor->prev_char->next_char = newL;

        cursor->prev_char = NULL;

        LINE * p = cursor;
        while(p!=NULL)
        {
            p->line_header = newT;
            p=p->next_char;
        }
        return cursor;
    }
}

struct line_node *op_pressed_delete(struct line_node *cursor)
{
    return NULL;
}

struct line_node *op_pressed_left(struct line_node *cursor)
{
    return NULL;

}

struct line_node *op_pressed_down(struct line_node *cursor)
{
    return NULL;
}

struct line_node *op_pressed_up(struct line_node *cursor)
{
    return NULL;
}

struct line_node *op_pressed_right(struct line_node *cursor)
{
    return NULL;
}

struct line_node *op_pressed_home(struct line_node *cursor)
{
    return NULL;
}

struct line_node *op_pressed_end(struct line_node *cursor)
{
    return NULL;
}

void op_print_line(struct line_node *cursor)
{
    LINE * p;
    p=cursor->line_header->line_begin;
    while(p!=NULL)
    {
        printf("%c",p->ch);
        p=p->next_char;
    }
}

void op_print_text(struct text_node *text)
{
    TEXT * p;
    p=text;
    while(p!=NULL)
    {
        op_print_line(p->line_begin);
        p=p->next_line;
    }
}
