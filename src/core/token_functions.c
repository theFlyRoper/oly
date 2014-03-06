/* modifies a 
 * will always return a (char **) where there is at least one string with 
 * an empty value. the two counters may be null but status, string and delims are required.
 */
char **
token_str_to_array(char *s, char *delims, unsigned int *count_chars, 
        unsigned int *count_tokens, oly_status *status) 
{
    int32_t             num_chars = 0, num_tokens = 0;
    /* we need arr_size char pointers and arr_size (for the EOLs) + num_chars chars. */
    char          **result, *c = xstrdup(s), *r, *rest;
    
    assert(s != NULL && delims != NULL && status != NULL);
    num_chars = 0;
    r = strtok_r(c, delims, &rest);
    while((r) = strtok_r(NULL, delims, &rest)) {
        (num_tokens)++;
    }
    result = (char **)xmalloc((num_tokens)*sizeof(char *));
    XFREE(c);
    num_tokens = 0;
    c = xstrdup(s);
    r = strtok_r(c, delims, &rest);
    result[(num_tokens)++] = r;
    (num_chars) += strlen(r);
    while(r = strtok_r(NULL, delims, &rest))
    {
        result[(num_tokens)++] = r;
        (num_chars)+=strlen(r);
    }
    
    if (count_tokens != NULL) {
        *count_tokens = num_tokens;
    }
    if (count_chars != NULL) {
        *count_chars = num_chars;
    }
    return result;
}

