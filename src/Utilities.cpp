#include <string.h>

// utility string function
void str_replace(char *src, char *oldchars, char *newchars) 
{ 
  char *p = strstr(src, oldchars);
  char buf[99];
  do 
  {
    if (p) 
    {
      memset(buf, '\0', strlen(buf));
      if (src == p) 
      {
        strcpy(buf, newchars);
        strcat(buf, p + strlen(oldchars));
      } 
      else 
      {
        strncpy(buf, src, strlen(src) - strlen(p));
        strcat(buf, newchars);
        strcat(buf, p + strlen(oldchars));
      }
      memset(src, '\0', strlen(src));
      strcpy(src, buf);
    }
  } while (p && (p = strstr(src, oldchars)));
}