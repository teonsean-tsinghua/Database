#ifndef DBTABLE_H_INCLUDED
#define DBTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
typedef struct TABLE {
  unsigned int row_num;
  unsigned int col_num;
  unsigned int *col_max_width;
  const char ***content;
  bool b;
} table;

extern void format_table(table *t, char *formated_out);

#endif // DBTABLE_H_INCLUDED
