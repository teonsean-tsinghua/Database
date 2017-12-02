#include "DBTable.h"

#define CORNER_FLAG "+"
#define ROW_LINE "-"
#define EDGE_LINE "="
#define COL_PADDING " "
#define COL_LINE "|"

static int str_len(const char *str){
  int len = 0;
  int offset = 2;
  int word_len = strlen(str);
  int i = 0;

  for(i; i < word_len; i++){
    if(str[i] < 128){
      len += 1;
    } else {
      len += 2;
      i += offset;
    }
  }
  return len;
}

static void make_edge(const unsigned int *col_width, const unsigned int col_num, char *edge){
  memset(edge, 0 ,sizeof(edge));
  strcat(edge, CORNER_FLAG);
  int col_line_counter;
  int row_line_counter;
  for( col_line_counter = 0; col_line_counter < col_num; col_line_counter++ ){
      strcat(edge, ROW_LINE);
      for( row_line_counter = 0; row_line_counter < col_width[col_line_counter]; row_line_counter++ ){
        strcat(edge, ROW_LINE);
      }
      strcat(edge, ROW_LINE);
      strcat(edge, CORNER_FLAG);
  }
  strcat(edge, "\n");
}

static int get_max_col_width(int *col_width, const unsigned int row_num){
  int max_col_width = 0;
  int i = 0;
  for(i; i < row_num; i++){
    if( max_col_width < col_width[i]){
      max_col_width = col_width[i];
    }
  }
  return max_col_width;
}

static void get_max_col(int **col_width, const unsigned int row_num, const unsigned int col_num, unsigned int *max_col_width){
  int i;
  int j;
  for(i = 0; i < col_num; i++){
    int max_width[ row_num ];
    for(j = 0; j < row_num; j++){
      max_width[j] = *((int *)col_width + (j * col_num) + i);
    }
    max_col_width[i] = get_max_col_width(max_width, row_num);
  }
}

static void get_col_width(table *t, int **col_width){
  int i = 0;
  int j;
  for(i; i < t->row_num; i++){
    for(j = 0; j < t->col_num; j++){
      *((int *)col_width + i * t->col_num + j) = str_len(t->content[i][j]);
    }
  }
}

static void set_table_col_max_width(table *t){
  int col_width[t->row_num][t->col_num];
  get_col_width(t, (int **)col_width);
  get_max_col((int **)col_width, t->row_num, t->col_num, t->col_max_width);
}

static int get_table_width(table *t){
  int col_padding_width = 2 * t->col_num;
  int col_line_width = t->col_num + 1;
  int col_width = 0;
  int i;
  for(i = 0; i < t->col_num; i++){
    col_width += t->col_max_width[i];
  }
  return col_line_width + col_padding_width + col_width;
}

static void set_space_txt(char *str, const unsigned int space_num){
  int i;
  for(i = 0; i < space_num; i++){
    strcat(str, " ");
  }
}

static void make_row(const unsigned int row_id, table *t, char *row_txt){
  int i;
  memset(row_txt, 0, sizeof(row_txt));
  strcat(row_txt, COL_LINE);
  for(i = 0; i < t->col_num; i++){
    strcat(row_txt, COL_PADDING);
    int word_len = str_len(t->content[row_id][i]);
    strcat(row_txt, t->content[row_id][i]);
    set_space_txt(row_txt, t->col_max_width[i] - word_len);
    strcat(row_txt, COL_PADDING);
    strcat(row_txt, COL_LINE);
  }
}

void format_table(table *t, char *formated_out){
  set_table_col_max_width(t);
  int col_width = get_table_width(t);
  char row_txt[col_width + t->col_num];
  int i;

  make_edge(t->col_max_width, t->col_num, row_txt);
  strcat(formated_out, row_txt);

  for( i = 0; i < t->row_num; i++){
    make_row(i, t, row_txt);
    strcat(formated_out, row_txt);
    strcat(formated_out, "\n");
    make_edge(t->col_max_width, t->col_num, row_txt);
    strcat(formated_out, row_txt);
  }
}
