/**
 * Variable evaluation
 *
 * Copyright (C) 2003-2007 by
 * Jeffrey Fulmer - <jeff@joedog.org>, et al. 
 * This file is part of siege
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <eval.h>
#include <hash.h>
#include <joedog/joedog.h>

char *
evaluate(HASH hash_table, char *buf)
{
  int  x   = 0;
  int  ENV = 0;
  int  len = 0;
  char final[BUFSIZE];
  char *ptr;
  char *string;
  const char *scan;
  
  char *result = xrealloc(buf, BUFSIZE * sizeof(char));
  if(result != NULL)
    buf = result;

  scan = strchr(buf, '$') + 1;
  len  = (strlen(buf) - strlen(scan)) -1;
 
  if(scan[0] == '{' || scan[0] == '(')
    scan++;
 
  ptr = (char*)scan;
  
  while(*scan && *scan != '}' && *scan != ')' && *scan != '/'){
    scan++;
    x++;
  }
 
  if(scan[0] == '}' || scan[0] == ')')
    scan++;
 
  string = substring(ptr, 0, x);
  if(hash_lookup( hash_table, string) == 0){
    if(getenv(string) != NULL)
      ENV = 1;
    else{
      string = '\0'; /* user botched his config file */
    }
  }
 
  memset(final, 0, sizeof final);
  strncpy( final, buf, len);
  if(string != NULL)
    strcat(final, ENV==0?hash_get(hash_table, string):getenv(string));
  strcat(final, scan );
  memset(result, 0, BUFSIZE * sizeof(char));
  strncpy(result, final, strlen(final));
  
  xfree(string);
  return result;
}
 
