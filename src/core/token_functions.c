/* modifies a 
 * will always return a (char **) where there is at least one string with 
 * an empty value. 
 */
char **
token_str_to_array(char *s, char *delims, int *count_chars, int *count_tokens,
        oly_status *status) 
{
    int32_t    arr_ptr = 0, strsize = 0, token_flag = 0;
    /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
    char          **result, *c = s, *r;

    assert(count_chars != NULL && count_tokens != NULL && s != NULL && delims != NULL);
    *count_chars = 0;
    *count_tokens = 1;
        
    for (c; (*c != '\0'); c++) 
    {
        printf("firstloop!\n");
        r = strchr(delims, *c);
        if ( r == NULL )
        {
            token_flag = 0;
            *count_chars++;
        } else if ((r != NULL) && (token_flag == 0) && ((r+strspn(r, delims))!='\0'))
        {
            token_flag = 1;
            *count_tokens++;
        }
    }
    result = (char **)xmalloc(((int)count_tokens*(sizeof(char *))));
    result[arr_ptr] = s;
    *status = OLY_OKAY;
    token_flag = 0;
    for (c = s; (*c != '\0'); c++) 
    {
        printf("nextloop!\n");
        r = strchr(delims, *c);
        if ( r == NULL )
        {
            token_flag = 0;
        } else if ((r != NULL) && (token_flag == 0) && ((r+strspn(r, delims))!='\0'))
        {
            result[++arr_ptr] = (r+strspn(r,delims));
            *r = '\0';
            token_flag = 1;
        }
    }
    return result;
}

int32_t
count_tokens (char *s, char *delims)
{
  int32_t  current_count = 0;

  /* loop over s, adding 1 to current_count for each iteration. */
  if ( delims != NULL ) {
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
      current_count++;
      s = strpbrk(s, delims);
    }
  }
  return current_count;
}

int32_t
count_nondelim_chars (char *s, char *delims)
{
    int32_t  current_count = 0;
    char    *next;

    /* delims should never be null. */
    assert( delims != NULL ) ;
    /* loop over s, adding the difference between the next and the primary */
    while ( s != NULL && *(s += strspn(s, delims)) != '\0' ) {
        next = strpbrk(s, delims);
        if ( next != NULL ) {
        current_count += ( next - s );
        } else {
        current_count += strlen(s);
        }
        s = next;
    }
    return current_count;
}

